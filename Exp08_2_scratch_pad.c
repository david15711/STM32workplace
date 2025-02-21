/* ============================================================================ */
/*	Exp08_2_scratch_pad.c : 터치스크린을 이용한 낙서장 프로그그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned int xo = 0, yo = 0;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp08_2.c    ");
  Beep();

  Initialize_touch_screen();			// initialize touch screen

  Rectangle(0,0, 319,239, Green);               //draw screen outline
  Rectangle(1,1, 318,238, Green);
  TFT_string(5,1, White,Magenta, " 터치 스크린을 이용한 낙서장 ");

  while(1)
    { Touch_screen_input();			// input touch screen
    
      if((x_touch == 0) && (y_touch == 0))	// if not touch, skip
        { xo = 0;
          yo = 0;
        }

      if((xo == 0) && (yo == 0))		// if first touch, skip
        { xo = x_touch;
          yo = y_touch;
        }
      else
        { Line(xo,yo, x_touch,y_touch, White);
          xo = x_touch;
          yo = y_touch;
        }
    }
}
