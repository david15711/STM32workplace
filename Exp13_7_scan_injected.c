/* ============================================================================ */
/*	 Exp13_7_scan_injected.c : A/D 컨버터의 스캔 모드(인젝티드 채널)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned short ADC0_result, ADC1_result, ADC2_result, ADC3_result;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp13_7.c    ");
  Beep();

  TFT_string(0, 3,White,Magenta," A/D 컨버터의 스캔 모드 (인젝티드 채널) ");
  TFT_string(8, 7,White,Black,"ADC0  (+5.0V) = +0.00V");
  TFT_string(8,10,White,Black,"ADC1  (+3.3V) = +0.00V");
  TFT_string(8,13,White,Black,"ADC2  ( VR1 ) = +0.00V");
  TFT_string(8,16,White,Black,"ADC3 (LM35DZ) = +00.0 C");
  TFT_xy(29,16);
  TFT_English(0x7F);

  GPIOA->MODER |= 0x000000FF;			// use ADC0~ADC3
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000249;			// sampling time of channel 0~3 = 15 cycle
  ADC1->CR1 = 0x00000100;			// 12-bit resolution, SCAN = 1
  ADC1->CR2 = 0x00000001;			// right alignment, EOCS = 0, CONT = 0, ADON = 1
  ADC1->JSQR = 0x00318820;			// total injected channel number = 4 (3, 2, 1, 0)

  while(1)
    { ADC1->CR2 |= 0x00400000;			// start conversion by software
      while(!(ADC1->SR & 0x00000004));		// wait for JEOC
      ADC0_result = ADC1->JDR1;
      ADC1_result = ADC1->JDR2;
      ADC2_result = ADC1->JDR3;
      ADC3_result = ADC1->JDR4;

      TFT_xy(24,7);				// display channel 0 (+5.0V)
      TFT_color(Cyan,Black);
      TFT_signed_float((float)ADC0_result*2.*3.3/4095., 1, 2);

      TFT_xy(24,10);				// display channel 1 (+3.3V)
      TFT_color(Cyan,Black);
      TFT_signed_float((float)ADC1_result*2.*3.3/4095., 1, 2);

      TFT_xy(24,13);				// display channel 2 (VR1)
      TFT_color(Cyan,Black);
      TFT_signed_float((float)ADC2_result*3.3/4095., 1, 2);

      TFT_xy(24,16);				// display channel 3 (LM35DZ)
      TFT_color(Cyan,Black);
      TFT_signed_float((float)ADC3_result*330./4095., 2, 1);

      LED_toggle();				// blink LED1
      Delay_ms(250);
    }
}
