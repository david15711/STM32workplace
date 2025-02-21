/* ============================================================================ */
/*	Exp03_1_shift.c : 텍스트 LCD 기본 출력 및 시프트 동작 프로그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned int i;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();                             // initialize text LCD module

  while(1)
    { LCD_string(0x80," OK-STM767 V1.0 ");      // display logo
      LCD_string(0xC0,"   2017/01/01   ");      // display message 1
      Beep();
      Delay_ms(2000);
      LCD_string(0xC0,"   Cortex-M7    ");      // display message 2
      Beep();
      Delay_ms(2000);
      LCD_string(0xC0," STM32F767VGT6  ");      // display message 3
      Beep();
      Delay_ms(2000);

      LCD_string(0x80,"0123456789ABCDEF0123456789ABCDEF"); // display long line
      LCD_string(0xC0,"Go shift left and right 16 times");
      Beep();
      Delay_ms(1000);

      for(i = 0; i < 16; i++)			// shift left
        { LCD_command(0x18);
          Delay_ms(300);
        }
      Beep();
      Delay_ms(1000);

      for(i = 0; i < 16; i++)			// shift right
        { LCD_command(0x1C);
          Delay_ms(300);
        }
      Beep();
      Delay_ms(1000);
    }
}
