/* ============================================================================ */
/*     Exp14_3_function(3).c : D/A 컨버터의 함수 파형 출력(실시간 계산 출력)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void SysTick_Handler(void);			/* SysTick interrupt function */

volatile unsigned short data_count = 0;

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void SysTick_Handler(void)			/* SysTick interrupt function */
{
  unsigned short DAC1_data, DAC2_data;

  if(data_count <= 100)				// triangular wave
     DAC1_data = (unsigned short)((4095./100.) * (float)data_count);
  else
     DAC1_data = (unsigned short)((4095./100.) * (float)(100-data_count));

  DAC2_data = (signed short)(2047. * sin(data_count * 1.8 * 3.141592 / 180.)) + 2047;// sinusoidal wave

  DAC->DHR12R1 = DAC1_data;			// output data of DAC channel 1
  DAC->DHR12R2 = DAC2_data;			// output data of DAC channel 2

  data_count++;
  if(data_count >= 200) data_count = 0;
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp14_3.c    ");

  TFT_string(7,4,White,Black,"(실시간 계산에 의한 출력)");
  TFT_string(3, 7,White,Magenta," D/A 컨버터의 수학함수 파형 출력 ");
  TFT_string(8,13,White,Black,"J4의 우측 단자(DAC1_OUT)");
  TFT_string(4,16,Green,Black,"약 1kHz의 삼각파 출력 (0~+3.3V)");
  TFT_string(8,21,White,Black,"J5의 우측 단자(DAC2_OUT)");
  TFT_string(4,24,Green,Black,"약 1kHz의 정현파 출력 (0~+3.3V)");
  Beep();

  GPIOA->MODER |= 0x00000F00;			// PA5, PA4 = analog mode
  RCC->APB1ENR |= 0x20000000;			// enable DAC clock

  DAC->CR = 0x00030003;				// DAC channel 1~2 enable, output buffer disable

  SysTick->LOAD  = 134;				// 27MHz/(134+1) = 200kHz
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000003;			// 216MHz/8 = 27MHz, enable interrupt

  while(1);
}
