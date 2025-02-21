#include "stm32f767xx.h"

char* stringCopy(char* s1, char* s2);
void Delay_us(unsigned int time_us);
void Delay_ms(unsigned int time_ms);
void blinkTheLED(void);
void Initialize_LCD(void);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_string(unsigned char command, unsigned char *string);
void SystemInit(void);
void Initialize_MCU(void);

int main()
{
  unsigned int i, command;
  char string[17];
  
  Initialize_MCU();
  Delay_ms(50);
  RCC->AHB1ENR |= 0x0000001C;			// port C, D, E clock enable

  GPIOC->MODER &= 0x3FFFFCFF;			// set PC15(buzzer) and PC4(LED) to output
  GPIOC->MODER |= 0x40000100;
  GPIOC->ODR &= 0xFFFF7FEF;			// PC15(buzzer) = PC4(LED) = 0

  GPIOD->MODER &= 0xFFFF0000;			// set PD7~0 to output
  GPIOD->MODER |= 0x00005555;

  GPIOE->MODER &= 0xFFFFF3F3;			// set PE5(LCD_E), PE1(TFT_RS) to output
  GPIOE->MODER |= 0x00000404;
  GPIOE->ODR &= 0xFFFFFFDD;			// PE5(LCD_E) = PE1(TFT_RS) = 0

  Delay_ms(50);                                 // wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  
  while(1){
    stringCopy(string, " MJU Electronics");
    LCD_command(0x80);
    i = 0;
    while( i<16 ){
      LCD_data(string[i]);
      Delay_ms(200);
      i++;
    }
    
    stringCopy(string, " 60191859 Kim JN");
    LCD_command(0xC0);
    i = 0;
    while( string[i] != '\0' ){
      LCD_data(string[i]);
      Delay_ms(200);
      i++;
    }
    Delay_ms(2000);
    
    
    stringCopy(string, " MicroProcessor ");
    i=0;
    while(i<16){
      command = 0x80 | i; 
      LCD_command(command);
      LCD_data(' ');
      if( i >= 3 ){
        LCD_command( command - 3 );
        LCD_data(string[i-3]);
      }
      Delay_ms(200);
      i++;
    }
    LCD_data(string[13]);
    Delay_ms(200);
    LCD_data(string[14]);
    Delay_ms(200);
    LCD_data(string[15]);
    Delay_ms(200);
    
    stringCopy(string, " Application    ");
    i=0;
    while(i<16){
      command = 0xC0 | i; 
      LCD_command(command);
      LCD_data(' ');
      if( i >= 3 ){
        LCD_command( command - 3 );
        LCD_data(string[i-3]);
      }
      Delay_ms(200);
      i++;
    }
    LCD_data(string[13]);
    Delay_ms(200);
    LCD_data(string[14]);
    Delay_ms(200);
    LCD_data(string[15]);
    Delay_ms(200);
    
    LCD_command(0x01);
    Delay_ms(1000);
    LCD_string(0x80, " MJU Electronics");
    LCD_string(0xC0, " 60191859 Kim JN");
    Delay_ms(1000);
    LCD_command(0x01);
    Delay_ms(1000);

    blinkTheLED();
    Delay_ms(100);
    blinkTheLED();
    Delay_ms(100);
    blinkTheLED();
    Delay_ms(100);
    
  }
}

char* stringCopy(char* s1, char* s2){
  while( *s2 != '\0'){
    *s1 = *s2;
    s1++;
    s2++;
  }
  *s1 = '\0';
  return s1;
}

void Delay_us(unsigned int time_us)		// time delay for us in 216MHz
{
  register unsigned int i;

  for(i = 0; i < time_us*2; i++)			// 3 cycle
    { 
       asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");       //
      asm("NOP");
      asm("NOP");
      asm("NOP");
    }
}

void Delay_ms(unsigned int time_ms)		// time delay for ms in 16MHz
{
  register unsigned int i;

  for(i = 0; i < time_ms; i++)
    Delay_us(1000);
}

void blinkTheLED(void){
  *(int*)0x40020818 = 0x00000010;		// LED on, GPIOC->BSRR
  Delay_ms(50);
  *(int*)0x40020818 = 0x00100000;		// LED off
}


