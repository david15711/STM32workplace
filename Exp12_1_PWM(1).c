/* ============================================================================ */
/*		Exp12_1_PWM(1).c : 타이머1의 1채널 PWM 출력 프로그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned char key;
  unsigned int PWM;				// pulse width

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp12_1.c    ");
  Beep();

  TFT_string(0,4, White,Magenta, "      타이머 TIM1의 PWM 출력 모드       ");
  TFT_string(10,7,White,Black, "TP10(TIM1_CH4) 단자");
  TFT_string(14,7, White,Blue, "(TIM1_CH4)");
  TFT_string(12,12, Cyan,Black, "주기   = 1000 us");
  TFT_string(12,15, Cyan,Black, "펄스폭 =  500 us");
  TFT_string(12,18, Cyan,Black, "듀티비 =   50 %");
  TFT_string( 4,24, White,Black, "KEY3~KEY4로 듀티비를 가변하시오.");
  TFT_string(20,28,White,Blue," 증가( ) ");
  TFT_xy(26,28); TFT_English(0x18);
  TFT_string(30,28,White,Blue," 감소( ) ");
  TFT_xy(36,28); TFT_English(0x19);

  PWM = 499;					// initialize PWM = 499 (duty ratio 50%)

  GPIOE->MODER &= 0xCFFFFFFF;			// PE14 = alternate function mode
  GPIOE->MODER |= 0x20000000;
  GPIOE->AFR[1] &= 0xF0FFFFFF;			// PE14 = TIM1_CH4
  GPIOE->AFR[1] |= 0x01000000;

  RCC->APB2ENR |= 0x00000001;			// enable TIM1 clock

  TIM1->PSC = 107;				// 108MHz/(107+1) = 1MHz
  TIM1->ARR = 999;				// 1MHz / (999+1) = 1kHz
  TIM1->CCR4 = PWM;
  TIM1->CNT = 0;				// clear counter
  TIM1->CCMR2 = 0x00006C00;			// OC4M = 0110(PWM mode 1), CC4S = 00(output)
  TIM1->CCER = 0x00001000;			// CC4E = 1(enable OC4 output)
  TIM1->BDTR = 0x00008000;			// MOE = 1
  TIM1->CR1 = 0x0005;				// edge-aligned, up-counter, enable TIM1

  while(1)
    { key = Key_input();			// key input
      if((key == KEY3) && (PWM < 899))		// if KEY3, increment duty ratio by 10%
        { PWM += 100;
          TIM1->CCR4  = PWM;
          TFT_color(Yellow,Black);
          TFT_xy(21,15);
	  TFT_unsigned_decimal(PWM+1,0,4);
          TFT_xy(22,18);
	  TFT_unsigned_decimal((PWM+1)/10,0,3);
        }
      else if((key == KEY4) && (PWM > 99))	// if KEY4, decrement duty ratio by 10%
        { PWM -= 100;
          TIM1->CCR4  = PWM;
          TFT_color(Yellow,Black);
          TFT_xy(21,15);
	  TFT_unsigned_decimal(PWM+1,0,4);
          TFT_xy(22,18);
	  TFT_unsigned_decimal((PWM+1)/10,0,3);
	}
    }
}
