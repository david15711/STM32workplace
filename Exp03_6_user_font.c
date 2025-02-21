/* ============================================================================ */
/*	Exp03_6_user_font.c : 텍스트 LCD 사용자 정의 문자 출력 프로그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void Set_user_font(void);                           /* set user character font */

unsigned char font[] = {
  0x10, 0x18, 0x1C, 0x1E, 0x1C, 0x18, 0x10, 0x00,   // character 0 (left arrow)
  0x0F, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0F, 0x00,   // character 1 (마)
  0x08, 0x08, 0x08, 0x0E, 0x08, 0x08, 0x08, 0x00,   // character 2
  0x01, 0x02, 0x04, 0x08, 0x00, 0x00, 0x0F, 0x00,   // character 3 (스)
  0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x1E, 0x00,   // character 4
  0x0F, 0x08, 0x08, 0x0F, 0x08, 0x08, 0x0F, 0x00,   // character 5 (터)
  0x04, 0x04, 0x04, 0x1C, 0x04, 0x04, 0x04, 0x00,   // character 6
  0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01, 0x00 }; // character 7 (right arrow)

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();                             // initialize text LCD module

  Set_user_font();                              // set user character font

  while(1)
    { Beep();
      LCD_command(0x80);                        // display logo
      LCD_data(0x00);
      LCD_data(0x00);
      LCD_string(0x82," OK--STM767 ");
      LCD_data(0x07);
      LCD_data(0x07);

      LCD_string(0xC0,"Cortex-M7 ");            // display message 1
      LCD_data(0x01);
      LCD_data(0x02);
      LCD_data(0x03);
      LCD_data(0x04);
      LCD_data(0x05);
      LCD_data(0x06);
      Delay_ms(2000);

      LCD_string(0xC0,"   2017/01/01   ");      // display message 2
      Delay_ms(2000);

      LCD_string(0xC0," DUCK-YONG YOON ");      // display message 3
      Delay_ms(2000);
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Set_user_font(void)                        /* set user character font */
{
  unsigned int i;

  LCD_command(0x40);                            // set CGRAM address
  for(i = 0; i <= 63; i++)			// download font data
     LCD_data(font[i]);
}
