/* ============================================================================ */
/*		Exp07_1_function.c : 수학함수를 이용한 그래픽			*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned short i, x, y;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp07_1.c    ");

  while(1)
    { Beep();

      Rectangle(0,0, 319,239, Magenta);		// graphic show of line
      Rectangle(1,1, 318,238, Magenta);
      for(x = 10; x <= 310; x += 10)
        { Line(x,0, x,239, Green);
          Delay_ms(20);
        }
      for(y = 10; y <= 230; y += 10)
        { Line(0,y, 319,y, Green);
          Delay_ms(20);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 319,239, Magenta);		// graphic show of ramp
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0,x = 20,y = 12; i <= 34; i++,x += 20,y += 12)
        { Line(x,0, 0,y, Red);
          Line(0,239-y, x,239, Red);
          Delay_ms(20);
        }
      for(i = 0,x = 700,y = 420; i <= 34; i++,x -= 20,y -= 12)
        { Line(x,0, 0,y, Cyan);
          Line(0,239-y, x,239, Cyan);
          Delay_ms(20);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 319,239, Magenta);		// graphic show of rectangle
      Rectangle(1,1, 318,238, Magenta);
      for(i = 0,x = 152,y = 114; i <= 18; i++,x -= 8,y -= 6)
        { Rectangle(x,y, 319-x,239-y, Cyan);
          Delay_ms(20);
        }
      Delay_ms(1000);
      for(i = 0,x = 8,y = 6; i <= 18; i++,x += 8,y += 6)
        { Rectangle(x,y, 319-x,239-y, Red);
          Delay_ms(20);
        }
      for(i = 0,x = 152,y = 114; i <= 18; i++,x -= 8,y -= 6)
        { Rectangle(x,y, 319-x,239-y, Green);
          Delay_ms(20);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 319,239, Magenta);		// graphic show of circle
      Rectangle(1,1, 318,238, Magenta);
      for(i = 10; i <= 160; i += 10)
        { Circle(160,120,i, Green);
          Delay_ms(20);
        }
      Delay_ms(1000);
      for(i = 160; i >= 10; i -= 10)
        { Circle(160,120,i, Red);
          Delay_ms(20);
        }
      for(i = 10; i <= 160; i += 10)
        { Circle(160,120,i, White);
          Delay_ms(20);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 319,239, Magenta);		// graphic show of 2 circles
      Rectangle(1,1, 318,238, Magenta);
      for(i = 10; i <= 120; i += 10)
        { Circle(80,120,i, Green);
          Circle(240,120,i, Green);
          Delay_ms(20);
        }
      Delay_ms(1000);
      for(i = 120; i >= 10; i -= 10)
        { Circle(80,120,i, Red);
          Circle(240,120,i, Red);
          Delay_ms(20);
        }
      for(i = 10; i <= 120; i += 10)
        { Circle(80,120,i, Yellow);
          Circle(240,120,i, Yellow);
          Delay_ms(20);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 319,239, Magenta);		// graphic show of sine curve
      Rectangle(1,1, 318,238, Magenta);
      Line(0,120, 319,120, Magenta);
      for(i = 10; i <= 120; i += 10)
        { Sine(i, 0, Green);
          Delay_ms(20);
        }
      Delay_ms(1000);
      for(i = 10; i <= 120; i += 10)
        { Sine(i, 1, Cyan);
          Delay_ms(20);
        }
      Delay_ms(3000);
      TFT_clear_screen();
    }
}
