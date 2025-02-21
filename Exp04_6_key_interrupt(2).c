/* ============================================================================ */
/*   Exp04_6_key_interrupt(2).c : 키입력에 의한 EXTI 인터럽트(디바운싱 사용함)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

void EXTI0_IRQHandler(void);			/* EXTI0 interrupt function */
void EXTI1_IRQHandler(void);			/* EXTI1 interrupt function */
void EXTI2_IRQHandler(void);			/* EXTI2 interrupt function */
void EXTI3_IRQHandler(void);			/* EXTI3 interrupt function */

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

volatile unsigned int interrupt_flag, KEY1_count, KEY2_count, KEY3_count, KEY4_count;

void EXTI0_IRQHandler(void)			/* EXTI0 interrupt function */
{
  KEY1_count++;

  LED_on();					// LED on
  Beep();
  LED_off();					// LED off

  while((GPIOC->IDR & 0x00000001) != 0x00000001); // debouncing
  Delay_ms(20);
  EXTI->PR = 0x00000001;			// clear pending bit of EXTI0
  NVIC->ICPR[0] = 0x00000040;			// clear pending bit of (6)EXTI0

  interrupt_flag = 1;
}

void EXTI1_IRQHandler(void)			/* EXTI1 interrupt function */
{
  KEY2_count++;

  LED_on();					// LED on
  Beep();
  LED_off();					// LED off

  while((GPIOC->IDR & 0x00000002) != 0x00000002); // debouncing
  Delay_ms(20);
  EXTI->PR = 0x00000002;			// clear pending bit of EXTI1
  NVIC->ICPR[0] = 0x00000080;			// clear pending bit of (7)EXTI1

  interrupt_flag = 1;
}

void EXTI2_IRQHandler(void)			/* EXTI2 interrupt function */
{
  KEY3_count++;

  LED_on();					// LED on
  Beep();
  LED_off();					// LED off

  while((GPIOC->IDR & 0x00000004) != 0x00000004); // debouncing
  Delay_ms(20);
  EXTI->PR = 0x00000004;			// clear pending bit of EXTI2
  NVIC->ICPR[0] = 0x00000100;			// clear pending bit of (8)EXTI2

  interrupt_flag = 1;
}

void EXTI3_IRQHandler(void)			/* EXTI3 interrupt function */
{
  KEY4_count++;

  LED_on();					// LED on
  Beep();
  LED_off();					// LED off

  while((GPIOC->IDR & 0x00000008) != 0x00000008); // debouncing
  Delay_ms(20);
  EXTI->PR = 0x00000008;			// clear pending bit of EXTI3
  NVIC->ICPR[0] = 0x00000200;			// clear pending bit of (9)EXTI3

  interrupt_flag = 1;
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp04_6.c    ");
  Beep();

  TFT_string(0,6, White,Magenta, " 키입력 인터럽트 프로그램 (디바운싱 O) ");
  TFT_string_large(4,10, White,Black, "KEY1 입력 = 000000");
  TFT_string_large(4,14, White,Black, "KEY2 입력 = 000000");
  TFT_string_large(4,18, White,Black, "KEY3 입력 = 000000");
  TFT_string_large(4,22, White,Black, "KEY4 입력 = 000000");

  GPIOC->MODER &= 0xFFFFFF00;			// PC3,2,1,0 = input mode
  RCC->APB2ENR |= 0x00000001;			// enable SYSCFG
  SYSCFG->EXTICR[0] = 0x00002222;		// EXTI3,2,1,0 = PC3,2,1,0(KEY4,3,2,1)
  EXTI->IMR = 0x0000000F;			// enable EXTI3,2,1,0 interrupt
  EXTI->RTSR = 0x00000000;			// disable EXTI3,2,1,0 rising edge
  EXTI->FTSR = 0x0000000F;			// enable EXTI3,2,1,0 falling edge
  NVIC->ISER[0] = 0x000003C0;			// enable (9)EXTI3 ~ (6)EXTI0 interrupt

  interrupt_flag = 0;				// initialize variables
  KEY1_count = 0;
  KEY2_count = 0;
  KEY3_count = 0;
  KEY4_count = 0;

  while(1)
    { if(interrupt_flag == 1)			// interrupt ?
        { interrupt_flag = 0;
  	  TFT_color(Green,Black);
	  TFT_xy(16,10);			// display KEY1 count
	  TFT_unsigned_decimal_large(KEY1_count,1,6);
	  TFT_xy(16,14);			// display KEY2 count
	  TFT_unsigned_decimal_large(KEY2_count,1,6);
	  TFT_xy(16,18);			// display KEY3 count
	  TFT_unsigned_decimal_large(KEY3_count,1,6);
	  TFT_xy(16,22);			// display KEY4 count
	  TFT_unsigned_decimal_large(KEY4_count,1,6);
        }
    }
}
