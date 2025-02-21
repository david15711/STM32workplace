/* ============================================================================ */
/*	Exp21_5_polling(100).c : CAN 2.0A 폴링 수신 - 100번 ID측 프로그램	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2020.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void CAN1_initialize(void);			/* initialize bxCAN CAN1 */
void CAN1_transmit(U32 ID, U32 DLC);		/* transmit CAN1 message with ID and data length */
void CAN1_receive(U08 FIFO);			/* receive CAN1 data from FIFO 0/1 */
void Tick(void);				/* tick for 100 us */
void ADC_initialize(void);			/* initialize ADC */
void ADC_read(void);				/* read ADC */

unsigned int  TX_ID = 100, TX_DLC = 8;		// transmit information(송신측 ID=100)
unsigned int  RX_ID = 200, RX_DLC = 8;		// receive information(수신측 ID=200)
unsigned short TX_data[4]={0,0,0,0};		// 4-data(8-byte) transmit data
unsigned short RX_data[4]={0,0,0,0};		// 4-data(8-byte) receive data

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  CAN1_initialize();				// initialize bxCAN CAN1

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp21_5.c    ");

  TFT_string(10,3,White,Black,"CAN 2.0A(11비트 ID)");
  TFT_string(0,5,White,Magenta,"   CAN 직렬통신(bxCAN)  송수신 테스트   ");
  TFT_string(3,9,Yellow,Black,"<<나의 데이터>>   <<상대측 데이터>>");
  TFT_string(3,12,Cyan, Black," ID   = 000         ID   = 000");
  TFT_string(3,16,Cyan, Black," 5.0V = 0.00V       5.0V = 0.00V");
  TFT_string(3,19,Cyan, Black," 3.3V = 0.00V       3.3V = 0.00V");
  TFT_string(3,22,Cyan, Black," VR1  = 0.00V       VR1  = 0.00V");
  TFT_string(3,25,Cyan, Black," LM35 = 00.0 C      LM35 = 00.0 C");
  TFT_xy(15,25);
  TFT_English(0x7F);
  TFT_xy(34,25);
  TFT_English(0x7F);
  Beep();
  Delay_ms(1000);

  ADC_initialize();				// initialize ADC

  while(1)
    { ADC_read();				// read ADC
      CAN1_transmit(TX_ID,TX_DLC);		// transmit CAN1 message with 8 data

      TFT_color(White,Black);
      TFT_xy(11,12);				// display TX ID
      TFT_unsigned_decimal(TX_ID,0,3);
      TFT_xy(30,12);				// display RX ID
      TFT_unsigned_decimal(RX_ID,0,3);

      TFT_xy(11,16);				// display TX_data[0] = 5.0V
      TFT_unsigned_float((float)TX_data[0]*2.*3.3/4095., 1, 2);
      TFT_xy(11,19);				// display TX_data[1] = 3.3V
      TFT_unsigned_float((float)TX_data[1]*2.*3.3/4095., 1, 2);
      TFT_xy(11,22);				// display TX_data[2] = VR1
      TFT_unsigned_float((float)TX_data[2]*3.3/4095., 1, 2);
      TFT_xy(11,25);				// display TX_data[3] = LM35
      TFT_unsigned_float((float)TX_data[3]*330./4095., 2, 1);

      LED_on();					// LED flash and tick sound
      Delay_ms(50);
      Tick();
      LED_off();

      if((CAN1->RF0R & 0x00000003) != 0)	// check if FIFO 0 has message pending
        { CAN1_receive(0);			// if yes, receive data from FIFO 0

	  TFT_xy(30,16);			// display RX_data[0] = 5.0V
          TFT_unsigned_float((float)RX_data[0]*2.*3.3/4095., 1, 2);
          TFT_xy(30,19);			// display RX_data[1] = 3.3V
          TFT_unsigned_float((float)RX_data[1]*2.*3.3/4095., 1, 2);
          TFT_xy(30,22);			// display RX_data[2] = VR1
          TFT_unsigned_float((float)RX_data[2]*3.3/4095., 1, 2);
          TFT_xy(30,25);			// display RX_data[3] = LM35
          TFT_unsigned_float((float)RX_data[3]*330./4095., 2, 1);
        }

      Delay_ms(700);
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
  CAN1->BTR = 0x0033000B;			// 54MHz/(11+1)/(3+3+3)=500kbps, SJW[1:0]=0
  CAN1->MCR &= 0xFFFFFFFE;			// enter normal mode
  while((CAN1->MSR & 0x00000001) != 0x00000000);// wait until INAK=0
  CAN1->MCR |= 0x00000040;			// ABOM=1

  CAN1->FMR = 0x00000001;			// enter filter initialization mode
  CAN1->FFA1R = 0x00003F80;			// filter bank 0~6 = FIFO 0
  CAN1->FS1R = 0x0000007F;			// filter bank 0~6 = 32 bit
  CAN1->FM1R = 0x0000007F;			// filter bank 0~6 = list mode
  CAN1->sFilterRegister[0].FR1 = 0x00000000;	// set ID of filter number 0 and IDE=0
  CAN1->sFilterRegister[0].FR1 |= (RX_ID << 21);
  CAN1->sFilterRegister[0].FR2 = 0x00000000;	// set ID of filter number 1
  CAN1->FA1R = 0x00000001;			// filter bank 0 = active
  CAN1->FMR &= 0xFFFFFFFE;			// exit filter initialization mode
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

  CAN1->sTxMailBox[TX_empty].TDHR = ((U32)TX_data[3] << 16) | // data byte
                                    ((U32)TX_data[2]);
  CAN1->sTxMailBox[TX_empty].TDLR = ((U32)TX_data[1] << 16) |
                                    ((U32)TX_data[0]);

  CAN1->sTxMailBox[TX_empty].TIR |= 0x00000001;	// transmit request (TXRQ=1)
}

