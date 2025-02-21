/* ============================================================================ */
/*		Exp12_2_PWM(2).c : 타이머3의 2채널 PWM 출력 프로그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned char key;
  unsigned int PWM1, PWM2;			// pulse width of TIM3_CH1 and TIM3_CH2

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp12_2.c    ");
  Beep();

  TFT_string(0, 2, White,Magenta, "      타이머 TIM3의 PWM 출력 모드       ");
  TFT_string(4, 6, Cyan,   Black, "J4 좌측 단자 : 주기   = 1000 us");
  TFT_string(4, 8, Cyan,   Black, " (TIM3_CH1)    펄스폭 =  200 us");
  TFT_string(4,10, Cyan,   Black, "               듀티비 =   20 % ");
  TFT_string(5, 8, White,   Red, "(TIM3_CH1)");
  TFT_string(4,14, Cyan,   Black, "J5 좌측 단자 : 주기   = 1000 us");
  TFT_string(4,16, Cyan,   Black, " (TIM3_CH2)    펄스폭 =  500 us");
  TFT_string(4,18, Cyan,   Black, "               듀티비 =   50 % ");
  TFT_string(5,16, White,   Blue, "(TIM3_CH2)");
  TFT_string(4,25, White,  Black, "KEY1~KEY4로 듀티비를 가변하시오.");
  TFT_string( 1,28,White,Red," CH1( ) ");
  TFT_xy( 6,28); TFT_English(0x18);
  TFT_string(11,28,White,Red," CH1( ) ");
  TFT_xy(16,28); TFT_English(0x19);
  TFT_string(21,28,White,Blue," CH2( ) ");
  TFT_xy(26,28); TFT_English(0x18);
  TFT_string(31,28,White,Blue," CH2( ) ");
  TFT_xy(36,28); TFT_English(0x19);

  PWM1 = 199;					// initialize PWM1 = 199 (duty ratio 20%)
  PWM2 = 499;					// initialize PWM2 = 499 (duty ratio 50%)

  GPIOA->MODER &= 0xFFFF0FFF;			// PA7/PA6 = alternate function mode
  GPIOA->MODER |= 0x0000A000;
  GPIOA->AFR[0] &= 0x00FFFFFF;			// PA7 = TIM3_CH2, PA6 = TIM3_CH1
  GPIOA->AFR[0] |= 0x22000000;

  RCC->APB1ENR |= 0x00000002;			// enable TIM3 clock

  TIM3->PSC = 107;				// 108MHz/(107+1) = 1MHz
  TIM3->ARR = 999;				// 1MHz/(999+1) = 1kHz
  TIM3->CCR1 = PWM1;
  TIM3->CCR2 = PWM2;
  TIM3->CNT = 0;				// clear counter
  TIM3->CCMR1 = 0x00006C6C;			// OC2M=OC1M=0110(PWM mode 1), CC2S=CC1S=00(output)
  TIM3->CCER = 0x0011;				// CC2E = CC1E = 1(enable OC2/OC1 output)
  TIM3->CR1 = 0x0005;				// edge-aligned, up-counter, enable TIM3

  while(1)
    { key = Key_input();			// key input
      if((key == KEY1) && (PWM1 != 899))	// if KEY1, increment CH1 duty ratio by 10%
        { PWM1 += 100;
          TIM3->CCR1  = PWM1;
          TFT_color(Yellow,Black);
          TFT_xy(28,8);
	  TFT_unsigned_decimal(PWM1+1,0,4);
          TFT_xy(29,10);
          TFT_unsigned_decimal((PWM1+1)/10,0,3);
        }
      else if((key == KEY2) && (PWM1 != 99))	// if KEY2, decrement CH1 duty ratio by 10%
        { PWM1 -= 100;
          TIM3->CCR1  = PWM1;
          TFT_color(Yellow,Black);
          TFT_xy(28,8);
	  TFT_unsigned_decimal(PWM1+1,0,4);
          TFT_xy(29,10);
          TFT_unsigned_decimal((PWM1+1)/10,0,3);
	}
      else if((key == KEY3) && (PWM2 != 899))	// if KEY3, increment CH2 duty ratio by 10%
        { PWM2 += 100;
          TIM3->CCR2  = PWM2;
          TFT_color(Yellow,Black);
          TFT_xy(28,16);
	  TFT_unsigned_decimal(PWM2+1,0,4);
          TFT_xy(29,18);
          TFT_unsigned_decimal((PWM2+1)/10,0,3);
        }
      else if((key == KEY4) && (PWM2 != 99))	// if KEY4, decrement CH2 duty ratio by 10%
        { PWM2 -= 100;
          TIM3->CCR2  = PWM2;
          TFT_color(Yellow,Black);
          TFT_xy(28,16);
	  TFT_unsigned_decimal(PWM2+1,0,4);
          TFT_xy(29,18);
          TFT_unsigned_decimal((PWM2+1)/10,0,3);
	}
    }
}
