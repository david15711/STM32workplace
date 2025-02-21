/* ============================================================================ */
/*		Exp09_1_SysTick.c : SysTick 타이머 인터럽트			*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void SysTick_Handler(void);			/* SysTick interrupt function */
void Display_key(U08 key);			/* display key input */

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void SysTick_Handler(void)			/* SysTick interrupt function */
{
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
  LCD_string(0xC0,"   Exp09_1.c    ");
  Beep();

  TFT_string( 1, 4, White,Magenta, " SysTick 타이머 인터럽트 이용 LED 점멸 ");
  TFT_string(10,10, Green,  Black, " 50ms ON /  50ms OFF");
  TFT_string(7,16, White,Black, "KEY1 -  50ms 주기 인터럽트");
  TFT_string(7,19, White,Black, "KEY2 - 100ms 주기 인터럽트");
  TFT_string(7,22, White,Black, "KEY3 - 200ms 주기 인터럽트");
  TFT_string(7,25, White,Black, "KEY4 - 400ms 주기 인터럽트");

  SysTick->LOAD  = 1349999;			// 27MHz/(1349999+1) = 20Hz
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000003;			// 216MHz/8 = 27MHz, enable SysTick and interrupt

  while(1)
    { key = Key_input();			// key input
      switch(key)
        { case KEY1 : SysTick->LOAD  = 1349999;	// 27MHz/(1349999+1) = 20Hz
                      Display_key(KEY1);
	              TFT_string(10,10, Green,  Black, " 50ms ON /  50ms OFF");
                      break;
          case KEY2 : SysTick->LOAD  = 2699999;	// 27MHz/(2699999+1) = 10Hz
                      Display_key(KEY2);
    	              TFT_string(10,10, Green,  Black, "100ms ON / 100ms OFF");
                      break;
          case KEY3 : SysTick->LOAD  = 5399999;	// 27MHz/(5399999+1) = 5Hz
                      Display_key(KEY3);
	              TFT_string(10,10, Green,  Black, "200ms ON / 200ms OFF");
                      break;
          case KEY4 : SysTick->LOAD  = 10799999;// 27MHz/(10799999+1) = 2.5Hz
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
