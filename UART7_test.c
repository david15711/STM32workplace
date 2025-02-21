/* ============================================================================ */
/*		Exp15_1_loopback.c : RS-232C 직렬통신 루프백 테스트		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"


void EXTI0_IRQHandler(void)			/* EXTI0 interrupt function */
{
  LED_on();
  Delay_ms(100);
  LED_off();
  EXTI->PR = 0x00000001;			// clear pending bit of EXTI0

  UART7->TDR = 'c';
  UART7->TDR = 'c';
  Delay_us(10);
  UART7->TDR = '\n';
}

void EXTI1_IRQHandler(void)			/* EXTI1 interrupt function */
{
  LED_on();
  Delay_ms(100);
  LED_off();
  EXTI->PR = 0x00000002;			// clear pending bit of EXTI1

  UART7->TDR = 's';
  UART7->TDR = 's';
  Delay_us(10);
  UART7->TDR = '\n';
}

void EXTI2_IRQHandler(void)			/* EXTI2 interrupt function */
{
  LED_on();
  Delay_ms(100);
  LED_off();
  EXTI->PR = 0x00000004;			// clear pending bit of EXTI2

  UART7->TDR = 'a';
  UART7->TDR = 'a';
  Delay_us(10);
  UART7->TDR = '\n';
}

void EXTI3_IRQHandler(void)			/* EXTI3 interrupt function */
{
  LED_on();
  Delay_ms(100);
  LED_off();
  EXTI->PR = 0x00000008;			// clear pending bit of EXTI3
  USART3->TDR = 'B';
}

int main(void)
{
  unsigned char i, j, RXD, test_flag;
  unsigned char string[24] = { "OK-STM767 Serial Test" };

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  TFT_string(4,4,White,Magenta," RS-232C 직렬통신 루프백 테스트 ");
  Delay_ms(1000);

  //PA8 == UART7_RX
  GPIOA->MODER &= 0xFFFCFFFF;
  GPIOA->MODER |= 0x00020000;
  GPIOA->AFR[1] &= 0xFFFFFFF0;
  GPIOA->AFR[1] |= 0x0000000C;
  //PE8 == UART7_TX
  GPIOE->MODER &= 0xFFFCFFFF;
  GPIOE->MODER |= 0x00020000;
  GPIOE->AFR[1] &= 0xFFFFFFF0;
  GPIOE->AFR[1] |= 0x00000008;
  //UART7 enable
  RCC->APB1ENR |= 0x40000000;
  UART7->CR2 = 0x00000000;			// asynchronous mode, 1 stop bit
  UART7->CR3 = 0x00000000;		// 3sampling bit
  UART7->CR1 = 0x0000000D;
  UART7->BRR = 5625;                    //9600
  Delay_ms(1);
  RXD = UART7->RDR;
  
  
  
  //USART2
  GPIOD->MODER&=0xFFFF00FF;
  GPIOD->MODER|=0x00002800;
  GPIOD->AFR[0]&=0xF00FFFFF;
  GPIOD->AFR[0]|=0x07700000;
  RCC->APB1ENR |=0x00020000;
  USART2->CR2 = 0x00000000;			// asynchronous mode, 1 stop bit
  USART2->CR3 = 0x00000000;		// 3sampling bit
  USART2->BRR = 469;			// 115200 bps = 54MHz/468.75
  USART2->CR1 = 0x0000000D;			// TE=RE=UE = 1, 8 data bit, oversampling by 16
  Delay_ms(1);
  RXD = USART2->RDR;				// dummy read
  
  GPIOB->MODER &= 0xFF0FFFFF;			// PB10 = USART3_TX, PB11 = USART3_RX   1010
  GPIOB->MODER |= 0x00A00000;
  GPIOB->AFR[1] &= 0xFFFF00FF;
  GPIOB->AFR[1] |= 0x00007700;                  //0111  = AF7 = USART3_TX, USART3_RX
  RCC->APB1ENR |= 0x00040000;			// enable USART3 clock
  USART3->CR1 = 0x0000000D;			// TE=RE=UE = 1, 8 data bit, oversampling by 16
  USART3->CR2 = 0x00000000;			// asynchronous mode, 1 stop bit
  USART3->CR3 = 0x00000000;			// 3 sampling bit
  USART3->BRR = 469;				// 19200 bps = 54MHz/2812.5
  Delay_ms(1);
  RXD = USART3->RDR;				// dummy read
  
  GPIOC->MODER &= 0xFFFFFF00;			// PC3,2,1,0 = input mode
  RCC->APB2ENR |= 0x00000001;			// enable SYSCFG
  SYSCFG->EXTICR[0] = 0x00002222;		// EXTI3,2,1,0 = PC3,2,1,0(KEY4,3,2,1)
  EXTI->IMR = 0x0000000F;			// enable EXTI3,2,1,0 interrupt
  EXTI->RTSR = 0x00000000;			// disable EXTI3,2,1,0 rising edge
  EXTI->FTSR = 0x0000000F;			// enable EXTI3,2,1,0 falling edge
  NVIC->ISER[0] = 0x000003C0;			// enable (9)EXTI3 ~ (6)EXTI0 interrupt

  while(1);
    /*{ test_flag = 0;
      for(i = 0; i < 23; i++) {
          UART7->TDR = string[i];		// transmit test data
          Delay_ms(100);
          if(UART7->ISR & 0x00000020) RXD = UART7->RDR; // receive test data
          else                         RXD = ' ';
          USART3->TDR = RXD;
          TFT_xy(14+i,18);			// display transmitted test data
          TFT_color(Green,Black);
          TFT_English(string[i]);
          Delay_ms(5);

          TFT_xy(14+i,21);			// display transmitted test data
          TFT_color(Magenta,Black);
          TFT_English(RXD);

          if(RXD == string[i])			// data OK ?
            test_flag++;

          //Beep();
          Delay_ms(300);
        }

      if(test_flag == 23)
        { TFT_string(12,25,  Green,Black,"통신 테스트 OK !");
          //Beep();
          
        }
      else
        { TFT_string(11,25,Magenta,Black,"통신 테스트 에러 !");
          //Beep_3times();
        }
      Delay_ms(3000);

      TFT_string(11,25,White,Black,"                  "); // clear message
      TFT_string(14,18,White,Black,"                       ");
      TFT_string(14,21,White,Black,"                       ");
      Delay_ms(1000);
    }*/
}
