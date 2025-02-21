/* ============================================================================ */
/*   Exp04_4_key_count(2).c : 키입력 카운트(디바운싱 및 원터치 기능을 사용함)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

int main(void)
{
  unsigned char key;
  unsigned int KEY1_count, KEY2_count, KEY3_count, KEY4_count;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp04_4.c    ");
  Beep();

  TFT_string(1,6, White,Magenta, " 키입력 카운트 프로그램 (디바운싱 O) ");
  TFT_string_large(4,10, White,Black, "KEY1 입력 = 000000");
  TFT_string_large(4,14, White,Black, "KEY2 입력 = 000000");
  TFT_string_large(4,18, White,Black, "KEY3 입력 = 000000");
  TFT_string_large(4,22, White,Black, "KEY4 입력 = 000000");

  KEY1_count = 0;				// initialize variables
  KEY2_count = 0;
  KEY3_count = 0;
  KEY4_count = 0;

  while(1)
    { key = Key_input();			// key input
      switch(key)
        { case KEY1 : KEY1_count++;		// increment KEY1 count
	              TFT_xy(16,10);		// display KEY1 count
  	              TFT_color(Green,Black);
	              TFT_unsigned_decimal_large(KEY1_count,1,6);
                      break;
          case KEY2 : KEY2_count++;		// increment KEY2 count
	              TFT_xy(16,14);		// display KEY2 count
  	              TFT_color(Green,Black);
	              TFT_unsigned_decimal_large(KEY2_count,1,6);
                      break;
          case KEY3 : KEY3_count++;		// increment KEY3 count
	              TFT_xy(16,18);		// display KEY3 count
  	              TFT_color(Green,Black);
	              TFT_unsigned_decimal_large(KEY3_count,1,6);
                      break;
          case KEY4 : KEY4_count++;		// increment KEY4 count
	              TFT_xy(16,22);		// display KEY4 count
  	              TFT_color(Green,Black);
	              TFT_unsigned_decimal_large(KEY4_count,1,6);
                      break;
          default:    break;
        }
    }
}
