/* ============================================================================ */
/*	Exp06_3_transparent.c : TFT-LCD 투명 글씨 및 외곽선체 출력 프로그램	*/
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
  LCD_string(0xC0,"   Exp06_3.c    ");

  while(1)
    { TFT_color_screen(White);
      TFT_string(1, 2, White,Magenta," 투명 글씨 및 외곽선체 사용 예(16x16) ");
      TFT_string(7, 7, White,Blue,   " (1) 외곽선이 없을 경우 ");
      TFT_string(3,10, Yellow,Transparent,"한글, English, ABCDEF, 0123456789");
      TFT_string(6,13, Yellow,Transparent,"글씨가 잘 보이지 않습니다.");

      TFT_string(7,18, White,Blue,   " (2) 외곽선이 있을 경우 ");
      TFT_outline(Blue);
      TFT_string(3,22, Yellow,Transparent,"한글, English, ABCDEF, 0123456789");
      TFT_string(7,25, Yellow,Transparent,"이제 글씨가 잘 보입니다.");
      TFT_outline(Transparent);
      Beep();
      Delay_ms(5000);

      TFT_color_screen(Yellow);
      TFT_string(1, 2, White,Magenta," 투명 글씨 및 외곽선체 사용 예(16x16) ");
      TFT_string(7, 7, White,Blue,   " (1) 외곽선이 없을 경우 ");
      TFT_string(3,10, White,Transparent,"한글, English, ABCDEF, 0123456789");
      TFT_string(6,13, White,Transparent,"글씨가 잘 보이지 않습니다.");

      TFT_string(7,18, White,Blue,   " (2) 외곽선이 있을 경우 ");
      TFT_outline(Blue);
      TFT_string(3,22, White,Transparent,"한글, English, ABCDEF, 0123456789");
      TFT_string(7,25, White,Transparent,"이제 글씨가 잘 보입니다.");
      TFT_outline(Transparent);
      Beep();
      Delay_ms(5000);

      TFT_color_screen(White);
      TFT_string(1, 2, White,Magenta," 투명 글씨 및 외곽선체 사용 예(24x24) ");
      TFT_string(7, 7, White,Blue,   " (1) 외곽선이 없을 경우 ");
      TFT_string_large(3,10, Yellow,Transparent,"한글, English, 01234");
      TFT_string_large(0,13, Yellow,Transparent,"글씨가 잘 보이지 않습니다.");

      TFT_string(7,18, White,Blue,   " (2) 외곽선이 있을 경우 ");
      TFT_outline(Blue);
      TFT_string_large(3,22, Yellow,Transparent,"한글, English, 01234");
      TFT_string_large(1,25, Yellow,Transparent,"이제 글씨가 잘 보입니다.");
      TFT_outline(Transparent);
      Beep();
      Delay_ms(5000);

      TFT_color_screen(Yellow);
      TFT_string(1, 2, White,Magenta," 투명 글씨 및 외곽선체 사용 예(24x24) ");
      TFT_string(7, 7, White,Blue,   " (1) 외곽선이 없을 경우 ");
      TFT_string_large(3,10, White,Transparent,"한글, English, 01234");
      TFT_string_large(0,13, White,Transparent,"글씨가 잘 보이지 않습니다.");

      TFT_string(7,18, White,Blue,   " (2) 외곽선이 있을 경우 ");
      TFT_outline(Blue);
      TFT_string_large(3,22, White,Transparent,"한글, English, 01234");
      TFT_string_large(1,25, White,Transparent,"이제 글씨가 잘 보입니다.");
      TFT_outline(Transparent);
      Beep();
      Delay_ms(5000);
    }
}
