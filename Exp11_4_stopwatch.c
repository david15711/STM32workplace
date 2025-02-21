/* ============================================================================ */
/*		Exp11_4_stopwatch.c : 타이머7을 이용한 전자 스톱워치		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

void TIM7_IRQHandler(void);			/* TIM7 interrupt function */
void Clear_time(void);				/* clear time */
void Display_key(U08 key);			/* display key input */

volatile unsigned int second, minute, hour;
volatile unsigned char run_flag;

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void TIM7_IRQHandler(void)			/* TIM7 interrupt function */
{
  TIM7->SR = 0x0000;				// clear pending bit of TIM7 interrupt

  second++;                                     // increment 1/100 second
  if(second == 6000)                            // second = 60 ?
    { second = 0;                               // if yes, second = 0 and minute++
      minute++;
      if(minute == 60)                          // minute = 60 ?
        { minute = 0;                           // if yes, minute = 0 and hour++
          hour++;
          if(hour == 100)                       // hour = 100 ?
            hour = 0;                           // if yes, hour = 0
        }
    }

  TFT_color(Green,Black);			// display time
  TFT_xy(5,10);
  TFT_unsigned_decimal_large(hour,1,2);
  TFT_xy(10,10);
  TFT_unsigned_decimal_large(minute,1,2);
  TFT_xy(15,10);
  TFT_unsigned_decimal_large(second / 100,1,2);
  TFT_xy(18,10);
  TFT_unsigned_decimal_large(second % 100,1,2);
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
  LCD_string(0xC0,"   Exp11_4.c    ");
  Beep();

  TFT_string(11, 4, White,Magenta, " 전자 스톱 워치 ");
  TFT_string_large(5,10, Magenta,Black, "00 : 00 : 00.00");
  TFT_string(10,18, White,Black, "KEY1 - 스타트(start)");
  TFT_string(10,20, White,Black, "KEY2 -");
  TFT_string(10,22, White,Black, "KEY3 - 리셋(reset)");
  TFT_string(10,24, White,Black, "KEY4 - 스톱(stop)");
  Clear_time();					/* clear time */

  RCC->APB1ENR |= 0x00000020;			// enable TIM7 clock
  TIM7->PSC = 1079;				// 108MHz/(1079+1) = 100kHz
  TIM7->ARR = 999;				// 100kHz/(999+1) = 100Hz
  TIM7->CNT = 0;				// clear counter
  TIM7->DIER = 0x0000;				// disable update interrupt
  TIM7->CR1 = 0x0005;				// enable TIM7 and update event
  NVIC->ISER[1] |= 0x00800000;			// enable (55)TIM7 interrupt

  run_flag = 0;					// initial status

  while(1)
    { key = Key_input();			// key input
      switch(key)
        { case KEY1 : if(run_flag == 1) break;	// if run_flag=1, ignore KEY1
		      run_flag =  1;
                      Display_key(KEY1);
                      TIM7->DIER = 0x0001;	// enable update interrupt
                      break;
          case KEY3 : if(run_flag == 1) break;	// if run_flag=1, ignore KEY3
	              TIM7->DIER = 0x0000;	// disable update interrupt
                      Clear_time();
                      Display_key(KEY3);
                      break;
          case KEY4 : if(run_flag == 0) break;	// if run_flag=0, ignore KEY4
                      run_flag = 0;
	              TIM7->DIER = 0x0000;	// disable update interrupt
                      Display_key(KEY4);
                      break;
          default:    break;
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Clear_time(void)				/* clear time */
{
  second = 0;                                   // clear time
  minute = 0;
  hour = 0;

  TFT_string_large( 5,10, Green,Black, "00 : 00 : 00.00"); // clear display
  TFT_string_large( 8,10, Green,Black, ":");
  TFT_string_large(13,10, Green,Black, ":");
  TFT_string_large(17,10, Green,Black, ".");
}

void Display_key(U08 key)			/* display key input */
{
  TFT_string(10,18, White,Black, "KEY1");
  TFT_string(10,22, White,Black, "KEY3");
  TFT_string(10,24, White,Black, "KEY4");

  if(key == KEY1)      TFT_string(10,18, Magenta,Black, "KEY1");
  else if(key == KEY3) TFT_string(10,22, Magenta,Black, "KEY3");
  else if(key == KEY4) TFT_string(10,24, Magenta,Black, "KEY4");
}
