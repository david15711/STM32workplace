#include "stm32f767xx.h"
#include "OK-STM767.h"

char* stringCopy(char* s1, char* s2);

int main()
{
  unsigned int i, command;
  char string[17];
  Initialize_MCU();     //216MHz
    
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
    
    stringCopy(string, " 60191859   KJN ");
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
    Delay_ms(2000);
    
    LCD_command(0x01);  //clear all
    Delay_ms(1000);
    LCD_string(0x80," MJU Electronics");
    LCD_string(0xC0," 60191859    KJN");
    Delay_ms(1000);
    LCD_command(0x01);  //clear all
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