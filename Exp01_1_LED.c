/* ============================================================================ */
/*	Exp01_1_LED.c : 기본 LED 및 버저 출력 프로그램(16MHz HSI 사용)          */
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"

void Delay_us(unsigned int time_us);		/* time delay for us in 16MHz */
void Delay_ms(unsigned int time_ms);		/* time delay for ms in 16MHz */
void Beep(void);				/* beep for 50 ms */

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned int i;

  RCC->AHB1ENR |= 0x00000004;			// port C clock enable(GPIOCEN = 1)

  GPIOC->MODER &= 0x3FFFFCFF;			// set PC15(buzzer) and PC4(LED) to output
  GPIOC->MODER |= 0x40000100;
  GPIOC->ODR &= 0xFFFF7FEF;			// PC15(buzzer) = PC4(LED) = 0

  while(1)
    { for(i = 0; i < 5; i++)
        { GPIOC->BSRR = 0x00000010;		// LED on
          Delay_ms(500);
	  GPIOC->BSRR = 0x00100000;		// LED off
	  Delay_ms(500);
          GPIOC->BSRR = 0x00000010;		// LED on
          Delay_ms(50);
	  GPIOC->BSRR = 0x00100000;		// LED off
	  Delay_ms(50);
          GPIOC->BSRR = 0x00000010;		// LED on
          Delay_ms(50);
	  GPIOC->BSRR = 0x00100000;		// LED off
	  Delay_ms(50);
        }

      //Beep();					// beep
    }
}

/* ----- 사용자 함수 ---------------------------------------------------------- */

void SystemInit(void)				/* dummy system function */
{
  asm volatile("NOP");
}

void Delay_us(unsigned int time_us)		/* time delay for us in 16MHz */
{
  register unsigned int i;

  for(i = 0; i < time_us; i++)			// 3 cycle
    { asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");
      asm volatile("NOP");			// 13 cycle
    }
}

void Delay_ms(unsigned int time_ms)		/* time delay for ms in 16MHz */
{
  register unsigned int i;

  for(i = 0; i < time_ms; i++)
    Delay_us(1000);
}

void Beep(void)					/* beep for 50 ms */
{ 
  GPIOC->BSRR = 0x00008000;   			// buzzer on
  Delay_ms(50);					// delay 50 ms
  GPIOC->BSRR = 0x80000000;			// buzzer off
}
