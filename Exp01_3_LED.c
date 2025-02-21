/* ============================================================================ */
/*	Exp01_3_LED.c : �⺻ LED �� ���� ��� ���α׷�(216MHz SYSCLK ���)      */
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned int i;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization

  while(1)
    { for(i = 0; i < 5; i++)
        { GPIOC->BSRR = 0x00000010;		// LED on
          Delay_ms(500);
	  GPIOC->BSRR = 0x00100000;		// LED off
	  Delay_ms(500);
        }

      blinkTheLED();
    }
}
