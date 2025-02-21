/* ============================================================================ */
/*	Exp02_5_time_delay(2).c : 시간지연 함수의 정확성을 측정하는 프로그램   	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned int count;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp02_5.c    ");
  Beep();

  TFT_string(0, 2,White,Magenta,"   시간지연 사용자 함수의 정확성 측정   ");
  TFT_string(2, 5,Cyan,   Black,"  Delay_us(),Delay_ms() for 216MHz  ");
  TFT_string(2,11,White,  Black,"Delay_us(1)   = 0000000 = 000.000 us");
  TFT_string(2,13,White,  Black,"Delay_us(10)  = 0000000 = 000.000 us");
  TFT_string(2,15,White,  Black,"Delay_us(100) = 0000000 = 000.000 us");
  TFT_string(2,19,White,  Black,"Delay_ms(1)   = 0000000 = 000.000 ms");
  TFT_string(2,21,White,  Black,"Delay_ms(10)  = 0000000 = 000.000 ms");
  TFT_string(2,23,White,  Black,"Delay_ms(100) = 0000000 = 000.000 ms");
  Rectangle(5,80, 314,208, White);
  Line(5,144, 314,144, White);

  SysTick->LOAD  = 0x00FFFFFF;			// maximum value
  SysTick->CTRL  = 0x00000005;			// SysTick enable and interrupt no request
  SysTick->VAL   = 0;				// clear SysTick Counter

  while(1)
    { SysTick->CTRL  = 0x00000005;		// SysTick enable and clock source = 216MHz
      SysTick->VAL = 0;			        // Delay_us(1)
      Delay_us(1);
      count = SysTick->VAL;
      count = 0x00FFFFFF + 1 - count;
      TFT_color(Green,Black);
      TFT_xy(18,11);
      TFT_unsigned_decimal(count,0,7);
      TFT_xy(28,11);
      TFT_unsigned_float((float)count/216.,3,3);

      SysTick->VAL = 0;			        // Delay_us(10)
      Delay_us(10);
      count = SysTick->VAL;
      count = 0x00FFFFFF + 1 - count;
      TFT_xy(18,13);
      TFT_unsigned_decimal(count,0,7);
      TFT_xy(28,13);
      TFT_unsigned_float((float)count/216.,3,3);

      SysTick->VAL = 0;		        	// Delay_us(100)
      Delay_us(100);
      count = SysTick->VAL;
      count = 0x00FFFFFF + 1 - count;
      TFT_xy(18,15);
      TFT_unsigned_decimal(count,0,7);
      TFT_xy(28,15);
      TFT_unsigned_float((float)count/216.,3,3);

      SysTick->CTRL  = 0x00000001;		// SysTick enable and clock source = 216MHz/8
      SysTick->VAL = 0;		        	// Delay_ms(1)
      Delay_ms(1);
      count = SysTick->VAL;
      count = 0x00FFFFFF + 1 - count;
      TFT_color(Cyan,Black);
      TFT_xy(18,19);
      TFT_unsigned_decimal(count,0,7);
      TFT_xy(28,19);
      TFT_unsigned_float((float)count/27000.,3,3);

      SysTick->VAL = 0;		        	// Delay_ms(10)
      Delay_ms(10);
      count = SysTick->VAL;
      count = 0x00FFFFFF + 1 - count;
      TFT_xy(18,21);
      TFT_unsigned_decimal(count,0,7);
      TFT_xy(28,21);
      TFT_unsigned_float((float)count/27000.,3,3);

      SysTick->VAL = 0;			        // Delay_ms(100)
      Delay_ms(100);
      count = SysTick->VAL;
      count = 0x00FFFFFF + 1 - count;
      TFT_xy(18,23);
      TFT_unsigned_decimal(count,0,7);
      TFT_xy(28,23);
      TFT_unsigned_float((float)count/27000.,3,3);

      Delay_ms(500);
      LED_toggle();				// blink LED1
    }
}
