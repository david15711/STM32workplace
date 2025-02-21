/* ============================================================================ */
/*	Exp04_1_key_value.c : 키입력 값 표시(키입력 함수를 사용하지 않음)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

int main(void)
{
  unsigned int key;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp04_1.c    ");
  Beep();

  TFT_string(14,9,White,Magenta," 키입력 값 ");
  TFT_string_large(0,15,White,Black,"GPIOC = 0b0000000000000000");

  while(1)
    { key = GPIOC->IDR & 0x0000000F;		// read GPIOC
      TFT_xy(10,15);				// display value
      TFT_color(Cyan,Black);
      TFT_binary_large(key,16);
    }
}
