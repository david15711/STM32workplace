/* ============================================================================ */
/*	Exp06_2_large.c : TFT-LCD에 여러가지 큰 글씨를 출력하는 프로그램	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"
#include "OK-STM767_xlarge.h"

int main(void)
{
  unsigned char i;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp06_2.c    ");

  while(1)
    { TFT_clear_screen(); 			// clear screen
      TFT_landscape_mode();			// landscape mode
      Beep();

      TFT_string(0, 0,Green,  Black,"****************************************");
      TFT_string(0, 2,Magenta,Black,"      OK-STM767 트레이닝 키트 V1.0      ");
      TFT_string(0, 4,Cyan,   Black,"        (TFT-32A 보드 V1.0 포함)        ");
      TFT_string(0, 6,Green,  Black,"****************************************");
      TFT_string(0, 9,Magenta,Black,"      (디스플레이 모드 = Landscape)     ");

      TFT_string(0,13,Cyan,   Black,"        8x16 English ASCII Normal       ");
      TFT_string(0,15,Yellow, Black,"       16x16 한글 보통 글씨 명조체      ");

      TFT_string_large(1,18, Cyan,  Black, "12x24 English ASCII Large");
      TFT_string_large(1,21, Yellow,Black, "24x24 한글 큰 글씨 명조체");

      TFT_string(0,26,White,  Blue, "     윤교수의 마이크로프로세서 월드     ");
      TFT_string(0,28,Green,  Black,"    (http://cafe.naver.com/mcu32bit)    ");
      Delay_ms(5000);

      Beep();
      Kfont_type = 'G';
      TFT_string(0,15,Yellow, Black,"       16x16 한글 보통 글씨 고딕체      ");
      Kfont_type_large = 'G';
      TFT_string_large(1,21, Yellow,Black, "24x24 한글 큰 글씨 고딕체");
      Kfont_type = 'M';
      Kfont_type_large = 'M';
      Delay_ms(3000);

      Beep();
      Kfont_type = 'P';
      TFT_string(0,15,Yellow, Black,"       16x16 한글 보통 글씨 필기체      ");
      Kfont_type_large = 'P';
      TFT_string_large(1,21, Yellow,Black, "24x24 한글 큰 글씨 필기체");
      Kfont_type = 'M';
      Kfont_type_large = 'M';
      Delay_ms(3000);

      Beep();
      TFT_string(0, 9,Cyan,Black,"       16x16 한글 보통 글씨 명조체      ");
      Kfont_type = 'G';
      TFT_string(0,11,Cyan,Black,"       16x16 한글 보통 글씨 고딕체      ");
      Kfont_type = 'P';
      TFT_string(0,13,Cyan,Black,"       16x16 한글 보통 글씨 필기체      ");
      Kfont_type = 'M';
      TFT_string_large(1,15, Yellow,Black, "24x24 한글 큰 글씨 명조체");
      Kfont_type_large = 'G';
      TFT_string_large(1,18, Yellow,Black, "24x24 한글 큰 글씨 고딕체");
      Kfont_type_large = 'P';
      TFT_string_large(1,21, Yellow,Black, "24x24 한글 큰 글씨 필기체");
      Kfont_type_large = 'M';
      Delay_ms(3000);

      TFT_string(0, 9,Cyan,Black,"                                        ");
      TFT_string(0,11,Cyan,Black,"                                        ");
      TFT_string(0,13,Cyan,Black,"                                        ");
      TFT_string_large(1,15, Yellow,Black, "                         ");
      TFT_string_large(1,18, Yellow,Black, "                         ");
      TFT_string_large(1,21, Yellow,Black, "                         ");
      Beep();
      TFT_string( 8, 8,Magenta,Black,"32x48 아주 큰 글씨 숫자");
      TFT_xy(0,10);
      TFT_color(Cyan,Black);
      for(i = 0; i < 10; i++)
        TFT_English_vlarge(i + '0');
      TFT_string( 8,17,Magenta,Black,"40x56 가장 큰 글씨 숫자");
      TFT_xy(0,19);
      TFT_color(Yellow,Black);
      for(i = 0; i < 8; i++)
        TFT_English_xlarge(i + '0');
      Delay_ms(3000);

      TFT_clear_screen(); 			// clear screen
      TFT_portrait_mode();			// portrait mode
      Beep();

      TFT_string(0, 0,Green,  Black,"******************************");
      TFT_string(0, 2,Magenta,Black," OK-STM767 트레이닝 키트 V1.0 ");
      TFT_string(0, 4,Cyan,   Black,"   (TFT-32A 보드 V1.0 포함)   ");
      TFT_string(0, 6,Green,  Black,"******************************");
      TFT_string(0,10,Magenta,Black," (디스플레이 모드 = Portrait) ");

      TFT_string(0,16,Cyan,   Black,"     8x16 English Normal      ");
      TFT_string(0,19,Yellow, Black,"    16x16 한글 보통 명조체    ");

      TFT_string_large(1,24, Cyan,  Black, "12x24 English Large");
      TFT_string_large(1,28, Yellow,Black, "24x24 한글 큰 명조");

      TFT_string(0,36,White,  Blue, "윤교수의 마이크로프로세서 월드");
      TFT_string(0,38,Green,  Black,"http://cafe.naver.com/mcu32bit");
      Delay_ms(5000);

      Beep();
      Kfont_type = 'G';
      TFT_string(0,19,Yellow, Black,"    16x16 한글 보통 고딕체    ");
      Kfont_type_large = 'G';
      TFT_string_large(1,28, Yellow,Black, "24x24 한글 큰 고딕");
      Kfont_type = 'M';
      Kfont_type_large = 'M';
      Delay_ms(3000);

      Beep();
      Kfont_type = 'P';
      TFT_string(0,19,Yellow, Black,"    16x16 한글 보통 필기체    ");
      Kfont_type_large = 'P';
      TFT_string_large(1,28, Yellow,Black, "24x24 한글 큰 필기");
      Kfont_type = 'M';
      Kfont_type_large = 'M';
      Delay_ms(3000);

      Beep();
      TFT_string(0,15,Cyan,Black,"    16x16 한글 보통 명조체    ");
      Kfont_type = 'G';
      TFT_string(0,17,Cyan,Black,"    16x16 한글 보통 고딕체    ");
      Kfont_type = 'P';
      TFT_string(0,19,Cyan,Black,"    16x16 한글 보통 필기체    ");
      Kfont_type = 'M';
      TFT_string_large(1,23, Yellow,Black, "24x24 한글 큰 명조 ");
      Kfont_type_large = 'G';
      TFT_string_large(1,26, Yellow,Black, "24x24 한글 큰 고딕 ");
      Kfont_type_large = 'P';
      TFT_string_large(1,29, Yellow,Black, "24x24 한글 큰 필기 ");
      Kfont_type_large = 'M';
      Delay_ms(5000);
    }
}
