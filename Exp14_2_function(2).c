/* ============================================================================ */
/*      Exp14_2_function(2).c : D/A 컨버터의 함수 파형 출력(인터럽트 사용)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void SysTick_Handler(void);			/* SysTick interrupt function */

volatile unsigned short DAC1_data[200], DAC2_data[200], data_count = 0;

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void SysTick_Handler(void)			/* SysTick interrupt function */
{
  DAC->DHR12R1 = DAC1_data[data_count];		// output data of DAC channel 1
  DAC->DHR12R2 = DAC2_data[data_count];		// output data of DAC channel 2

  data_count++;
  if(data_count >= 200) data_count = 0;
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned short i, x;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp14_2.c    ");

  TFT_string(11,4,White,Black,"(인터럽트를 사용)");
  TFT_string(3, 7,White,Magenta," D/A 컨버터의 수학함수 파형 출력 ");
  TFT_string(8,13,White,Black,"J4의 우측 단자(DAC1_OUT)");
  TFT_string(4,16,Green,Black,"약 1kHz의 삼각파 출력 (0~+3.3V)");
  TFT_string(8,21,White,Black,"J5의 우측 단자(DAC2_OUT)");
  TFT_string(4,24,Green,Black,"약 1kHz의 정현파 출력 (0~+3.3V)");
  Beep();

  for(x = 0; x <= 100; x++)			// calculate data of triangular wave
     DAC1_data[x] = (unsigned short)((4095./100.) * (float)x);
  i = 1;
  for(x = 101; x < 200; x++)
    { DAC1_data[x] = DAC1_data[100-i];
      i++;
    }

  for(x = 0; x < 200; x++)			// calculate data of sinusoidal wave   1/20 Hz
    DAC2_data[x] = (signed short)(2047. * sin(x * 1.8 * 3.141592 / 180.)) + 2047;

  GPIOA->MODER |= 0x00000F00;			// PA5, PA4 = analog mode
  RCC->APB1ENR |= 0x20000000;			// enable DAC clock

  DAC->CR = 0x00030003;				// DAC channel 1~2 enable, output buffer disable

  SysTick->LOAD  = 134;				// 27MHz/(134+1) = 200kHz
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000003;			// 216MHz/8 = 27MHz, enable interrupt

  while(1);
}
