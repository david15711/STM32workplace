#include "stm32f767xx.h"
#include "OK-STM767.h"
#include <math.h>

unsigned int x, y;
float volume = 1.;
unsigned short DAC1_data[1000];

void SysTick_Handler(void) {
  DAC->DHR12R1 = (unsigned short)(volume * DAC1_data[x]);
  x++;
  if(x>1000) x=0;
}

void EXTI0_IRQHandler(void){
  for(x = 0; x < 1000; x++)			// calculate data of sinusoidal wave
    DAC1_data[x] = (signed short)(2047. * sin(x * 261.6256 * 2 * 3.141592/1000)) + 2047;
}

void EXTI1_IRQHandler(void){
  for(x = 0; x < 1000; x++)			// calculate data of sinusoidal wave
    DAC1_data[x] = (signed short)(2047. * sin(x * 293.6648 * 2 * 3.141592/1000)) + 2047;
}

void EXTI2_IRQHandler(void){
  for(x = 0; x < 1000; x++)			// calculate data of sinusoidal wave
    DAC1_data[x] = (signed short)(2047. * sin(x * 329.6276 * 2 * 3.141592/1000)) + 2047;
}

void EXTI3_IRQHandler(void){
  for(x = 0; x < 1000; x++)			// calculate data of sinusoidal wave
    DAC1_data[x] = (signed short)(2047. * sin(x * 349.2282 * 2 * 3.141592/1000)) + 2047;
}

  
int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  
  GPIOC->MODER &= 0xFFFFFF00;
  GPIOC->MODER |= 0x00000000;                   // PC3,2,1,0 = input mode
  RCC->APB2ENR |= 0x00000001;			// enable SYSCFG
  SYSCFG->EXTICR[0] = 0x00002222;		// EXTI3,2,1,0 = PC3,2,1,0(KEY4,3,2,1)
  EXTI->IMR = 0x0000000F;                       // interrupt mask 0~3 enable
  EXTI->RTSR = 0x00000000;                      // rising edge disable
  EXTI->FTSR = 0x0000000F;                      // falling edge enable
  NVIC->ISER[0] = 0x000003C0;                   // nested vectored interrupt ->interrupt set enable 
  
  //DAC enable
  GPIOA->MODER |= 0x00000F00;			// PA5, PA4 = analog mode
  RCC->APB1ENR |= 0x20000000;			// enable DAC clock
  DAC->CR = 0x00030003;				// DAC channel 2/1 enable, output buffer disable
  //ADC2 enable
  GPIOA->MODER |= 0x00000030;			// use ADC2
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00800000;			// enable channel 17~18, ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000249;			// sampling time of channel 0~3 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000002;			// total regular channel number = 1
  
  SysTick->LOAD  = 26999;			        // 27MHz/27000 = 1000Hz
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000003;			// 216MHz/8 = 27MHz, enable SysTick and interrupt

  while(1) {
    ADC2->CR2 |= 0x40000000;
    volume = (float)ADC2->DR/4095.;
  }
}
