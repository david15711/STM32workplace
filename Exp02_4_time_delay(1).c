/* ============================================================================ */
/*		Exp02_4_time_delay(1).c : 시간지연 루틴 테스트 프로그램		*/
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
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp02_4.c    ");

  TFT_string(0,8,White,Magenta,"   시간지연 함수  정확도 테스트 (TP3)   ");
  TFT_string(18,19,Yellow,Black,"(5초)");

  while(1)
    { TFT_string( 0,16,Cyan,Black,"             H 펄스 = 10 us             ");
      Beep();
      for(i = 0; i < 250000; i++)
        { LED_on();				// LED1 on
          Delay_us(10);
	  LED_off();				// LED1 off
	  Delay_us(10);
        }

      TFT_string( 0,16,Cyan,Black,"             H 펄스 = 100 us            ");
      Beep();
      for(i = 0; i < 25000; i++)
        { LED_on();				// LED1 on
          Delay_us(100);
	  LED_off();				// LED1 off
	  Delay_us(100);
        }

      TFT_string( 0,16,Cyan,Black,"             H 펄스 = 1 ms              ");
      Beep();
      for(i = 0; i < 2500; i++)
        { LED_on();				// LED1 on
          Delay_ms(1);
	  LED_off();				// LED1 off
	  Delay_ms(1);
        }

      TFT_string( 0,16,Cyan,Black,"             H 펄스 = 10 ms             ");
      Beep();
      for(i = 0; i < 250; i++)
        { LED_on();				// LED1 on
          Delay_ms(10);
	  LED_off();				// LED1 off
	  Delay_ms(10);
        }

      TFT_string( 0,16,Cyan,Black,"             H 펄스 = 100 ms            ");
      Beep();
      for(i = 0; i < 25; i++)
        { LED_on();				// LED1 on
          Delay_ms(100);
	  LED_off();				// LED1 off
	  Delay_ms(100);
        }
    }
}
