/* ============================================================================ */
/*		Exp13_10_AWD.c : A/D 컨버터의 아날로그 워치독 기능		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned short result;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp13_10.c   ");
  Beep();

  TFT_string(0, 5,White,Magenta," A/D 컨버터의 아날로그 워치독(AWD) 기능 ");
  TFT_string(10,11,White,Black,"HT = 0x0C1F = +2.50V");
  TFT_string(10,14,White,Black,"LT = 0x04D9 = +1.00V");
  TFT_string(10,20,White,Black,"  VR1(ADC2) = +0.00V");
  TFT_string(24,11,Cyan,Black,"+2.50");
  TFT_string(24,14,Cyan,Black,"+1.00");

  GPIOA->MODER |= 0x00000030;			// use VR1(ADC2)
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000040;			// sampling time of channel 2 = 15 cycle
  ADC1->CR1 = 0x00800202;			// 12-bit resolution, AWD input = channel 2
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;			// total regular channel number = 1
  ADC1->SQR3 = 0x00000002;			// channel 2 (VR1)
  ADC1->HTR = 0x00000C1F;			// HT = 0x0C1F for analog watchdog
  ADC1->LTR = 0x000004D9;			// LT = 0x04D9

  while(1)
    { ADC1->SR = 0x00000000;			// clear AWD flag

      ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x000002));		// wait for end of conversion
      result = ADC1->DR;
      TFT_xy(24,20);				// display voltage
      TFT_color(Cyan,Black);
      TFT_signed_float((float)result*3.3/4095., 1, 2);

      if(ADC1->SR & 0x00000001)
        TFT_string(1,23,Yellow,Black,"VR1 is out of analog watchdog window. ");
      else
        TFT_string(1,23, Green,Black,"   VR1 is in analog watchdog window.  ");

      LED_toggle();				// blink LED1
      Delay_ms(250);
    }
}
