/* ============================================================================ */
/*		Exp05_1_basic.c : TFT-LCD 영문 ASCII 출력 프로그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned int x, y;
  unsigned char code;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD modul

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp05_1.c    ");

  while(1)
    { TFT_color_screen(White);  		// display full color(white) 
      TFT_landscape_mode();			// landscape mode
      Beep();

      TFT_string( 6, 2,White, Black," OK-STM767  ASCII Character ");
      TFT_string(12, 4,White, Black,      " (White Screen) ");

      TFT_string( 0,10,White,  Blue,"Foreground White Color / Blue Background");

      TFT_string( 1,15,Red,    Cyan, "Foreground Red Color / Cyan Background");

      TFT_outline(Black);
      TFT_string( 1,20,Yellow,Transparent,"Foreground Yellow Color/Black Outline");
      TFT_outline(Transparent);

      TFT_underscore(Red);
      TFT_string( 3,25,Blue,Transparent, "Foreground Blue Color / Transparent");
      TFT_underscore(Transparent);
      Delay_ms(5000);

      TFT_color_screen(White);  		// display full color(white) 
      TFT_portrait_mode();			// portrait mode
      Beep();

      TFT_string( 1, 2,White, Black," OK-STM767  ASCII Character ");
      TFT_string( 7, 4,White, Black,      " (White Screen) ");

      TFT_string( 4,12,White,  Blue,   "Foreground White Color");
      TFT_string( 3,14,White,  Blue,  "on Background Blue Color");

      TFT_string( 5,18,Red,    Cyan,    "Foreground Red Color");
      TFT_string( 3,20,Red,    Cyan,  "on Background Cyan Color");

      TFT_outline(Black);
      TFT_string( 3,24,Yellow,Transparent, "Foreground Yellow Color");
      TFT_string( 6,26,Yellow,Transparent,    "with Black Outline");
      TFT_outline(Transparent);

      TFT_underscore(Red);
      TFT_string( 4,30,Blue,Transparent,   "Foreground Blue Color");
      TFT_string( 2,32,Blue,Transparent, "on Transparent Background");
      TFT_underscore(Transparent);
      Delay_ms(5000);

      TFT_clear_screen();  			// clear screen 
      TFT_landscape_mode();			// landscape mode
      Beep();

      TFT_string(11, 5,White,Magenta," ASCII Code Table ");
      code = 0x00;
      for(y = 0; y <= 7; y++)
        { TFT_xy(1,8+y*2); TFT_color(Green,Black);
	  TFT_0x_hexadecimal(code,2);
          TFT_English(' '); TFT_English(':');
	  TFT_color(Cyan,Black);

          for(x = 0; x <= 15; x++)
            { TFT_English(' '); 
	      TFT_English(code++);
            }
        }
      Delay_ms(5000);

      TFT_clear_screen();  			// clear screen 
      TFT_portrait_mode();			// portrait mode
      Beep();

      TFT_string( 6, 2,White,Magenta," ASCII Code Table ");
      code = 0x00;
      for(y = 0; y <= 15; y++)
        { TFT_xy(4,6+y*2); TFT_color(Green,Black);
	  TFT_0x_hexadecimal(code,2);
          TFT_English(' '); TFT_English(':');
	  TFT_color(Cyan,Black);

          for(x = 0; x <= 7; x++)
            { TFT_English(' '); 
	      TFT_English(code++);
            }
        }
      Delay_ms(5000);
    }
}
