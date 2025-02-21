/* ============================================================================ */
/*	Exp08_1_touch_screen.c : 터치 스크린 입력 프로그램(풍경화 모드)		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  Initialize_touch_screen();			// initialize touch screen

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp08_1.c    ");
  Beep();

  Rectangle(0,0, 319,239, Green);		// display title
  Rectangle(1,1, 318,238, Green);

  TFT_string( 8,5,White,Magenta, " 터치스크린 좌표 테스트 ");

  TFT_string( 1, 1, Cyan,   Black, "(0,0)");
  TFT_string(32, 1, Cyan,   Black, "(319,0)");
  TFT_string( 1,27, Cyan,   Black, "(0,239)");
  TFT_string(30,27, Cyan,   Black, "(319,239)");

  TFT_string( 9, 9, Cyan,   Black, " 원시 데이터");
  TFT_string(21, 9, Magenta,Black, " x = 0000 ");
  TFT_string(21,11, Magenta,Black, " y = 0000 ");
  TFT_string( 9,15, Cyan,   Black, " 픽셀 데이터");
  TFT_string(21,15, Magenta,Black, " x =  000 ");
  TFT_string(21,17, Magenta,Black, " y =  000 ");
  TFT_string( 8,22, White,   Blue, " 비프음 ");
  TFT_string(20,22, White,    Red, " LED ON/OFF ");
  Rectangle(61,173, 130,194, White);
  Rectangle(157,173, 258,194, White);

  while(1)
    { Touch_screen_input();			// input touch screen

      TFT_color(Cyan,Black);
      TFT_xy(26,9);				// display 12-bit original X data
      TFT_unsigned_decimal(x_12bit,0,4);
      TFT_xy(26,11);				// display 12-bit original Y data
      TFT_unsigned_decimal(y_12bit,0,4);
      TFT_xy(27,15);				// display pixel X data
      TFT_unsigned_decimal(x_touch,0,3);
      TFT_xy(27,17);				// display pixel Y data
      TFT_unsigned_decimal(y_touch,0,3);

      if((x_touch >= 61) && (x_touch <= 130) && (y_touch >= 173) && (y_touch <= 194))
        { Beep();				// beep
          Delay_ms(200);
        }
      else if((x_touch >= 157) && (x_touch <= 258) && (y_touch >= 173) && (y_touch <= 194))
        { Beep();
          LED_toggle();				// blink LED1
	  Delay_ms(200);
        }
    }
}
