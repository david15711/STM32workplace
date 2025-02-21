/* ============================================================================ */
/*	Exp13_2_thermometer.c : 매우 큰 글씨로 표시한 디지털 온도계		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"
#include "OK-STM767_xlarge.h"

int main(void)
{
  unsigned int i, result;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp13_2.c    ");
  Beep();

  TFT_string(0,1,Green,  Black,"****************************************");
  TFT_string(0,3,Magenta,Black,"           디 지 털  온 도 계           ");
  TFT_string(0, 5,Green, Black,"****************************************");
  TFT_string(0,27,Green, Black,"****************************************");
  TFT_string(3,27,White, Black,"(설계/제작:윤덕용)");

  Rectangle(30,105, 289,171, Cyan);
  Rectangle(31,106, 288,170, Cyan);

  TFT_xy(7,14);
  TFT_color(White,Black);
  TFT_English_xlarge('+');
  TFT_xy(21,17);
  TFT_color(Yellow,Black);
  TFT_English_large(0x7F);
  TFT_English_large('C');

  GPIOA->MODER |= 0x000000C0;			// use ADC3
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000200;			// sampling time of channel 3 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;			// total regular channel number = 1
  ADC1->SQR3 = 0x00000003;			// channel 3 (LM35DZ)

  while(1)
    { result = 0;
      for(i = 0; i < 256; i++)
        { ADC1->CR2 |= 0x40000000;		// start conversion by software
          while(!(ADC1->SR & 0x00000002));	// wait for end of conversion
          result += ADC1->DR;
          Delay_ms(1);
        }

      result >>= 8;				// calculate average
      TFT_xy(12,14);				// display temperature
      TFT_color(White,Black);
      TFT_unsigned_float_xlarge((float)result*330./4095., 2, 1);

      Delay_ms(245);
    }
}
