/* ============================================================================ */
/*		Exp11_1_timer1.c : 타이머1을 사용한 주기적인 인터럽트		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void TIM1_UP_TIM10_IRQHandler(void);	        /* TIM1 interrupt function (2Hz)*/

volatile unsigned int interrupt_count;		        // interrupt count
volatile unsigned char interrupt_flag, beep_flag;       // flag by interrupt

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void TIM1_UP_TIM10_IRQHandler(void)	        /* TIM1 interrupt function (2Hz)*/
{
  TIM1->SR = 0x0000;				// clear pending bit of TIM1 interrupt
  interrupt_flag = 1;				// interrupt ocurred

  interrupt_count++;				// increment interrupt count
  if((interrupt_count % 10) == 0)		// beep in 5-second interval
    beep_flag = 1;

  LED_toggle();					// toggle LED
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp11_1.c    ");
  Beep();

  TFT_string( 4,10,White,Magenta," 고성능 타이머1 인터럽트 (2Hz) ");
  TFT_string(18,16,Green,Black,  "00000");

  interrupt_count = 0;				// initialize interrupt variables
  interrupt_flag = 0;
  beep_flag = 0;

  RCC->APB2ENR |= 0x00000001;			// enable TIM1 clock
  TIM1->PSC = 1079;				// 108MHz/(1079+1) = 100kHz
  TIM1->ARR = 49999;				// 100kHz/(49999+1) = 2Hz
  TIM1->CNT = 0;				// clear counter
  TIM1->DIER = 0x0001;				// enable update interrupt
  TIM1->CR1 = 0x0005;				// enable TIM1 and update event
  NVIC->ISER[0] |= 0x02000000;			// enable (25)TIM1 interrupt

  while(1)
    { if(interrupt_flag == 1)
        { interrupt_flag = 0;
	  TFT_xy(18,16);			// display interrupt count
          TFT_color(Green,Black);
          TFT_unsigned_decimal(interrupt_count,1,5);

          if(beep_flag == 1)			// if 5 seconds, beep
            { beep_flag = 0;
              Beep();
	    }
        }
    }
}
