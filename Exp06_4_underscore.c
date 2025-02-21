/* ============================================================================ */
/*	Exp06_4_underscore.c : TFT-LCD 밑줄 및 외곽선체 출력 프로그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp06_4.c    ");

  while(1)
    { TFT_clear_screen();
      Beep();

      TFT_landscape_mode();			// landscape mode for normal font
      TFT_string(6, 7, Green,Black,"한글/English 12345 정상체");
      TFT_underscore(Yellow);
      TFT_string(6,10, Cyan, Black,"한글/English 12345 밑줄");
      TFT_underscore(Transparent);
      TFT_outline(Red);
      TFT_string(6,13, White,Black,"한글/English 12345 외곽선체");
      TFT_outline(Transparent);

      TFT_landscape_mode_large();		// landscape mode for large font
      TFT_string_large(2, 1, White,Magenta," 밑줄/외곽선체 사용 예 ");
      TFT_string_large(1,18, Green,Black,"한글/English 12345 정상체");
      TFT_underscore(Yellow);
      TFT_string_large(1,22, Cyan, Black,"한글/English 12345 밑줄");
      TFT_underscore(Transparent);
      TFT_outline(Red);
      TFT_string_large(1,26, White,Black,"한글/English 12345 외곽선");
      TFT_outline(Transparent);
      Delay_ms(5000);

      TFT_clear_screen();
      Beep();

      TFT_portrait_mode();			// portrait mode for normal font
      TFT_string(2,11, Green,Black,"한글/English 12345 정상체");
      TFT_underscore(Yellow);
      TFT_string(2,14, Cyan, Black,"한글/English 12345 밑줄");
      TFT_underscore(Transparent);
      TFT_outline(Red);
      TFT_string(2,17, White,Black,"한글/English 12345 외곽선체");
      TFT_outline(Transparent);

      TFT_portrait_mode_large();		// portrait mode for large font
      TFT_string_large(0, 3, White,Magenta," 밑줄/외곽선체 사용 ");
      TFT_string_large(1,24, Green,Black,"한글/English 12345");
      TFT_underscore(Yellow);
      TFT_string_large(1,29, Cyan, Black,"한글/English 12345");
      TFT_underscore(Transparent);
      TFT_outline(Red);
      TFT_string_large(1,34, White,Black,"한글/English 12345");
      TFT_outline(Transparent);
      Delay_ms(5000);
    }
}
