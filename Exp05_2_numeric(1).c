/* ============================================================================ */
/*	Exp05_2_numeric(1).c : TFT-LCD 수치 데이터 출력 프로그램(1)		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void TFT_2d(unsigned int number);		/* display 2-digit decimal number */
void TFT_4d(unsigned int number);		/* display 4-digit decimal number */
void TFT_1d1(float number);		        /* floating-point number x.x */
void TFT_2d2(float number);		        /* floating-point number xx.xx */

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned int x, y;
  float a, b;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp05_2.c    ");

  while(1)
    { TFT_clear_screen();			// 정수
      TFT_string(6,0,White, Magenta," Multiplication Table(정수) ");

      for(x = 2; x <= 9; x++)
        { for(y = 1; y <= 9; y++)
	    { TFT_color(Green, Black);		// 단자리 구구단
	      TFT_xy( 5,3*y); TFT_English(x + '0');
	      TFT_xy( 7,3*y); TFT_English('x');
              TFT_xy( 9,3*y); TFT_English(y + '0');
              TFT_xy(11,3*y); TFT_English('=');
              TFT_xy(13,3*y); TFT_2d(x*y);

              TFT_color(Yellow, Black);		// 10자리 구구단
	      TFT_xy(21,3*y); TFT_2d(x*10);
	      TFT_xy(24,3*y); TFT_English('x');
              TFT_xy(26,3*y); TFT_2d(y*10);
              TFT_xy(29,3*y); TFT_English('=');
              TFT_xy(31,3*y); TFT_4d((x*10)*(y*10));
            }
          Beep();
          Delay_ms(3000);
        }

      TFT_clear_screen();			// 실수
      TFT_string(6,0,White, Magenta," Multiplication Table(실수) ");

      for(x = 1,a = 1.1; x <= 9; x++, a += 1.1)
        { for(y = 1, b = 1.1; y <= 9; y++, b += 1.1)
	    { TFT_color(Green, Black);		// 실수 구구단
	      TFT_xy(11,3*y); TFT_1d1(a);
	      TFT_xy(15,3*y); TFT_English('x');
              TFT_xy(17,3*y); TFT_1d1(b);
              TFT_xy(21,3*y); TFT_English('=');
              TFT_xy(23,3*y); TFT_2d2(a*b);
            }
          Beep();
          Delay_ms(3000);
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void TFT_2d(unsigned int number)                /* display 2-digit decimal number */
{
  unsigned int i;

  i = number/10;                                // 10^1
  if(i == 0) TFT_English(' ');
  else       TFT_English(i + '0');

  i = number % 10;                              // 10^0
  TFT_English(i + '0');
}

void TFT_4d(unsigned int number)                /* display 4-digit decimal number */
{
  unsigned int i, flag;

  flag = 0;
  i = number/1000;                              // 10^3
  if(i == 0) TFT_English(' ');
  else {     TFT_English(i + '0');
             flag = 1;
       }

  number = number % 1000;                       // 10^2
  i = number/100;
  if((i == 0) && (flag == 0))
             TFT_English(' ');
  else {     TFT_English(i + '0');
             flag = 1;
       }

  number = number % 100;                        // 10^1
  i = number/10;
  if((i == 0) && (flag == 0))
             TFT_English(' ');
  else {     TFT_English(i + '0');
             flag = 1;
       }

  i = number % 10;                              // 10^0
  TFT_English(i + '0');
}

void TFT_1d1(float number)		        /* floating-point number x.x */
{
  unsigned int i, j;

  j = (unsigned int)(number*10. + 0.5);
  i = j / 10;					// 10^0
  TFT_English(i + '0');
  TFT_English('.');
  i = j % 10;					// 10^-1
  TFT_English(i + '0');
}

void TFT_2d2(float number)		        /* floating-point number xx.xx */
{
  unsigned int i, j;

  j = (unsigned int)(number*100. + 0.5);
  i = j / 1000;					// 10^1
  if(i == 0) TFT_English(' ');
  else       TFT_English(i + '0');

  j = j % 1000;					// 10^0
  i = j / 100;
  TFT_English(i + '0');
  TFT_English('.');

  j = j % 100;					// 10^-1
  i = j / 10;
  TFT_English(i + '0');

  i = j % 10;					// 10^-2
  TFT_English(i + '0');
}
