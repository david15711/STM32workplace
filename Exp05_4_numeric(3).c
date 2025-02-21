/* ============================================================================ */
/*	Exp05_4_numeric(3).c : TFT-LCD 수치 데이터 출력 프로그램(3)		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include <stdio.h>
#include "stm32f767xx.h"
#include "OK-STM767.h"

int putchar(int character)			/* output a character to stream */
{
  TFT_English(character);

  return character;
}

int main(void)
{
  unsigned int x, y;
  float a, b;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp05_4.c    ");

  while(1)
    { TFT_clear_screen();			// 정수
      TFT_string(6,0,White, Magenta," Multiplication Table(정수) ");

      for(x = 2; x <= 9; x++)
        { for(y = 1; y <= 9; y++)
	    { TFT_color(Green, Black);		// 단자리 구구단
	      TFT_xy( 5,3*y);
              printf("%1d x %1d = %2d", x, y, x*y);

              TFT_color(Yellow, Black);		// 10자리 구구단
	      TFT_xy(21,3*y);
              printf("%2d x %2d = %4d", x*10, y*10, (x*10)*(y*10));
            }
          Beep();
          Delay_ms(3000);
        }

      TFT_clear_screen();			// 실수
      TFT_string(6,0,White, Magenta," Multiplication Table(실수) ");

      for(x = 1,a = 1.1; x <= 9; x++, a += 1.1)
        { for(y = 1, b = 1.1; y <= 9; y++, b += 1.1)
	    { TFT_color(Green, Black);		// 실수 구구단
	      TFT_xy(11,3*y);
              printf("%3.1f x %3.1f = %5.2f", a, b, a*b);
            }
          Beep();
          Delay_ms(3000);
        }
    }
}
