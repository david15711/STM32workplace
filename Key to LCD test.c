#include "stm32f767xx.h"
#include "OK-STM767.h"

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

int main(void){
  Initialize_MCU();
  RCC->AHB1ENR |= 0x0000001C;			// port C, D, E clock enable
  GPIOC->MODER &= 0x3FFFFCFF;			// set PC15(buzzer) and PC4(LED) to output
  GPIOC->MODER |= 0x40000100;
  GPIOC->ODR &= 0xFFFF7FEF;			// PC15(buzzer) = PC4(LED) = 0
  GPIOD->MODER &= 0xFFFF0000;			// set PD7~0 to output
  GPIOD->MODER |= 0x00005555;
  GPIOE->MODER &= 0xFFFFF3F3;			// set PE5(LCD_E), PE1(TFT_RS) to output
  GPIOE->MODER |= 0x00000404;
  GPIOE->ODR &= 0xFFFFFFDD;			// PE5(LCD_E) = PE1(TFT_RS) = 0
  Delay_ms(50);
  Initialize_LCD();
  
  LCD_string(0x80, " Press Key");
  LCD_string(0xC7, "0");
  unsigned int sum = 0;
  unsigned char key;
  while( !(sum > 20) ){
    key = Key_input_silent();
    switch(key){
      case KEY1:
        sum += 1;
        LCD_string(0xC0, " KEY 1 ");
        printSum(sum);
        break;
      case KEY2:
        sum += 2;
        LCD_string(0xC0, " KEY 2 ");
        printSum(sum);
        break;
      case KEY3:
        sum += 3;
        LCD_string(0xC0, " KEY 3 ");
        printSum(sum);
        break;
      case KEY4:
        sum += 4;
        LCD_string(0xC0, " KEY 4 ");
        printSum(sum);
        break;
      default:
        break;
    }
  }
  LCD_string(0xC0, " Stop sum > 20");
  
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