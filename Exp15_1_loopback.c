/* ============================================================================ */
/*		Exp15_1_loopback.c : RS-232C 직렬통신 루프백 테스트		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned char i, test_flag, RXD;
  unsigned char string[24] = { "OK-STM767 Loopback Test" };

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp15_1.c    ");

  TFT_string(4,4,White,Magenta," RS-232C 직렬통신 루프백 테스트 ");
  TFT_string(1,9,White,Black,"CN5 컨넥터에 RS-232C 케이블을 접속하고");
  TFT_string(6,12,White,Black,"2번핀과 3번핀을 연결하시오 !");
  TFT_string(2,18,Cyan, Black,"송신 문자 : ");
  TFT_string(2,21,Cyan, Black,"수신 문자 : ");
  Delay_ms(1000);

  GPIOB->MODER &= 0xFF0FFFFF;			// PB10 = USART3_TX, PB11 = USART3_RX   1010
  GPIOB->MODER |= 0x00A00000;
  GPIOB->AFR[1] &= 0xFFFF00FF;
  GPIOB->AFR[1] |= 0x00007700;                  //0111  = AF7 = USART3_TX, USART3_RX
  RCC->APB1ENR |= 0x00040000;			// enable USART3 clock

  USART3->CR1 = 0x0000000D;			// TE=RE=UE = 1, 8 data bit, oversampling by 16
  USART3->CR2 = 0x00000000;			// asynchronous mode, 1 stop bit
  USART3->CR3 = 0x00000000;			// 3 sampling bit
  USART3->BRR = 2813;				// 19200 bps = 54MHz/2812.5
  Delay_ms(1);
  RXD = USART3->RDR;				// dummy read

  while(1)
    { test_flag = 0;
      for(i = 0; i < 23; i++)
        { USART3->TDR = string[i];		// transmit test data
          TFT_xy(14+i,18);			// display transmitted test data
          TFT_color(Green,Black);
          TFT_English(string[i]);
          Delay_ms(5);

          if(USART3->ISR & 0x00000020) RXD = USART3->RDR; // receive test data
          else                         RXD = ' ';
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
    }
}
