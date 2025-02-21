/* ============================================================================ */
/*	Exp07_5_realtime.c : VR1 전압을 실시간으로 A/D 변환하여 측정한 파형	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void SysTick_Handler(void);			/* SysTick interrupt with 20ms interval */
void Draw_axis(void);				/* draw x-axis and y-axis */

unsigned short x0, x, y0, y;
volatile unsigned short ADC_result, ADC_count, end_flag;
volatile unsigned char ADC_buffer[291];

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void SysTick_Handler(void)			/* SysTick interrupt with 20ms interval */
{
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));		// wait for end of conversion

  x = ADC_count;
  ADC_buffer[x] = (unsigned short)((float)ADC1->DR*3.3*50./4095. + 0.5);

  if(ADC_count != 0)				// draw graph
    { x0 = ADC_count - 1;
      x = ADC_count;
      y0 = ADC_buffer[x0];
      y = ADC_buffer[x];
      Line(20+x0,215-y0, 20+x,215-y, Magenta);
    }

  if(ADC_count >= 290)				// screen end ?
    { end_flag = 1;
      ADC_count = 0;
    }
  else
    ADC_count++;
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned short i;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp07_5.c    ");

  GPIOA->MODER |= 0x000000FF;			// use ADC0~ADC3
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000040;			// sampling time of channel 2 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;			// total regular channel number = 1
  ADC1->SQR3 = 0x00000002;			// channel 2 (VR1)

  SysTick->LOAD  = 539999;			// 27MHz/(539999+1) = 50Hz
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000003;			// 216MHz/8 = 27MHz, enable SysTick and interrupt

  while(1)
    { Draw_axis();				// draw x-axis and y-axis
      Beep();
      end_flag = 0;
      ADC_count = 0;
      SysTick->CTRL  = 0x0000003;		// enable SysTick and interrupt

      while(end_flag == 0);			// wait screen graph end

      SysTick->CTRL  = 0x0000000;		// disable SysTick and interrupt
      for(i = 1; i <= 290; i++)			// clear old graph
        { x0 = i - 1;
          x = i;
          y0 = ADC_buffer[x0];
          y = ADC_buffer[x];
          Line(20+x0,215-y0, 20+x,215-y, Black);
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Draw_axis(void)				/* draw x-axis and y-axis */
{
  TFT_string(15,0, White,Blue, " VR1 전압 ");	// display title

  Line(15,215, 310,215, White);			// draw x-axis
  Line(305,210, 310,215, White);
  Line(305,220, 310,215, White);
  Line(70,212, 70,218, White);			// draw x-axis scale
  Line(120,212, 120,218, White);
  Line(170,212, 170,218, White);
  Line(220,212, 220,218, White);
  Line(270,212, 270,218, White);
  TFT_color(Cyan,Black);			// write time scale
  TFT_English_pixel(16,222, '0');
  TFT_English_pixel(66,222, '1');
  TFT_English_pixel(116,222, '2');
  TFT_English_pixel(166,222, '3');
  TFT_English_pixel(216,222, '4');
  TFT_English_pixel(266,222, '5');
  TFT_color(Magenta,Black);
  TFT_English_pixel(288,222, '[');
  TFT_English_pixel(296,222, 's');
  TFT_English_pixel(304,222, ']');

  Line(20,220, 20,5, White);			// draw y-axis
  Line(15,10, 20,5, White);
  Line(25,10, 20,5, White);
  Line(17,165, 23,165, White);			// draw y-axis scale
  Line(17,115, 23,115, White);
  Line(17,65, 23,65, White);
  Line(17,15, 23,15, White);
  TFT_color(Cyan,Black);			// write voltage scale
  TFT_English_pixel(5,208, '0');
  TFT_English_pixel(5,158, '1');
  TFT_English_pixel(5,108, '2');
  TFT_English_pixel(5,58, '3');
  TFT_English_pixel(5,8, '4');
  TFT_color(Magenta,Black);
  TFT_English_pixel(28,8, '[');
  TFT_English_pixel(36,8, 'V');
  TFT_English_pixel(44,8, ']');
}
