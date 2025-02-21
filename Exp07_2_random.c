/* ============================================================================ */
/*		Exp07_2_random.c : 난수를 이용한 랜덤 그래픽			*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include <stdlib.h>
#include "stm32f767xx.h"
#include "OK-STM767.h"

void Gradation(U16 x1,U16 y1, U16 x2,U16 y2, U16 color); /* gradation for rectangle */

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned char i, key;
  unsigned short color, fill, random;
  signed short x1, y1, x2, y2, r;
  unsigned short Color[16] = { White, DarkGrey, Red, Green, Blue, Yellow, Cyan, Magenta,
                               Pink, Purple, LightGrey, Violet, Navy, Khaki, DarkGreen, Olive };

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp07_2.c    ");
  Beep();

  TFT_string(6,10, White,Magenta, " 난수를 이용한 랜덤 그래픽 "); // wait KEY1 to start
  TFT_string(8,15,  Cyan,  Black, " Press KEY1 to start ! ");

  SysTick->LOAD  = 0x00FFFFFF;			// maximum value
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000001;			// SysTick enable and no interrupt

  key = 0;					// random starting time
  while(key != KEY1) key = Key_input();
  random = SysTick->VAL;
  srand(random);
  TFT_clear_screen();				// clear screen

  while(1)
    { Rectangle(0,0, 319,239, Magenta);		// graphic show 1(line 1)
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0; i < 100; i++)
        { x2 = rand() % 320;
          y2 = rand() % 240;

	  Line(2,2, x2,y2, White);
          Delay_ms(30);
        }
      Delay_ms(1000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 319,239, Magenta);		// graphic show 2(line 2)
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0; i < 100; i++)
        { x2 = rand() % 320;
          y2 = rand() % 240;

	  Line(2,237, x2,y2, Yellow);
          Delay_ms(30);
        }
      Delay_ms(1000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 319,239, Magenta);		// graphic show 3(line 3)
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0; i < 100; i++)
        { x2 = rand() % 320;
          y2 = rand() % 240;

	  Line(317,237, x2,y2, Green);
          Delay_ms(30);
        }
      Delay_ms(1000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 319,239, Magenta);		// graphic show 4(line 4)
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0; i < 100; i++)
        { x2 = rand() % 320;
          y2 = rand() % 240;

	  Line(317,2, x2,y2, Cyan);
          Delay_ms(30);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 319,239, Magenta);		// graphic show 5(rectangle)
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0; i < 50; i++)
        { x1 = rand() % 320;
          y1 = rand() % 240;
          x2 = rand() % 320;
          y2 = rand() % 240;
          color = rand() % 16;

	  Rectangle(x1,y1, x2,y2, Color[color]);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 319,239, Magenta);		// graphic show 6(block)
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0; i < 30; i++)
        { x1 = rand() % 320;
          y1 = rand() % 240;
          x2 = rand() % 320;
          y2 = rand() % 240;
          color = rand() % 16;
          fill = rand() % 16;

	  Block(x1,y1, x2,y2, Color[color], Color[fill]);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 319,239, Magenta);		// graphic show 7(circle)
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0; i < 50; i++)
        { x1 = rand() % 320;
          y1 = rand() % 240;
	  r = rand() % 120;
          color = rand() % 16;

	  Circle(x1,y1, r, Color[color]);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 319,239, Magenta);		// graphic show 8(gradation)
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0; i < 30; i++)
        { x1 = rand() % 320;
          y1 = rand() % 240;
          r = rand() & 0x007F;
          x2 = x1 + r;
	  y2 = y1 + r;
          color = (rand() % 32) << 11;
 	  color |= (rand() % 64) << 5;
	  color |= (rand() % 32);

	  Gradation(x1,y1, x2,y2, color);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Gradation(U16 x1,U16 y1, U16 x2,U16 y2, U16 color) /* gradation for rectangle */
{
  unsigned short i, limit, red, green, blue;

  red   = (color >> 11) & 0x001F;
  green = (color >> 5) & 0x003F;
  blue  = color & 0x001F;

  limit = abs(x1-x2) / 2;

  for(i = 0; i < limit; i++)
  { Rectangle(x1++,y1++, x2--,y2--, color);

    red   += 1;
    green += 2;
    blue  += 1;

    color  = (red % 32) >> 11;
    color |= (green % 64) << 5;
    color |= (blue % 32);
  }
}
