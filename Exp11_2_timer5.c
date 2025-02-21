/* ============================================================================ */
/*		Exp11_2_timer5.c : 타이머5를 사용한 주기적인 인터럽트		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void TIM5_IRQHandler(void);		        /* TIM5 interrupt function (2Hz)*/

volatile unsigned int interrupt_count;		        // interrupt count
volatile unsigned char interrupt_flag, beep_flag;       // flag by interrupt

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void TIM5_IRQHandler(void)			/* TIM5 interrupt function (2Hz)*/
{
  TIM5->SR = 0x0000;				// clear pending bit of TIM5 interrupt
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
  LCD_string(0xC0,"   Exp11_2.c    ");
  Beep();

  TFT_string( 5,10,White,Magenta," 범용 타이머5 인터럽트 (2Hz) ");
  TFT_string(18,16,Green,Black,  "00000");

  interrupt_count = 0;				// initialize interrupt variables
  interrupt_flag = 0;
  beep_flag = 0;

  RCC->APB1ENR |= 0x00000008;			// enable TIM5 clock
  TIM5->PSC = 1079;				// 108MHz/(1079+1) = 100kHz
  TIM5->ARR = 49999;				// 100kHz/(49999+1) = 2Hz
  TIM5->CNT = 0;				// clear counter
  TIM5->DIER = 0x0001;				// enable update interrupt
  TIM5->CR1 = 0x0005;				// enable TIM5 and update event
  NVIC->ISER[1] |= 0x00040000;			// enable (50)TIM5 interrupt

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
