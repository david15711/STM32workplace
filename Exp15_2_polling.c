/* ============================================================================ */
/*	Exp15_2_polling.c : RS-232C 직렬통신에서 폴링 방식에 의한 송수신	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

unsigned char RX3_char(void);			/* receive a character by USART3 */
void TX3_char(U08 data);			/* transmit a character by USART3 */
void TX3_string(U08 *string);			/* transmit a string by USART3 */

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned char RXD, count;
  unsigned short color;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp15_2.c    ");

  TFT_string( 2, 4,White,Magenta," RS-232C 직렬통신(폴링 방식 송수신) ");
  TFT_string(17, 8,White,Blue," 송신 ");
  TFT_string( 9,11,White,Black,"KEY1~KEY4를 누르시오 !");
  TFT_string(17,16,White,Blue," 수신 ");
  Line(0,156,319,156,White);
  Line(0,212,319,212,White);

  GPIOB->MODER &= 0xFF0FFFFF;			// PB10 = USART3_TX, PB11 = USART3_RX
  GPIOB->MODER |= 0x00A00000;
  GPIOB->AFR[1] &= 0xFFFF00FF;
  GPIOB->AFR[1] |= 0x00007700;
  RCC->APB1ENR |= 0x00040000;			// enable USART3 clock

  USART3->CR1 = 0x0000000D;			// TE=RE=UE = 1, 8 data bit, oversampling by 16
  USART3->CR2 = 0x00000000;			// asynchronous mode, 1 stop bit
  USART3->CR3 = 0x00000000;			// 3 sampling bit
  USART3->BRR = 2813;				// 19200 bps = 54MHz/2812.5
  Delay_ms(1);
  RXD = USART3->RDR;				// dummy read

  count = 0;					// initialize received data display
  color = Cyan;
  TFT_xy(0,20);
  TFT_color(color,Black);

  while(1)
    { switch(Key_input())                       // key input
        { case KEY1 : TX3_string("  KEY1 is OK !  ");
                      TX3_char(0x0D); TX3_char(0x0A);
                      break;
          case KEY2 : TX3_string("  KEY2 is OK !  ");
                      TX3_char(0x0D); TX3_char(0x0A);
                      break;
          case KEY3 : TX3_string("  KEY3 is OK !  ");
                      TX3_char(0x0D); TX3_char(0x0A);
                      break;
          case KEY4 : TX3_string("  KEY4 is OK !  ");
                      TX3_char(0x0D); TX3_char(0x0A);
                      break;
          default:    break;
        }

      RXD = RX3_char();				// receive data if any
      if((RXD >= 0x20) && (RXD <= 0x7F))
        { TFT_English(RXD);			// display a character
          count++;				// 120 character OK ?
          if(count >= 120)			// if yes, go home
            { count = 0;
	      TFT_xy(0,20);
              color ^= 0xFFFF;
              TFT_color(color,Black);
            }
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

unsigned char RX3_char(void)			/* receive a character by USART3 */
{
  if(USART3->ISR & 0x00000020)   		// if RXNE=1, return with a received character
    return USART3->RDR;
  else						// if RXNE=0, return with 0x00
    return 0x00;
}

void TX3_char(U08 data)				/* transmit a character by USART3 */
{
  while(!(USART3->ISR & 0x00000080));		// wait until TXE = 1
  USART3->TDR = data;
}

void TX3_string(U08 *string)			/* transmit a string by USART3 */
{
  while(*string != '\0')
    { TX3_char(*string);
       string++;
    }
}
