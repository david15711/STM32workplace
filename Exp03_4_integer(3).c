/* ============================================================================ */
/*		Exp03_4_integer(3).c : 정수 데이터 출력 프로그램(3)		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include <stdio.h>
#include "stm32f767xx.h"
#include "OK-STM767.h"

int putchar(int character)			/* output a character to stream */
{
  LCD_data(character);

  return character;
}

int main(void)
{
  unsigned int i, j;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();                             // initialize text LCD module

  while(1)
    { LCD_string(0x80," INTEGER = 000  ");      // display 8-bit integer
      LCD_string(0xC0,"= 00H = 00000000");
      Beep();

      for(i = 0; i <= 255; i++)			// integer number from 0 to 255
        { LCD_command(0x8B);                    // decimal number
          printf("%3d", i);
          LCD_command(0xC2);                    // hexadecimal number
          printf("%02X", i);
          LCD_command(0xC8);                    // binary number
          LCD_binary(i,8);
          Delay_ms(250);
          if((i % 8) == 0) Delay_ms(1000);
	}

      LCD_string(0x80," MULTI TABLE(1) ");      // display multiplication table(1)
      LCD_string(0xC0,"   0 x 0 = 00   ");
      Beep();

      for(i = 2; i <= 9; i++)
        for(j = 1; j <= 9; j++)
          { LCD_command(0xC3);			// display multiplicand
	    LCD_data(i+'0');
            LCD_command(0xC7);			// display multiplier
	    LCD_data(j+'0');
            LCD_command(0xCB);			// display multiplication
	    printf("%2d", i*j);
            Delay_ms(1000);
          }

      LCD_string(0x80," MULTI TABLE(2) ");      // display multiplication table(2)
      LCD_string(0xC0," 00 x 00 = 0000 ");
      Beep();

      for(i = 10; i <= 90; i += 10)
        for(j = 10; j <= 90; j += 10)
          { LCD_command(0xC1);			// display multiplicand
	    printf("%2d", i);
            LCD_command(0xC6);			// display multiplier
	    printf("%2d", j);
            LCD_command(0xCB);			// display multiplication
	    printf("%4d", i*j);
            Delay_ms(1000);
          }
    }
}
