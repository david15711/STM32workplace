/* ============================================================================ */
/*	Exp06_1_basic.c : TFT-LCD에서 기본적인 한글 출력 프로그램		*/
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

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp06_1.c    ");

  while(1)
    { TFT_clear_screen();			// clear screen
      TFT_landscape_mode();			// landscape mode
      Beep();

      TFT_string( 0, 0,Green,  Black,"****************************************");
      TFT_string( 0, 2,Magenta,Black,"  Cortex-M7 시리즈 OK-STM767 키트 V1.0  ");
      TFT_string( 0, 4,Cyan,   Black,"        (TFT-32A 보드 V1.0 포함)        ");
      TFT_string( 0, 6,Green,  Black,"****************************************");
      TFT_string( 0, 8,Black,  Black,"                                        ");
      TFT_string( 0,10,Magenta,Black,"      (디스플레이 모드 = Landscape)     ");
      TFT_string( 0,14,Cyan,   Black,"    3.2인치, 320x240 픽셀, 65536 컬러   ");
      TFT_string( 0,16,White,  Black," 컨트롤러/드라이버 HX8347-A(대만 Himax) ");
      TFT_string( 0,20,Red,    Black," 빨강 Red    ");
      TFT_string(13,20,Green,  Black," 녹색 Green  ");
      TFT_string(26,20,Blue,   Black," 파랑 Blue    ");
      TFT_string( 0,22,Yellow, Black," 노랑 Yellow ");
      TFT_string(13,22,Cyan,   Black," 녹청 Cyan   ");
      TFT_string(26,22,Magenta,Black," 자홍 Magenta ");
      TFT_string( 0,24,Black,  Black,"                                        ");
      TFT_string( 0,26,White,  Blue, "     윤교수의 마이크로프로세서 월드     ");
      TFT_string( 0,28,Green,  Black,"    (http://cafe.naver.com/mcu32bit)    ");
      Delay_ms(5000);

      TFT_clear_screen(); 			// clear screen
      TFT_portrait_mode();			// portrait mode
      Beep();

      TFT_string( 0, 0,Green,  Black,"******************************");
      TFT_string( 0, 2,Magenta,Black,"     OK-STM767 키트  V1.0     ");
      TFT_string( 0, 4,Cyan,   Black,"   (TFT-32A 보드 V1.0 포함)   ");
      TFT_string( 0, 6,Green,  Black,"******************************");
      TFT_string( 0, 8,Black,  Black,"                              ");
      TFT_string( 0,10,Magenta,Black," (디스플레이 모드 = Portrait) ");
      TFT_string( 0,14,Cyan,   Black,"    3.2인치, 240x320 픽셀     ");
      TFT_string( 0,16,Yellow, Black,"          65536 컬러          ");
      TFT_string( 0,20,Red,    Black,"    16비트 CPU 인터페이스     ");
      TFT_string( 0,24,White,  Black,"  컨트롤러/드라이버 HX8347-A  ");
      TFT_string( 0,26,White,  Black,"         (대만 Himax)         ");
      TFT_string( 0,30,Red,    Black," 빨강 Red ");
      TFT_string(10,30,Green,  Black," 녹색 Gre ");
      TFT_string(20,30,Blue,   Black," 파랑 Blu ");
      TFT_string( 0,32,Yellow, Black," 노랑 Yel ");
      TFT_string(10,32,Cyan,   Black," 녹청 Cya ");
      TFT_string(20,32,Magenta,Black," 자홍 Mag ");
      TFT_string( 0,36,White,  Blue, "윤교수의 마이크로프로세서 월드");
      TFT_string( 0,38,Green,  Black,"http://cafe.naver.com/mcu32bit");
      Delay_ms(5000);
    }
}
