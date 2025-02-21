/* ============================================================================ */
/*		Exp01_5_TFT-LCD.c : 기본 TFT-LCD 출력 프로그램			*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

void TFT_8color_horizontal(void);		/* display 8-color horizontal */

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp01_5.c    ");

  while(1)
    { TFT_clear_screen(); 			// clear screen
      TFT_landscape_mode();			// landscape mode

      TFT_string( 0, 0,Green,  Black,"****************************************");
      TFT_string( 0, 2,Magenta,Black,"  Cortex-M7 시리즈 OK-STM767 키트 V1.0  ");
      TFT_string( 0, 4,Cyan,   Black,"        (TFT-32A 보드 V1.0 포함)        ");
      TFT_string( 0, 6,Green,  Black,"****************************************");
      TFT_string( 0, 8,Black,  Black,"                                        ");
      TFT_string( 0,10,Magenta,Black,"      (디스플레이 모드 = Landscape)     ");
      TFT_string( 0,13,Cyan,   Black,"    3.2인치, 320x240 픽셀, 65536 컬러   ");
      TFT_string( 0,15,White,  Black,"       컨트롤러/드라이버 HX8347-A       ");
      TFT_string( 5,18,Green,  Black,"16x16 한글");
      TFT_string(24,18,Cyan,   Black,"8x16 영문");
      TFT_string_large( 2,21,Green,  Black,"24x24 한글");
      TFT_string_large(14,21,Cyan,   Black,"12x24 영문");
      TFT_string( 0,24,Black,  Black,"                                        ");
      TFT_string( 0,26,White,  Blue, "     윤교수의 마이크로프로세서 월드     ");
      TFT_string( 0,28,Green,  Black,"    (http://cafe.naver.com/mcu32bit)    ");
      Beep();
      Delay_ms(5000);

      TFT_clear_screen(); 			// clear screen
      TFT_portrait_mode();			// portrait mode

      TFT_string( 0, 0,Green,  Black,"******************************");
      TFT_string( 0, 2,Magenta,Black,"     OK-STM767 키트  V1.0     ");
      TFT_string( 0, 4,Cyan,   Black,"   (TFT-32A 보드 V1.0 포함)   ");
      TFT_string( 0, 6,Green,  Black,"******************************");
      TFT_string( 0, 8,Black,  Black,"                              ");
      TFT_string( 0,10,Magenta,Black," (디스플레이 모드 = Portrait) ");
      TFT_string( 0,14,Cyan,   Black,"    3.2인치, 240x320 픽셀     ");
      TFT_string( 0,16,Yellow, Black,"          65536 컬러          ");
      TFT_string( 0,20,Red,    Black,"    16비트 CPU 인터페이스     ");
      TFT_string( 0,23,White,  Black,"  컨트롤러/드라이버 HX8347-A  ");
      TFT_string( 2,28,Green,  Black,"16x16 한글");
      TFT_string(18,28,Cyan,   Black,"8x16 영문");
      TFT_string_large( 0,31,Green,  Black,"24x24");
      TFT_string_large( 5,31,Magenta,Black,"한글");
      TFT_string_large(11,31,Cyan,   Black,"12x24");
      TFT_string_large(16,31,Red,    Black,"영문");
      TFT_string( 0,36,White,  Blue, "윤교수의 마이크로프로세서 월드");
      TFT_string( 0,38,Green,  Black,"http://cafe.naver.com/mcu32bit");
      Beep();
      Delay_ms(5000);

      TFT_clear_screen(); 			// clear screen
      TFT_landscape_mode();			// landscape mode

      TFT_8color_horizontal();			// display 8-Color(horizontal)
      TFT_string(18, 1,Black,Transparent,"백색");
      TFT_string(18, 5,White,Transparent,"흑색");
      TFT_string(18, 8,Green,Transparent,"적색");
      TFT_string(18,12,Red,  Transparent,"녹색");
      TFT_string(18,16,Cyan, Transparent,"청색");
      TFT_string(18,20,Black,Transparent,"노랑");
      TFT_string(18,24,Red,  Transparent,"녹청");
      TFT_string(18,27,White,Transparent,"자홍");
      Beep();
      Delay_ms(5000);
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

unsigned int Color[8] =				// 8-color data
   { White, Black, Red, Green, Blue, Yellow, Cyan, Magenta };

void TFT_8color_horizontal(void)		/* display 8-color horizontal */
{
  unsigned int i, j, k;

  TFT_GRAM_address(0,0);

  for(i = 0; i < 8; i++)
    for(j = 0; j < 30; j++)
      for(k = 0; k < 320; k++)
        TFT_data(Color[i]);
}
