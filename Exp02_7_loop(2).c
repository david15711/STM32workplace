/* ============================================================================ */
/*		Exp02_7_loop(2).c : ���ѷ��� ó�� ���α׷�(2)			*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f746xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned int i;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp02_7.c    ");
  TFT_string(0,10,White,Magenta,"       ���ѷ��� ó�� ���α׷�(2)        ");
  TFT_string(0,15,White,  Black,"   ���ѷ����� �۾��� �ݺ��Ͽ� ������.   ");

  while(1)					// ***** ���� ������ ���� �κ� *****
    { for(i = 0; i < 5; i++)
        { LED_on();				// LED1 on
          Delay_ms(500);
	  LED_off();				// LED1 off
	  Delay_ms(500);
        }
      Beep();					// beep
    }						// ***** ���� ������ �� �κ� *****
}