void CAN1_receive(U08 FIFO)			/* receive CAN1 data from FIFO 0/1 */
{
  unsigned int ID, DLC;

  ID = (CAN1->sFIFOMailBox[FIFO].RIR >> 21) & 0x000007FF;// get 11-bit ID
  DLC = CAN1->sFIFOMailBox[FIFO].RDTR & 0x0000000F;	// get data length

  if((ID == RX_ID) && (DLC == RX_DLC))		// if message is OK, get 4 data
    { RX_data[0] = CAN1->sFIFOMailBox[FIFO].RDLR & 0x0000FFFF;
      RX_data[1] = (CAN1->sFIFOMailBox[FIFO].RDLR >> 16) & 0x0000FFFF;
      RX_data[2] = CAN1->sFIFOMailBox[FIFO].RDHR & 0x0000FFFF;
      RX_data[3] = (CAN1->sFIFOMailBox[FIFO].RDHR >> 16) & 0x0000FFFF;
    }
  else						// if message is not OK, clear 4 data
    { RX_data[0] = 0;
      RX_data[1] = 0;
      RX_data[2] = 0;
      RX_data[3] = 0;
    }

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

void ADC_initialize(void)			/* initialize ADC */
{
  GPIOA->MODER |= 0x000000FF;			// use ADC0~ADC3
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00800000;			// enable channel 17~18, ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000249;			// sampling time of channel 0~3 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;			// total regular channel number = 1
}

void ADC_read(void)				/* read ADC */
{
  ADC1->SQR3 = 0x00000000;			// channel 0 (+5.0V)
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
  TX_data[0] = ADC1->DR;

  ADC1->SQR3 = 0x00000001;			// channel 1 (+3.3V)
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
  TX_data[1] = ADC1->DR;

  ADC1->SQR3 = 0x00000002;			// channel 2 (VR1)
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
  TX_data[2] = ADC1->DR;

  ADC1->SQR3 = 0x00000003;			// channel 3 (LM35DZ)
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
  TX_data[3] = ADC1->DR;
}
