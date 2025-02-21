/* ============================================================================ */
/*	Exp16_4_I2C.c : I2C 직렬통신에 의한 AT24C16 EEPROM의 액세스		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void Initialize_I2C1(void);			/* initialize I2C1 */
void AT24C16_write(U16 address,U08 byte);	/* write a byte to AT24C16 */
unsigned char AT24C16_read(U16 address);	/* read a byte from AT24C16 */

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned char byte, error;
  unsigned short address;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  Initialize_I2C1();				// initialize I2C1

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp16_4.c    ");
  Beep();

  TFT_string( 2, 5,White,Magenta, " I2C를 사용한 AT24C16 EEPROM 액세스 ");
  TFT_string(11,10,White,Black, "어드레스 = 0x0000");
  TFT_string( 7,14,White,Black, "메모리 쓰기 데이터 = 0x00");
  TFT_string( 7,17,White,Black, "메모리 읽기 데이터 = 0x00");

  error = 0;
  for(address = 0x0000; address <= 0x07FF; address++)
    { TFT_color(Yellow,Black);
      TFT_xy(22,10);
      TFT_0x_hexadecimal(address,4);
      TFT_color(Cyan,Black);

      TFT_xy(28,14);				// test 0x55
      TFT_0x_hexadecimal(0x55,2);

      AT24C16_write(address,0x55);
      byte = AT24C16_read(address);

      TFT_xy(28,17);
      TFT_0x_hexadecimal(byte,2);
      Delay_ms(200);

      if(byte != 0x55)				// if error, beep 3 times
        { error = 1;
	  Beep_3times();
          Delay_ms(3000);
        }

      TFT_xy(28,14);				// test 0xAA
      TFT_0x_hexadecimal(0xAA,2);

      AT24C16_write(address,0xAA);
      byte = AT24C16_read(address);

      TFT_xy(28,17);
      TFT_0x_hexadecimal(byte,2);
      Delay_ms(200);

      if(byte != 0xAA)				// if error, beep 3 times
        { error = 1;
	  Beep_3times();
          Delay_ms(3000);
        }
    }

  Beep();
  if(error == 0) TFT_string(9,22,Green,Black,"AT24C16 읽기/쓰기 OK !");
  else           TFT_string(8,22,Red,Black,"AT24C16 읽기/쓰기 에러 !");

  while(1);
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Initialize_I2C1(void)			/* initialize I2C1 */
{
  GPIOB->MODER &= 0xFFFF0FFF;			// PB7(I2C1_SDA),PB6(I2C1_SCL) = alternate function
  GPIOB->MODER |= 0x0000A000;
  GPIOB->AFR[0] &= 0x00FFFFFF;			// PB7 = I2C1_SDA, PB6 = I2C1_SCL
  GPIOB->AFR[0] |= 0x44000000;

  RCC->APB1ENR |= 0x00200000;			// enable I2C1 clock(= PCLK = 54MHz)

  I2C1->CR1 = 0x00000000;			// PE = 0
  I2C1->TIMINGR = 0x50430909;			// PRESC=5, SCLDEL=4, SDADEL=3, SCLH=9, SCLL=9
  I2C1->CR1 = 0x00000001;			// PE = 1
}

void AT24C16_write(U16 address,U08 byte)	/* write a byte to AT24C16 */
{
  I2C1->CR2 = 0x000200A0 + ((address >> 7) & 0x000E); // write operation, NBYTES=2, SADD=0xA0
  I2C1->CR2 |= 0x00002000;			// START condition
  while(!(I2C1->ISR & 0x00000002));		// TXIS = 1 ?

  I2C1->TXDR = address & 0x00FF;		// write address
  while(!(I2C1->ISR & 0x00000002));		// TXIS = 1 ?

  I2C1->TXDR = byte;				// write data
  while(!(I2C1->ISR & 0x00000040));		// TC = 1 ?
  I2C1->CR2 |= 0x00004000;			// STOP condition
  Delay_ms(5);                                  // delay 5 ms for twr time
}

unsigned char AT24C16_read(U16 address)		/* read a byte from AT24C16 */
{
  unsigned char byte;

  I2C1->CR2 = 0x000100A0 + ((address >> 7) & 0x000E); // write operation, NBYTES=1, SADD=0xA0
  I2C1->CR2 |= 0x00002000;			// START condition
  while(!(I2C1->ISR & 0x00000002));		// TXIS = 1 ?

  I2C1->TXDR = address & 0x00FF;		// write address
  while(!(I2C1->ISR & 0x00000040));		// TC = 1 ?

  I2C1->CR2 = 0x000104A0 + ((address >> 7) & 0x000E); // read operation, NBYTES=1, SADD=0xA0
  I2C1->CR2 |= 0x00002000;			// START condition
  while(!(I2C1->ISR & 0x00000004));		// RXNE = 1 ?
  byte = I2C1->RXDR & 0x000000FF;		// read data
  while(!(I2C1->ISR & 0x00000040));		// TC = 1 ?

  I2C1->CR2 |= 0x00004000;			// STOP condition
  Delay_us(10);

  return byte;                                  // return data
}
