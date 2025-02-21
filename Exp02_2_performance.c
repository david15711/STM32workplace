/* ============================================================================ */
/*	Exp02_2_performance.c : STM32F767VGT6의 연산속도 테스트 프로그램	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include <arm_math.h>				// 반드시 마지막에 인클루드할 것.

#define PI_rad	3.14159265358979F

int main(void)
{
  signed int i;
  unsigned int count;
  float x, y, z, answer;
  unsigned int sum_sinx, sum_sinf32x, number;	// 평균 처리용 변수

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD modul

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp02_2.c    ");
  Beep();

  SysTick->LOAD  = 0x00FFFFFF;			// maximum value
  SysTick->CTRL  = 0x00000005;			// SysTick enable and clock source = 216MHz
  SysTick->VAL   = 0;				// clear SysTick Counter

  while(1)
    { TFT_clear_screen();
      TFT_string(3, 2,White,Magenta,"  STM32F767VGT6의  연산속도 측정  ");
      TFT_string(3, 5,Cyan,   Black,"         (실수 계산 시간)         ");
      TFT_string(3, 8,Yellow, Black,"    x=000.000, y=00.00, z=0.0     ");
      TFT_string(3,13,White,  Black,"x * y * z * 9. = 0000 = 00.0000 us");
      TFT_string(3,15,White,  Black,"x / y / z / 9. = 0000 = 00.0000 us");
      TFT_string(3,19,White,  Black,"sqrt(x)        = 0000 = 00.0000 us");
      TFT_string(3,21,White,  Black,"log(x)         = 0000 = 00.0000 us");
      TFT_string(3,23,White,  Black,"exp(x)         = 0000 = 00.0000 us");
      TFT_color(Cyan,Black);
      TFT_xy(24,8);
      TFT_English(0x7F);
      Rectangle(10,96, 309,208, White);
      Line(10,144, 309,144, White);

      for(i = 1; i <= 9; i++)
        { x = 111.111 * (float)i;
          y = 11.11 * (float)i;
          z = 1.1 * (float)i;

          TFT_color(Yellow,Black);
          TFT_xy(9,8);
          TFT_unsigned_float(x,3,3);
          TFT_xy(20,8);
          TFT_unsigned_float(y,2,2);
          TFT_xy(29,8);
          TFT_unsigned_float(z,1,1);

	  SysTick->VAL = 0;			// x * y * z * 9.
	  answer = x * y * z * 9.;
          count = SysTick->VAL;
          if(count == 0) count = 0x00FFFFFF;
	  count = 0x00FFFFFF + 1 - count;
          TFT_color(Green,Black);
          TFT_xy(20,13);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,13);
          TFT_unsigned_float((float)count/216.,2,4);

	  SysTick->VAL = 0;			// x / y / z / 9.
	  answer = x / y / z / 9.;
          count = SysTick->VAL;
          if(count == 0) count = 0x00FFFFFF;
	  count = 0x00FFFFFF + 1 - count;
          TFT_xy(20,15);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,15);
          TFT_unsigned_float((float)count/216.,2,4);

	  SysTick->VAL = 0;			// sqrt(x)
	  answer = sqrt(x);
          count = SysTick->VAL;
          count = 0x00FFFFFF + 1 - count;
          TFT_color(Cyan,Black);
          TFT_xy(20,19);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,19);
          TFT_unsigned_float((float)count/216.,2,4);

	  SysTick->VAL = 0;			// log(x)
	  answer = log(x);
          count = SysTick->VAL;
          count = 0x00FFFFFF + 1 - count;
          TFT_xy(20,21);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,21);
          TFT_unsigned_float((float)count/216.,2,4);

	  SysTick->VAL = 0;			// exp(x)
	  answer = exp(x);
          count = SysTick->VAL;
          count = 0x00FFFFFF + 1 - count;
          TFT_xy(20,23);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,23);
          TFT_unsigned_float((float)count/216.,2,4);

          answer = answer * 2.;			// dummy use of answer variable
          Delay_ms(1000);
        }
      Delay_ms(2000);
      Beep();

      TFT_clear_screen();
      TFT_string(3, 2,White,Magenta,"  STM32F767VGT6의  연산속도 측정  ");
      TFT_string(3, 5,Cyan,   Black,"       (삼각함수 계산 시간)       ");
      TFT_string(3, 8,Yellow, Black,"              x = +90             ");
      TFT_string(3,13,White,  Black,"sin(x)         = 0000 = 00.0000 us");
      TFT_string(3,15,White,  Black,"arm_sin_f32(x) = 0000 = 00.0000 us");
      TFT_string(3,19,White,  Black,"cos(x)         = 0000 = 00.0000 us");
      TFT_string(3,21,White,  Black,"arm_cos_f32(x) = 0000 = 00.0000 us");
      TFT_string(3,25,White,  Black,"sin(x)의 평균         = 00.0000 us");
      TFT_string(3,27,White,  Black,"arm_sin_f32(x)의 평균 = 00.0000 us");
      TFT_color(Yellow,Black);
      TFT_xy(24,8);
      TFT_English(0x7F);
      Rectangle(10,96, 309,192, White);
      Line(10,144, 309,144, White);

      number = 1;
      sum_sinx = 0;
      sum_sinf32x = 0;
      for(i = -90; i <= 90; i += 5)
        { x = (float)i * PI_rad / 180.;

	  TFT_color(Yellow,Black);
          TFT_xy(21,8);
          TFT_signed_decimal(i,1,2);

	  SysTick->VAL = 0;			// sin()
	  y = sin(x);
          count = SysTick->VAL;
          count = 0x00FFFFFF + 1 - count;
          TFT_color(Green,Black);
          TFT_xy(20,13);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,13);
          TFT_unsigned_float((float)count/216.,2,4);

	  sum_sinx += count;

	  SysTick->VAL = 0;			// arm_sin_f32()
	  y = arm_sin_f32(x);
          count = SysTick->VAL;
          count = 0x00FFFFFF + 1 - count;
          TFT_xy(20,15);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,15);
          TFT_unsigned_float((float)count/216.,2,4);

	  sum_sinf32x += count;

	  SysTick->VAL = 0;			// cos()
	  y = cos(x);
          count = SysTick->VAL;
          count = 0x00FFFFFF + 1 - count;
          TFT_color(Cyan,Black);
          TFT_xy(20,19);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,19);
          TFT_unsigned_float((float)count/216.,2,4);

	  SysTick->VAL = 0;			// arm_cos_f32()
	  y = arm_cos_f32(x);
          count = SysTick->VAL;
          count = 0x00FFFFFF + 1 - count;
          TFT_xy(20,21);
          TFT_unsigned_decimal(count,0,4);
          TFT_xy(27,21);
          TFT_unsigned_float((float)count/216.,2,4);

	  TFT_color(Yellow,Black);		// 평균
          TFT_xy(27,25);
          TFT_unsigned_float((float)sum_sinx/216./(float)number,2,4);
          TFT_xy(27,27);
          TFT_unsigned_float((float)sum_sinf32x/216./(float)number,2,4);
          number += 1;

	  y = y * 2.;				// dummy use of y variable
          Delay_ms(1000);
        }
      Delay_ms(2000);
      Beep();
    }
}