void Initialize_LCD(void)			// initialize text LCD module
{
  RCC->AHB1ENR |= 0x00000018;			// enable clock of port D,E

  GPIOD->MODER &= 0xFFFF0000;			// 포트D 설정
  GPIOD->MODER |= 0x00005555;
  GPIOD->ODR &= 0xFFFFFF00;			// TFT_D7~TFT_D0 = 0
  GPIOD->OSPEEDR &= 0xFFFF0000;			// 50MHz medium speed
  GPIOD->OSPEEDR |= 0x00005555;

  GPIOE->MODER &= 0xFFFFF3F3;			// 포트E 설정
  GPIOE->MODER |= 0x00000404;
  GPIOE->ODR &= 0xFFFFFFDD;			// LCD_E = TFT_RS = 0
  GPIOE->OSPEEDR &= 0xFFFFF3F3;			// 50MHz medium speed
  GPIOE->OSPEEDR |= 0x00000404;

  LCD_command(0x38);				// function set(8 bit, 2 line, 5x7 dot)
  LCD_command(0x0C);				// display control(display ON, cursor OFF)
  LCD_command(0x06);				// entry mode set(increment, not shift)
  LCD_command(0x01);				// clear display
  Delay_ms(2);
}

void LCD_command(unsigned char command)			// write a command(instruction) to text LCD
{
  GPIOE->BSRR = 0x00220000;			// E = 0, Rs = 0
  GPIOD->ODR = command;                  	// output command
  Delay_us(1);
  GPIOE->BSRR = 0x00000020;			// E = 1
  Delay_us(1);
  GPIOE->BSRR = 0x00200000;			// E = 0
  Delay_us(50);
}

void LCD_data(unsigned char data)				// display a character on text LCD
{
  GPIOE->BSRR = 0x00200002;			// E = 0, Rs = 1
  GPIOD->ODR = data;				// output data
  Delay_us(1);
  GPIOE->BSRR = 0x00000020;			// E = 1
  Delay_us(1);
  GPIOE->BSRR = 0x00200000;			// E = 0
  Delay_us(50);
}

void LCD_string(unsigned char command, unsigned char *string)	// display a string on LCD
{
  LCD_command(command);				// start position of string
  while(*string != '\0')			// display string
    { LCD_data(*string);
       string++;
    }
}

void SystemInit(void)
{
  asm volatile("NOP");
}

