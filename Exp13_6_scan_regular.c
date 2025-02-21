/* ============================================================================ */
/*	    Exp13_6_scan_regular.c : A/D 컨버터의 스캔 모드(레귤러 채널)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned short ADC0_result, ADC1_result, ADC2_result, ADC3_result;
  unsigned short ADC17_result, ADC18a_result, ADC18b_result;
  float temperature;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp13_6.c    ");
  Beep();

  TFT_string(0, 3,White,Magenta,"  A/D 컨버터의 스캔 모드 (레귤러 채널)  ");
  TFT_string(8, 7,White,Black,"ADC0  (+5.0V) = +0.00V");
  TFT_string(8,10,White,Black,"ADC1  (+3.3V) = +0.00V");
  TFT_string(8,13,White,Black,"ADC2  ( VR1 ) = +0.00V");
  TFT_string(8,16,White,Black,"ADC3 (LM35DZ) = +00.0 C");
  TFT_string(8,19,White,Black,"ADC17 (+1.2V) = +0.00V");
  TFT_string(8,22,White,Black,"ADC18 (Temp.) = +00.0 C");
  TFT_string(8,25,White,Black,"ADC18 (+VBAT) = +0.00V");
  TFT_xy(29,16);
  TFT_English(0x7F);
  TFT_xy(29,22);
  TFT_English(0x7F);

  GPIOA->MODER |= 0x000000FF;			// use ADC0~ADC3
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00800000;			// enable channel 17~18, ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR1 = 0x07E00000;			// sampling time of channel 17~18 = 480 cycle
  ADC1->SMPR2 = 0x00000249;			// sampling time of channel 0~3 = 15 cycle
  ADC1->CR1 = 0x00000100;			// 12-bit resolution, SCAN = 1
  ADC1->CR2 = 0x00000401;			// right alignment, EOCS = 1, CONT = 0, ADON = 1
  ADC1->SQR1 = 0x00600000;			// total regular channel number = 6
  ADC1->SQR2 = 0x00000012;			// channel 18b
  ADC1->SQR3 = 0x25118820;			// channel 18, 17, 3, 2, 1, 0

  while(1)
    { ADC1->CR2 |= 0x40000000;			// start conversion by software

      while(!(ADC1->SR & 0x00000002));		// channel 0 (+5.0V) : wait for EOC
      ADC0_result = ADC1->DR;

      while(!(ADC1->SR & 0x00000002));		// channel 1 (+3.3V) : wait for EOC
      ADC1_result = ADC1->DR;

      while(!(ADC1->SR & 0x00000002));		// channel 2 (VR1) : wait for EOC
      ADC2_result = ADC1->DR;

      while(!(ADC1->SR & 0x00000002));		// channel 3 (LM35DZ) : wait for EOC
      ADC3_result = ADC1->DR;

      ADC->CCR &= 0xFFBFFFFF;			// set channel 18 = temperature sensor
      Delay_us(5);
      while(!(ADC1->SR & 0x00000002));		// channel 17 (VREFINT = 1.21V) : wait for EOC
      ADC17_result = ADC1->DR;

      while(!(ADC1->SR & 0x00000002));		// channel 18 (temperature sensor) : wait for EOC
      ADC18a_result = ADC1->DR;

      ADC->CCR |= 0x00400000;			// set channel 18 = VBAT/4
      Delay_us(5);
      while(!(ADC1->SR & 0x00000002));		// channel 18 (VBAT/4) : wait for EOC
      ADC18b_result = ADC1->DR;

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

      TFT_xy(24,19);				// display channel 17 (VREFINT = 1.21V)
      TFT_color(Magenta,Black);
      TFT_signed_float((float)ADC17_result*3.3/4095., 1, 2);

      TFT_xy(24,22);				// display channel 18 (temperature sensor)
      TFT_color(Magenta,Black);
      temperature = (float)ADC18a_result*3.3/4095.;	// calculate temperature
      temperature = (temperature-0.76)/0.0025 + 25.;
      TFT_signed_float(temperature, 2, 1);

      TFT_xy(24,25);				// display channel 18 (VBAT/4)
      TFT_color(Magenta,Black);
      TFT_signed_float((float)ADC18b_result*4.*3.3/4095., 1, 2);

      LED_toggle();				// blink LED1
      Delay_ms(250);
    }
}
