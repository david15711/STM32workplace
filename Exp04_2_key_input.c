/* ============================================================================ */
/*			Exp04_2_key_input.c : 키입력 함수			*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

int main(void)
{
  unsigned char key;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp04_2.c    ");
  Beep();

  TFT_string(6, 9,White,Magenta," 키 KEY1~KEY4를 누르시오 ! ");

  while(1)
    { key = Key_input();                       // key input
      switch(key)
        { case KEY1 : LED_on();
                      TFT_string_large(7,18, Cyan,Black, "KEY1 is OK !");
		      Delay_ms(100);
		      LED_off();
                      break;
          case KEY2 : LED_on();
                      TFT_string_large(7,18, Cyan,Black, "KEY2 is OK !");
		      Delay_ms(100);
		      LED_off();
                      break;
          case KEY3 : LED_on();
                      TFT_string_large(7,18, Cyan,Black, "KEY3 is OK !");
		      Delay_ms(100);
		      LED_off();
                      break;
          case KEY4 : LED_on();
                      TFT_string_large(7,18, Cyan,Black, "KEY4 is OK !");
		      Delay_ms(100);
		      LED_off();
                      break;
          default:    break;
        }
    }
}