void Initialize_MCU(void)			// initialize STM32F767VGT6 MCU
{
// (1) 명령 캐시 및 데이터 캐시 설정
  
  SCB_EnableICache();				// enable L1 instruction cache
  SCB_EnableDCache();				// enable L1 data cache

// (2) ART 가속기, 프리페치 버퍼, 웨이트 사이클 설정
  FLASH->ACR = 0x00000307;			// 7 waits, enable ART accelerator and prefetch

// (3) HSE 및 PLL 설정(시스템 클록 SYSCLK = 216MHz)
  RCC->CR |= 0x00010001;			// HSE on, HSI on
  while((RCC->CR & 0x00000002) == 0);		// wait until HSIRDY = 1
  RCC->CFGR = 0x00000000;			// SYSCLK = HSI
  while((RCC->CFGR & 0x0000000C) != 0);		// wait until SYSCLK = HSI

  RCC->CR = 0x00010001;				// PLL off, HSE on, HSI on
  RCC->PLLCFGR = 0x09403608;			// SYSCLK = HSE*PLLN/PLLM/PLLP = 16MHz*216/8/2 = 216MHz
						// PLL48CK = HSE*PLLN/PLLM/PLLQ = 16MHz*216/8/9 = 48MHz
  RCC->CR = 0x01010001;				// PLL on, HSE on, HSI on
  while((RCC->CR & 0x02000000) == 0);		// wait until PLLRDY = 1

// (4) 오버드라이브 설정
  RCC->APB1ENR |= 0x10000000;			// 전원모듈 클록(PWREN = 1)
  PWR->CR1 |= 0x00010000;			// over-drive enable(ODEN = 1)
  while((PWR->CSR1 & 0x00010000) == 0);		// ODRDY = 1 ?
  PWR->CR1 |= 0x00020000;			// over-drive switching enable(ODSWEN = 1)
  while((PWR->CSR1 & 0x00020000) == 0);		// ODSRDY = 1 ?

// (5) 주변장치 클록 설정(APB1CLK = APB2CLK = 54MHz)
  RCC->CFGR = 0x3040B402;			// SYSCLK = PLL, AHB = 216MHz, APB1 = APB2 = 54MHz
						// MCO1 = HSE, MCO2 = SYSCLK/4 (timer = 108MHz)
  while((RCC->CFGR & 0x0000000C) != 0x00000008);// wait until SYSCLK = PLL
  RCC->CR |= 0x00080000;			// CSS on

// (6) I/O 보상 설정
  RCC->APB2ENR |= 0x00004000;			// 주변장치 클럭(SYSCFG = 1)
  SYSCFG->CMPCR = 0x00000001;			// enable compensation cell

// (7) 키트의 주변장치에 클록을 공급
  RCC->AHB1ENR |= 0x0000001F;			// 포트 A,B,C,D,E에 클럭 공급
  RCC->AHB2ENR |= 0x00000000;			// disable special peripherals
  RCC->AHB3ENR |= 0x00000000;			// disable FSMC
  RCC->APB1ENR |= 0x2024C000;			// 주변장치 클럭(DAC=I2C1=USART3=SPI3=SPI2=1)
  RCC->APB2ENR |= 0x00000100;			// 주변장치 클럭(ADC1 = 1)

// (8) 키트에 맞게 GPIO를 초기화
  GPIOA->MODER  = 0xA956FFFF;			// 포트A 설정
  GPIOA->AFR[1] = 0x00000000;			// PA15 = JTDI, PA14 = JTCK, PA13 = JTMS, PA8 = MCO1
  GPIOA->AFR[0] = 0x00000000;			// PA7/6 = ADC12_IN7/6, PA5/4 = DAC_OUT2/1
  GPIOA->ODR = 0x00000000;			// PA3~0 = ADC123_IN3~0
  GPIOA->OSPEEDR = 0xFC030000;			// JTDI = JTCK = JTMS = MCO1 = 180MHz very high speed

  GPIOB->MODER  = 0xA9AAA294;			// 포트B 설정
  GPIOB->AFR[1] = 0x55507799;			// PB15 = SPI2_MOSI, PB14 = SPI2_MISO, PB13 = SPI2_SCK
   						// PB11 = USART3_RX, PB10 = USART3_TX, PB9 = CAN1_TX, PB8 = CAN1_RX
  GPIOB->AFR[0] = 0x44000000;			// PB7 = I2C1_SDA, PB6 = I2C1_SCL, PB4 = NJTRST, PB3 = JTDO
  GPIOB->ODR = 0x00001000;			// -TS_CS = 1
  GPIOB->OTYPER = 0x000000C0;			// PB7(I2C1_SCL) = PB6(I2C1_SDA) = open drain
  GPIOB->OSPEEDR = 0x000000C0;			// JTDO = 180MHz very high speed

  GPIOC->MODER  = 0x46A91500;			// 포트C 설정
  GPIOC->AFR[1] = 0x00066600;			// PC12 = SPI3_MOSI, PC11 = SPI3_MISO, PC10 = SPI3_SCK, PC9 = MCO2
  GPIOC->AFR[0] = 0x00000000;
  GPIOC->ODR = 0x00002160;			// -SD_CS = -MP3_RESET = -MP3_DCS = -MP3_CS = 1, BUZZER = LED = 0
  GPIOC->OSPEEDR = 0x000C0000;			// MCO2 = 180MHz very high speed

  GPIOD->MODER  = 0x55555555;			// 포트D 설정
  GPIOD->ODR = 0x00000000;			// TFT_D15~TFT_D0 = 0
  GPIOD->OSPEEDR = 0x55555555;			// TFT-LCD signal = 50MHz medium speed

  GPIOE->MODER  = 0x6AAA5555;			// 포트E 설정, -RTC_CS = 1, LCD_E = 0
  GPIOE->AFR[1] = 0x05551111;			// PE14 = SPI4_MOSI, PE13 = SPI4_MISO, PE12 = SPI4_SCK
  GPIOE->AFR[0] = 0x00000000;			// PE11/10/9/8 = TIM1_CH2/CH2N/CH1/CH1N
  GPIOE->ODR = 0x0000800D;			// -RTC_CS = -TFT_RESET = -TFT_CS = -TFT_WR = 1
  GPIOE->OSPEEDR = 0x00000455;			// LCD and TFT-LCD signal = 50MHz medium speed
}
