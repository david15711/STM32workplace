/* ============================================================================ */
/*	Exp21_1_loopback(1).c : CAN 2.0A(11비트 ID를 사용) 루프백 테스트	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2020.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void CAN1_RX0_IRQHandler(void);			/* CAN1 RX0 interrupt */
void CAN1_initialize(void);			/* initialize bxCAN CAN1 */
void CAN1_transmit(U32 ID, U32 DLC);		/* transmit CAN1 message with ID and data length */
void CAN1_receive(U08 FIFO);			/* receive CAN1 data from FIFO 0/1 */
void Tick(void);				/* tick for 100 us */

volatile unsigned char RX_flag = 0;
unsigned int  TX_ID = 100, TX_DLC;		// transmit information
unsigned int  RX_ID, RX_FMI, RX_DLC;		// receive information
unsigned char TX_data[8]={0,0,0,0,0,0,0,0};	// 8-byte transmit data
unsigned char RX_data[8]={0,0,0,0,0,0,0,0};	// 8-byte receive data

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void CAN1_RX0_IRQHandler(void)			/* CAN1 RX0 interrupt */
{
  CAN1_receive(0);				// receive data from FIFO 0
  RX_flag = 1;
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned char i, loop = 0;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  CAN1_initialize();				// initialize bxCAN CAN1

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp21_1.c    ");

  TFT_string(10,3,White,Black,"CAN 2.0A(11비트 ID)");
  TFT_string(0,5,White,Magenta,"   CAN 직렬통신(bxCAN)  루프백 테스트   ");
  TFT_string(2,10,Yellow,Black,"   <<송신측>>         <<수신측>>");
  TFT_string(2,13,  Cyan,Black,"ID     =           ID     = ");
  TFT_string(2,16,  Cyan,Black,"DLC    =           DLC    = ");
  TFT_string(2,19,  Cyan,Black,"데이터 =           데이터 = ");
  TFT_string(2,22,  Cyan,Black,"                   필터 인덱스 = ");
  Beep();
  Delay_ms(1000);

  while(1)
    { if(loop == 0)
        { TX_data[0] = 'C';			// 첫번째 테스트 데이터(8문자)
          TX_data[1] = 'A';
          TX_data[2] = 'N';
          TX_data[3] = ' ';
          TX_data[4] = 'T';
          TX_data[5] = 'e';
          TX_data[6] = 's';
          TX_data[7] = 't';
          TX_DLC = 8;
	  CAN1_transmit(TX_ID,TX_DLC);		// transmit CAN1 message with 8 data
	}
      else if(loop == 1)
	{ TX_data[0] = 'b';			// 두번째 테스트 데이터(5문자)
          TX_data[1] = 'x';
          TX_data[2] = 'C';
          TX_data[3] = 'A';
          TX_data[4] = 'N';
          TX_DLC = 5;
	  CAN1_transmit(TX_ID,TX_DLC);		// transmit CAN1 message with 5 data
	}
      loop ^= 0x01;

      TFT_string(11,19,White,Black,"        ");	// clear 8 transmit data
      TFT_string(30,19,White,Black,"        ");	// clear 8 receive data

      TFT_xy(11,13);				// display TX ID
      TFT_unsigned_decimal(TX_ID,0,3);
      TFT_xy(11,16);				// display TX data length
      TFT_unsigned_decimal(TX_DLC,0,1);
      TFT_xy(11,19);				// display TX data
      for(i = 0; i < TX_DLC; i++)
	TFT_English(TX_data[i]);

      LED_on();					// LED flash and tick sound
      Delay_ms(50);
      Tick();
      LED_off();

      if(RX_flag == 1)
        { RX_flag = 0;
          TFT_xy(30,13);			// display RX ID
          TFT_unsigned_decimal(RX_ID,0,3);
	  TFT_xy(30,16);			// display RX data length
          TFT_unsigned_decimal(RX_DLC,0,1);
	  TFT_xy(30,19);			// display RX data
          for(i = 0; i < RX_DLC; i++)
	    TFT_English(RX_data[i]);
          TFT_xy(35,22);			// display filter index
          TFT_unsigned_decimal(RX_FMI,0,1);
        }

      Delay_ms(1000);
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void CAN1_initialize(void)			/* initialize bxCAN CAN1 */
{
  RCC->APB1ENR |= 0x02000000;	                // enable CAN1 clock
  RCC->AHB1ENR |= 0x00000002;	                // enable GPIOB clock
  GPIOB->MODER &= 0xFFF0FFFF;			// PB9 = CAN1_TX, PB8 = CAN1_RX
  GPIOB->MODER |= 0x000A0000;
  GPIOB->AFR[1] &= 0xFFFFFF00;
  GPIOB->AFR[1] |= 0x00000099;

  CAN1->MCR &= 0xFFFFFFFD;			// exit sleep mode
  while((CAN1->MSR & 0x00000002) != 0x00000002);// wait until SLAK=1
  CAN1->MCR |= 0x00000001;			// enter initilization mode
  while((CAN1->MSR & 0x00000001) != 0x00000001);// wait until INAK=1
  CAN1->BTR = 0x0033000B;			// 54MHz/(11+1)/(3+3+3)=500kbps, SILM=LBKM=1
  CAN1->BTR |= 0xC0000000;			// BRP[9:0]=11,TS1[3:0]=3,TS2[2:0]=3,SJW[1:0]=0
  CAN1->MCR &= 0xFFFFFFFE;			// enter normal mode
  while((CAN1->MSR & 0x00000001) != 0x00000000);// wait until INAK=0
  CAN1->MCR |= 0x00000040;			// ABOM=1

  CAN1->FMR = 0x00000001;			// enter filter initialization mode
  CAN1->FFA1R = 0x00003F80;			// filter bank 0~6 = FIFO 0
  CAN1->FS1R = 0x0000007F;			// filter bank 0~6 = 32 bit
  CAN1->FM1R = 0x0000007F;			// filter bank 0~6 = list mode
  CAN1->sFilterRegister[0].FR1 = 0x00000000;	// set ID of filter number 0 and IDE=0
  CAN1->sFilterRegister[0].FR1 |= (TX_ID << 21);
  CAN1->sFilterRegister[0].FR2 = 0x00000000;	// set ID of filter number 1
  CAN1->FA1R = 0x00000001;			// filter bank 0 = active
  CAN1->FMR &= 0xFFFFFFFE;			// exit filter initialization mode

  CAN1->IER = 0x00000002;			// enable FIFO 0 message pending interrupt
  NVIC->ISER[0] = 0x00100000;			// enable (20)CAN1_RX0 interrupt
}

void CAN1_transmit(U32 ID, U32 DLC)		/* transmit CAN1 message with ID and data length */
{
  unsigned char TX_empty;

  TX_empty = 0xFF;
  while(TX_empty == 0xFF)			// check if transmit mailbox is empty
    { if((CAN1->TSR & 0x04000000) == 0x04000000)
        TX_empty = 0;
      else if((CAN1->TSR & 0x08000000) == 0x08000000)
        TX_empty = 1;
      else if((CAN1->TSR & 0x10000000) == 0x10000000)
        TX_empty = 2;
    }

  CAN1->sTxMailBox[TX_empty].TIR = 0x00000000;	// IDE=0, RTR=0
  CAN1->sTxMailBox[TX_empty].TIR |= (ID << 21);	// 11-bit ID(CAN 2.0A)

  CAN1->sTxMailBox[TX_empty].TDTR = DLC;	// data length

  CAN1->sTxMailBox[TX_empty].TDHR = ((U32)TX_data[7] << 24) | // data byte
                                    ((U32)TX_data[6] << 16) |
                                    ((U32)TX_data[5] <<  8) |
                                    ((U32)TX_data[4]);
  CAN1->sTxMailBox[TX_empty].TDLR = ((U32)TX_data[3] << 24) |
                                    ((U32)TX_data[2] << 16) |
                                    ((U32)TX_data[1] <<  8) |
                                    ((U32)TX_data[0]);

  CAN1->sTxMailBox[TX_empty].TIR |= 0x00000001;	// transmit request (TXRQ=1)
}

void CAN1_receive(U08 FIFO)			/* receive CAN1 data from FIFO 0/1 */
{
  if((CAN1->sFIFOMailBox[FIFO].RIR & 0x00000004) == 0x00000000) // get 11-bit ID
    RX_ID = (CAN1->sFIFOMailBox[FIFO].RIR >> 21) & 0x000007FF;
  else								// get 29-bit ID
    RX_ID = (CAN1->sFIFOMailBox[FIFO].RIR >>  3) & 0x1FFFFFFF;

  RX_FMI = (CAN1->sFIFOMailBox[FIFO].RDTR >> 8) & 0x000000FF;	// get filter match index
  RX_DLC = CAN1->sFIFOMailBox[FIFO].RDTR & 0x0000000F;		// get data length

  RX_data[0] = CAN1->sFIFOMailBox[FIFO].RDLR & 0x000000FF;	// get 8-byte data
  RX_data[1] = (CAN1->sFIFOMailBox[FIFO].RDLR >>  8) & 0x000000FF;
  RX_data[2] = (CAN1->sFIFOMailBox[FIFO].RDLR >> 16) & 0x000000FF;
  RX_data[3] = (CAN1->sFIFOMailBox[FIFO].RDLR >> 24) & 0x000000FF;
  RX_data[4] = CAN1->sFIFOMailBox[FIFO].RDHR & 0x000000FF;
  RX_data[5] = (CAN1->sFIFOMailBox[FIFO].RDHR >>  8) & 0x000000FF;
  RX_data[6] = (CAN1->sFIFOMailBox[FIFO].RDHR >> 16) & 0x000000FF;
  RX_data[7] = (CAN1->sFIFOMailBox[FIFO].RDHR >> 24) & 0x000000FF;

  if(FIFO == 0)
    CAN1->RF0R = 0x00000020;			// release FIFO 0
  else if(FIFO == 1)
    CAN1->RF1R = 0x00000020;			// release FIFO 1
}

void Tick(void)					/* tick for 100 us */
{ 
  GPIOC->BSRR = 0x00008000;   			// buzzer on
  Delay_us(100);				// delay 100 us
  GPIOC->BSRR = 0x80000000;			// buzzer off
}
