#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "startup_stm32f767xx.s"

void printSum(unsigned int sum){
    if( sum<10){
      LCD_data( sum + '0');
    } else if(sum <20 && sum>=10) {
      LCD_data('1');
      LCD_data(sum - 10 + '0');
    } else if(sum == 20) {
      LCD_data('2');
      LCD_data('0');
    } 
}

volatile unsigned int interrupt_flag = 0, sum = 0;

void EXTI0_IRQHandler(void){
  sum+=1;
  while( (GPIOC->IDR & 0x00000001) != 0x00000001 );
  Delay_ms(20);
  LCD_string(0xC0, " KEY 1 ");
  EXTI->PR = 0x00000001;
  NVIC->ICPR[0] = 0x00000040;
  interrupt_flag = 1;
}
void EXTI1_IRQHandler(void){
  sum+=2;
  while( (GPIOC->IDR & 0x00000002) != 0x00000002 );
  Delay_ms(20);
  LCD_string(0xC0, " KEY 2 ");
  EXTI->PR = 0x00000002;
  NVIC->ICPR[0] = 0x00000080;
  interrupt_flag = 1;
}
void EXTI2_IRQHandler(void){
  sum+=3;
  while( (GPIOC->IDR & 0x00000004) != 0x00000004 );
  Delay_ms(20);
  LCD_string(0xC0, " KEY 3 ");
  EXTI->PR = 0x00000004;
  NVIC->ICPR[0] = 0x00000100;
  interrupt_flag = 1;
}
void EXTI3_IRQHandler(void){
  sum+=4;
  while( (GPIOC->IDR & 0x00000008) != 0x00000008 );
  Delay_ms(20);
  LCD_string(0xC0, " KEY 4 ");
  EXTI->PR = 0x00000008;
  NVIC->ICPR[0] = 0x00000200;
  interrupt_flag = 1;
}

int main(void){
  Delay_ms(100);
  Initialize_MCU();
  Initialize_LCD();
  RCC->AHB1ENR |= 0x0000001C;			// port C, D, E clock enable
  GPIOC->MODER &= 0x3FFFFC00;			// set PC15(buzzer) and PC4(LED) to output
  GPIOC->MODER |= 0x40000100;
  GPIOC->ODR &= 0xFFFF7FEF;			// PC15(buzzer) = PC4(LED) = 0
  GPIOD->MODER &= 0xFFFF0000;			// set PD7~0 to output
  GPIOD->MODER |= 0x00005555;
  GPIOE->MODER &= 0xFFFFF3F3;			// set PE5(LCD_E), PE1(TFT_RS) to output
  GPIOE->MODER |= 0x00000404;
  GPIOE->ODR &= 0xFFFFFFDD;			// PE5(LCD_E) = PE1(TFT_RS) = 0
  RCC->APB2ENR |= 0x00000001;			// enable SYSCFG
  SYSCFG->EXTICR[0] = 0x00002222;		// EXTI3,2,1,0 = PC3,2,1,0(KEY4,3,2,1)
  
  EXTI->IMR = 0x0000000F;                             //      interrupt mask 0~3 enable
  EXTI->RTSR = 0x00000000;                               //      rising edge disable
  EXTI->FTSR = 0x0000000F;                            //      falling edge enable
  NVIC->ISER[0] = 0x000003C0;                       //        nested vectored interrupt ->interrupt set enable 
  
  Delay_ms(50);
  
  LCD_string(0x80, " Press Key");
  LCD_string(0xC7, " ");
  sum = 0;
  while( !(sum > 20) ){
    if( interrupt_flag == 1){
      interrupt_flag =0;
      printSum(sum);
    }
  }
  LCD_string(0xC0, " Stop sum > 20");
  EXTI->IMR = 0x00000000;
  NVIC->ISER[0] = 0x00000000;
  
  while(1){
    blinkTheLED();
    Delay_ms(20);
    blinkTheLED();
    Delay_ms(20);
    blinkTheLED();
    Delay_ms(20);
    blinkTheLED();
    Delay_ms(20);
    
    Delay_ms(1000);
  }
}