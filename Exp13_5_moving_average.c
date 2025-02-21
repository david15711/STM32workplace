/* ============================================================================ */
/*	Exp13_5_moving_average.c : A/D º¯È¯ °á°úÀÇ Æò±Õ Ã³¸®(ÀÌµ¿Æò±Õ)		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned char i, ADC0_count = 0, ADC1_count = 0, ADC2_count = 0, ADC3_count = 0;
  unsigned char ADC17_count = 0, ADC18a_count = 0, ADC18b_count = 0;
  unsigned char ADC0_start_flag = 0, ADC1_start_flag = 0, ADC2_start_flag = 0, ADC3_start_flag = 0;
  unsigned char ADC17_start_flag = 0, ADC18a_start_flag = 0, ADC18b_start_flag = 0;
  unsigned short result, ADC0_buffer[16], ADC1_buffer[16], ADC2_buffer[16], ADC3_buffer[16];
  unsigned short ADC17_buffer[16], ADC18a_buffer[16], ADC18b_buffer[16];
  unsigned int count;
  float temperature;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp13_5.c    ");
  Beep();

  TFT_string(0, 3,White,Magenta,"   A/D º¯È¯ °á°úÀÇ Æò±Õ Ã³¸®(ÀÌµ¿Æò±Õ)  ");
  TFT_string(8, 7,White,Black,"ADC0  (+5.0V) = +0.00V");
  TFT_string(8,10,White,Black,"ADC1  (+3.3V) = +0.00V");
  TFT_string(8,13,White,Black,"ADC2  ( VR1 ) = +0.00V");
  TFT_string(8,16,White,Black,"ADC3 (LM35DZ) = +00.0 C");
  TFT_string(8,19,White,Black,"ADC17 (+1.2V) = +0.00V");
  TFT_string(8,22,White,Black,"ADC18 (Temp.) = +00.0 C");
  TFT_string(8,25,White,Black,"ADC18 (+VBAT) = +0.00V");
  TFT_string(9,28,White,Black,"                     00.000 ms");
  TFT_xy(29,16);
  TFT_English(0x7F);
  TFT_xy(29,22);
  TFT_English(0x7F);

  GPIOA->MODER |= 0x000000FF;			// use ADC0~ADC3
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00800000;			// enable channel 17~18, ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR1 = 0x07E00000;			// sampling time of channel 17~18 = 480 cycle
  ADC1->SMPR2 = 0x00000249;			// sampling time of channel 0~3 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;			// total regular channel number = 1

  SysTick->LOAD  = 0x00FFFFFF;			// maximum value
  SysTick->CTRL  = 0x00000001;			// SysTick enable and clock source = 216MHz/8
  SysTick->VAL   = 0;				// clear SysTick Counter

  while(1)
    { SysTick->VAL = 0;		        	// start measurement of A/D conversion time

      ADC1->SQR3 = 0x00000000;			// ***** channel 0 (+5.0V) *****
      ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
      result = ADC1->DR;

      if(ADC0_start_flag == 1)			// if not start new, store result
        { ADC0_buffer[ADC0_count] = result;
          ADC0_count++;
          ADC0_count = ADC0_count % 16;
        }
      else
        { for(i = 0; i < 16; i++)		// if start new, initialize buffer 
            ADC0_buffer[i] = result;
          ADC0_start_flag = 1;
        }

      result = 0;
      for(i = 0; i < 16; i++)			// calculate moving average of buffer
        result += ADC0_buffer[i];
      result >>= 4;

      count = SysTick->VAL;			// calculate A/D conversion time
      count = 0x00FFFFFF + 1 - count;
      TFT_color(Green,Black);
      TFT_xy(30,28);
      TFT_unsigned_float((float)count/27000.,2,3);

      TFT_xy(24,7);				// display voltage
      TFT_color(Cyan,Black);
      TFT_signed_float((float)result*2.*3.3/4095., 1, 2);

      ADC1->SQR3 = 0x00000001;			// ***** channel 1 (+3.3V) *****
      ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
      result = ADC1->DR;

      if(ADC1_start_flag == 1)			// if not start new, store result
        { ADC1_buffer[ADC1_count] = result;
          ADC1_count++;
          ADC1_count = ADC1_count % 16;
        }
      else
        { for(i = 0; i < 16; i++)		// if start new, initialize buffer 
            ADC1_buffer[i] = result;
          ADC1_start_flag = 1;
        }

      result = 0;
      for(i = 0; i < 16; i++)			// calculate moving average of buffer
        result += ADC1_buffer[i];
      result >>= 4;
      TFT_xy(24,10);				// display voltage
      TFT_color(Cyan,Black);
      TFT_signed_float((float)result*2.*3.3/4095., 1, 2);

      ADC1->SQR3 = 0x00000002;			// ***** channel 2 (VR1) *****
      ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
      result = ADC1->DR;

      if(ADC2_start_flag == 1)			// if not start new, store result
        { ADC2_buffer[ADC2_count] = result;
          ADC2_count++;
          ADC2_count = ADC2_count % 16;
        }
      else
        { for(i = 0; i < 16; i++)		// if start new, initialize buffer 
            ADC2_buffer[i] = result;
          ADC2_start_flag = 1;
        }

      result = 0;
      for(i = 0; i < 16; i++)			// calculate moving average of buffer
        result += ADC2_buffer[i];
      result >>= 4;
      TFT_xy(24,13);				// display voltage
      TFT_color(Cyan,Black);
      TFT_signed_float((float)result*3.3/4095., 1, 2);

      ADC1->SQR3 = 0x00000003;			// ***** channel 3 (LM35DZ) *****
      ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
      result = ADC1->DR;

      if(ADC3_start_flag == 1)			// if not start new, store result
        { ADC3_buffer[ADC3_count] = result;
          ADC3_count++;
          ADC3_count = ADC3_count % 16;
        }
      else
        { for(i = 0; i < 16; i++)		// if start new, initialize buffer 
            ADC3_buffer[i] = result;
          ADC3_start_flag = 1;
        }

      result = 0;
      for(i = 0; i < 16; i++)			// calculate moving average of buffer
        result += ADC3_buffer[i];
      result >>= 4;
      TFT_xy(24,16);				// display temperature
      TFT_color(Cyan,Black);
      TFT_signed_float((float)result*330./4095., 2, 1);

      ADC->CCR &= 0xFFBFFFFF;			// set channel 18 = temperature sensor
      Delay_us(5);
      ADC1->SQR3 = 0x00000011;			// ***** channel 17 (VREFINT = 1.21V) *****
      ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
      result = ADC1->DR;

      if(ADC17_start_flag == 1)			// if not start new, store result
        { ADC17_buffer[ADC17_count] = result;
          ADC17_count++;
          ADC17_count = ADC17_count % 16;
        }
      else
        { for(i = 0; i < 16; i++)		// if start new, initialize buffer 
            ADC17_buffer[i] = result;
          ADC17_start_flag = 1;
        }

      result = 0;
      for(i = 0; i < 16; i++)			// calculate moving average of buffer
        result += ADC17_buffer[i];
      result >>= 4;
      TFT_xy(24,19);				// display voltage
      TFT_color(Magenta,Black);
      TFT_signed_float((float)result*3.3/4095., 1, 2);

      ADC1->SQR3 = 0x00000012;			// ***** channel 18 (temperature sensor) *****
      ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
      result = ADC1->DR;

      if(ADC18a_start_flag == 1)		// if not start new, store result
        { ADC18a_buffer[ADC18a_count] = result;
          ADC18a_count++;
          ADC18a_count = ADC18a_count % 16;
        }
      else
        { for(i = 0; i < 16; i++)		// if start new, initialize buffer 
            ADC18a_buffer[i] = result;
          ADC18a_start_flag = 1;
        }

      result = 0;
      for(i = 0; i < 16; i++)			// calculate moving average of buffer
        result += ADC18a_buffer[i];
      result >>= 4;
      temperature = (float)result*3.3/4095.;	// calculate temperature
      temperature = (temperature-0.76)/0.0025 + 25.;
      TFT_xy(24,22);				// display temperature
      TFT_color(Magenta,Black);
      TFT_signed_float(temperature, 2, 1);

      ADC->CCR |= 0x00400000;			// set channel 18 = VBAT/4
      Delay_us(5);
      ADC1->SQR3 = 0x00000012;			// ***** channel 18 (VBAT/4) *****
      ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
      result = ADC1->DR;

      if(ADC18b_start_flag == 1)			// if not start new, store result
        { ADC18b_buffer[ADC18b_count] = result;
          ADC18b_count++;
          ADC18b_count = ADC18b_count % 16;
        }
      else
        { for(i = 0; i < 16; i++)		// if start new, initialize buffer 
            ADC18b_buffer[i] = result;
          ADC18b_start_flag = 1;
        }

      result = 0;
      for(i = 0; i < 16; i++)			// calculate moving average of buffer
        result += ADC18b_buffer[i];
      result >>= 4;
      TFT_xy(24,25);				// display voltage
      TFT_color(Magenta,Black);
      TFT_signed_float((float)result*4.*3.3/4095., 1, 2);

      LED_toggle();				// blink LED1
      Delay_ms(250);
    }
}
