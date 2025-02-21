/* ============================================================================ */
/*		Exp13_9_dual_ADC.c : A/D 컨버터 2개의 동시 변환			*/
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
  LCD_string(0xC0,"   Exp13_9.c    ");
  Beep();

  TFT_string(0, 3,White,Magenta,"   A/D 컨버터의 듀얼 모드 (ADC1,ADC2)   ");
  TFT_string(8, 7,White,Black,"ADC0  (+5.0V) = +0.00V");
  TFT_string(8,10,White,Black,"ADC1  (+3.3V) = +0.00V");
  TFT_string(8,13,White,Black,"ADC2  ( VR1 ) = +0.00V");
  TFT_string(8,16,White,Black,"ADC3 (LM35DZ) = +00.0 C");
  TFT_xy(29,16);
  TFT_English(0x7F);

  GPIOA->MODER |= 0x000000FF;			// use ADC0~ADC3
  RCC->APB2ENR |= 0x00000300;			// enable ADC1, ADC2 clock

  ADC->CCR = 0x00008006;			// ADCCLK = 54MHz/2 = 27MHz
						// DMA mode 2, dual regular simultaneous mode
  ADC1->SMPR2 = 0x00000249;			// sampling time of channel 0~3 = 15 cycle
  ADC2->SMPR2 = 0x00000249;
  ADC1->CR1 = 0x00000100;			// 12-bit resolution, SCAN = 1
  ADC2->CR1 = 0x00000100;
  ADC1->CR2 = 0x00000401;			// right alignment, EOCS = 1, CONT = 0, ADON = 1
  ADC2->CR2 = 0x00000401;
  ADC1->SQR1 = 0x00100000;			// total regular channel number = 2
  ADC2->SQR1 = 0x00100000;
  ADC1->SQR3 = 0x00000040;			// ADC1 = channel 2, 0
  ADC2->SQR3 = 0x00000061;			// ADC2 = channel 3, 1

  while(1)
    { 
      ADC1->CR2 |= 0x40000000;			// start conversion by software
      ADC2->CR2 |= 0x40000000;

      while((ADC->CSR & 0x00000202) != 0x00000202); // wait for EOC
      ADC0_result = ADC->CDR & 0x0000FFFF;
      ADC1_result = ADC->CDR >> 16;
      ADC1->SR = 0x00000000;
      ADC2->SR = 0x00000000;

      while((ADC->CSR & 0x00000202) != 0x00000202); // wait for EOC
      ADC2_result = ADC->CDR & 0x0000FFFF;
      ADC3_result = ADC->CDR >> 16;
      ADC1->SR = 0x00000000;
      ADC2->SR = 0x00000000;

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
