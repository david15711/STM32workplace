/* ============================================================================ */
/*		Exp05_6_visiting(2).c : Visiting Count Program(2)	        */
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include <stdlib.h>
#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned int i, j, random, total = 0, visit_flag, x, y;
  unsigned char key, count, table[40][15];

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  TFT_landscape_mode();				// landscape mode

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp05_6.c    ");

  SysTick->LOAD  = 0x00FFFFFF;			// maximum value
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000001;			// SysTick enable and no interrupt

  while(1)
    { TFT_string(6,10, White,Magenta, " Visiting Count Program (2) "); // wait KEY1 to start
      TFT_string(8,15,   Red,  Black, " Press KEY1 to start !! ");
      TFT_string(3,20, White,  Black, " (Total Visiting Counts = ");
      TFT_unsigned_decimal(total,1,6);
      TFT_English(')');
      TFT_English(' ');
      Beep();

      key = 0;					// random starting time
      while(key != KEY1) key = Key_input();
      random = SysTick->VAL;
      srand(random);				// initialize random number

      TFT_color(White,Black);
      for(j = 0; j <= 14; j++)			// clear visiting room
        for(i = 0; i <= 39; i++)
          { table[i][j] = 0;
            TFT_xy(i, 2*j);
            TFT_English('0');
          }
      visit_flag = 0;
      total = 0;

      x = rand() % 40;				// starting position
      y = rand() % 15;

      while(visit_flag == 0)
        { random = rand();			// get random number(0x00000000~0x3FFFFFFF)

          if(random <= 0x0FFFFFFF)		// 0x00000000 - 0x0FFFFFFF
            { if(x == 39) x = 0;
              else        x++;
	      table[x][y] += 1;
            }
          else if(random <= 0x1FFFFFFF)		// 0x10000000 - 0x1FFFFFFF
            { if(x == 0)  x = 39;
              else        x--;
	      table[x][y] += 1;
	    }
          else if(random <= 0x2FFFFFFF)		// 0x20000000 - 0x2FFFFFFF
            { if(y == 14) y = 0;
              else        y++;
	      table[x][y] += 1;
	    }
          else if(random <= 0x3FFFFFFF)		// 0x30000000 - 0x3FFFFFFF
            { if(y == 0)  y = 14;
              else        y--;
	      table[x][y] += 1;
	    }

          count = table[x][y];			// display visiting count
          if(count >= 62)
            { count = '*';
              TFT_color(Yellow,Blue);
	    }
          else if(count >= 36)
	    { count = count - 36 + 'a';
              TFT_color(Cyan,Blue);
	    }
          else if(count >= 10)
	    { count = count - 10 + 'A';
              TFT_color(Green,Blue);
	    }
          else
	    { count = count + '0';
              TFT_color(Magenta,Blue);
 	    }
          TFT_xy(x, 2*y);
          TFT_English(count);

          visit_flag = 1;			// check end
          for(j = 0; j <= 14; j++)
            for(i = 0; i <= 39; i++)
              if(table[i][j] == 0)
                visit_flag = 0;

          total++;				// increment total count
          Delay_ms(10);
        }
    }
}
