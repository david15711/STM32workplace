/* ============================================================================ */
/*    Exp01_4_Text-LCD.c : 기본 텍스트 LCD 출력 프로그램(216MHz SYSCLK 사용)    */
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned int i;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title

  while(1)
    { LCD_string(0xC0," 200ms interval ");
      //Beep();
      for(i = 0; i < 12; i++)
        { GPIOC->BSRR = 0x00000010;		// LED on
          Delay_ms(200);
	  GPIOC->BSRR = 0x00100000;		// LED off
	  Delay_ms(200);
        }

      LCD_string(0xC0," 400ms interval ");
      //Beep();
      for(i = 0; i < 6; i++)
        { GPIOC->BSRR = 0x00000010;		// LED on
          Delay_ms(400);
	  GPIOC->BSRR = 0x00100000;		// LED off
	  Delay_ms(400);
        }

      LCD_string(0xC0," 600ms interval ");
      //Beep();
      for(i = 0; i < 4; i++)
        { GPIOC->BSRR = 0x00000010;		// LED on
          Delay_ms(600);
	  GPIOC->BSRR = 0x00100000;		// LED off
	  Delay_ms(600);
        }
    }
}
