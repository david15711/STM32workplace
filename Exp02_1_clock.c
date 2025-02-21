/* ============================================================================ */
/*		Exp02_1_clock.c : MCO1/MCO2 주파수 출력 프로그램	        */
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp02_1.c    ");
  Beep();

  TFT_string(0,5,White,Magenta,"   MCO1/MCO2 단자 주파수 출력 테스트   ");
  TFT_string(0,7,Cyan,   Black,"   (TP1/TP2)");

  GPIOA->MODER &= 0xFFFCFFFF;			// PA8 = MCO1
  GPIOA->MODER |= 0x00020000;
  GPIOA->AFR[1] &= 0xFFFFFFF0;
  GPIOA->OSPEEDR |= 0x00030000;			// PA8(MCO1) = 180MHz very high speed

  GPIOC->MODER &= 0xFFF3FFFF;			// PC9 = MCO2
  GPIOC->MODER |= 0x00080000;
  GPIOC->AFR[1] &= 0xFFFFFF0F;
  GPIOC->OSPEEDR |= 0x000C0000;			// PC9(MCO2) = 180MHz very high speed

  while(1)
    { TFT_string_large(1,15,  Cyan,Black,"MCO1 = HSI      =  16MHz");
      TFT_string_large(1,19,  Cyan,Black,"MCO2 = SYSCLK/2 = 108MHz");
      RCC->CFGR &= 0x009FFFFF;			// MCO1 = HSI/1
      RCC->CFGR |= 0x20000000;			// MCO2 = SYSCLK/2
      Delay_ms(5000);
      Beep();

      TFT_string_large(1,15,Yellow,Black,"MCO1 = HSI/2    =   8MHz");
      TFT_string_large(1,19,Yellow,Black,"MCO2 = SYSCLK/4 =  54MHz");
      RCC->CFGR &= 0x009FFFFF;			// MCO1 = HSI/2
      RCC->CFGR |= 0x34000000;			// MCO2 = SYSCLK/4
      Delay_ms(5000);
      Beep();

      TFT_string_large(1,15, Green,Black,"MCO1 = HSE      =  16MHz");
      TFT_string_large(1,19, Green,Black,"MCO2 = PLL/2    = 108MHz");
      RCC->CFGR &= 0x009FFFFF;			// MCO1 = HSE/1
      RCC->CFGR |= 0xE0400000;			// MCO2 = PLL/2
      Delay_ms(5000);
      Beep();

      TFT_string_large(1,15, White,Black,"MCO1 = HSE/2    =   8MHz");
      TFT_string_large(1,19, White,Black,"MCO2 = PLL/4    =  54MHz");
      RCC->CFGR &= 0x009FFFFF;			// MCO1 = HSE/2
      RCC->CFGR |= 0xF4400000;			// MCO2 = PLL/4
      Delay_ms(5000);
      Beep();
    }
}
