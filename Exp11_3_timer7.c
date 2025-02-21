/* ============================================================================ */
/*		Exp11_3_timer7.c : 타이머7을 사용한 가변 주기 인터럽트		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void TIM7_IRQHandler(void);		        /* TIM7 interrupt function */
void Display_key(U08 key);			/* display key input */

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void TIM7_IRQHandler(void)			/* TIM7 interrupt function */
{
  TIM7->SR = 0x0000;				// clear pending bit of TIM7 interrupt

  LED_toggle();					// toggle LED
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned char key;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp11_3.c    ");
  Beep();

  TFT_string( 2, 4, White,Magenta, " 타이머7 인터럽트를 이용한 LED 점멸 ");
  TFT_string(10,10, Green,  Black, " 50ms ON /  50ms OFF");
  TFT_string( 7,16, White,  Black, "KEY1 -  50ms 주기 인터럽트");
  TFT_string( 7,19, White,  Black, "KEY2 - 100ms 주기 인터럽트");
  TFT_string( 7,22, White,  Black, "KEY3 - 200ms 주기 인터럽트");
  TFT_string( 7,25, White,  Black, "KEY4 - 400ms 주기 인터럽트");

  RCC->APB1ENR |= 0x00000020;			// enable TIM7 clock
  TIM7->PSC = 1079;				// 108MHz/(1079+1) = 100kHz
  TIM7->ARR = 4999;				// 100kHz/(4999+1) = 20Hz
  TIM7->CNT = 0;				// clear counter
  TIM7->DIER = 0x0001;				// enable update interrupt
  TIM7->CR1 = 0x0085;				// enable TIM7, update event, and preload
  NVIC->ISER[1] |= 0x00800000;			// enable (55)TIM7 interrupt

  while(1)
    { key = Key_input();			// key input
      switch(key)
        { case KEY1 : TIM7->ARR  = 4999;	// 100kHz/(4999+1) = 20Hz
                      Display_key(KEY1);
                      TFT_string(10,10, Green,  Black, " 50ms ON /  50ms OFF");
                      break;
          case KEY2 : TIM7->ARR  = 9999;	// 100kHz/(9999+1) = 10Hz
                      Display_key(KEY2);
	              TFT_string(10,10, Green,  Black, "100ms ON / 100ms OFF");
                      break;
          case KEY3 : TIM7->ARR  = 19999;	// 100kHz/(19999+1) = 5Hz
                      Display_key(KEY3);
	              TFT_string(10,10, Green,  Black, "200ms ON / 200ms OFF");
                      break;
          case KEY4 : TIM7->ARR  = 39999;	// 100kHz/(3999+1) = 2.5Hz
                      Display_key(KEY4);
	              TFT_string(10,10, Green,  Black, "400ms ON / 400ms OFF");
                      break;
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Display_key(U08 key)			/* display key input */
{
  TFT_string(7,16, White,Black, "KEY1");
  TFT_string(7,19, White,Black, "KEY2");
  TFT_string(7,22, White,Black, "KEY3");
  TFT_string(7,25, White,Black, "KEY4");

  if(key == KEY1)      TFT_string(7,16, Magenta,Black, "KEY1");
  else if(key == KEY2) TFT_string(7,19, Magenta,Black, "KEY2");
  else if(key == KEY3) TFT_string(7,22, Magenta,Black, "KEY3");
  else if(key == KEY4) TFT_string(7,25, Magenta,Black, "KEY4");
}
