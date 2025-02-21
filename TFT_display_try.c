#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"
#include "startup_stm32f767xx.s"

unsigned int password[6] = {KEY1,0x0E,0x0E,0x0E,0x0E,0x0E};
unsigned int passwordTemp[6] = {0};
unsigned int i = 0, success = 0, key, menu_cursor = 1;

unsigned int sayPassword(void){
  success = 0;
  TFT_clear_screen();
  i = 0;
  TFT_string_large(1,8,White,Black,"암호  입력");
  TFT_string_large(1,14,White,Black," _  _  _  _  _  _ ");
  while(i<6){
    //debouncing
    while( (GPIOC->IDR & 0x0000000F) == no_key);
    key = (GPIOC->IDR & 0x0000000F);
    if(key == 0x0F) continue;
    Delay_ms(100);
    while( (GPIOC->IDR & 0x0000000F) != no_key);
    
    passwordTemp[i] = key;
    TFT_xy(1,20);
    TFT_0x_hexadecimal_large(key,4);
    TFT_xy(2+3*i,14);
    TFT_English_large(0x2A);
    i++;
  }
  for(int i = 0;i<6;i++){
    if ( passwordTemp[i] != password[i] ){
      TFT_string_large(1,8,White,Black,"암호 틀림  ");
      
      GPIOC->BSRR = 0x00000010;
      Delay_ms(30);
      GPIOC->BSRR = 0x00100000;
      Delay_ms(30);
      GPIOC->BSRR = 0x00000010;
      Delay_ms(30);
      GPIOC->BSRR = 0x00100000;
      Delay_ms(30);
      GPIOC->BSRR = 0x00000010;
      Delay_ms(30);
      GPIOC->BSRR = 0x00100000;
      Delay_ms(200);
      key = 0x0F;
      
      return 0;
      }
  }
  key = 0x0F;
  return 1;
}

void setPassword(void){
  while( (GPIOC->IDR & 0x0000000F) != no_key);
  if( !sayPassword() ){
    return;
  } else {
    TFT_string_large(1,8,White,Black,"새 암호 입력");
    i = 0;
    TFT_string_large(1,14,White,Black," _  _  _  _  _  _ ");
    while(i<6){
      while( (GPIOC->IDR & 0x0000000F) == no_key);
      key = (GPIOC->IDR & 0x0000000F);
      if(key == 0x0F) continue;
      Delay_ms(100);
      while( (GPIOC->IDR & 0x0000000F) != no_key);
      password[i] = key;
      TFT_xy(2+3*i,14);
      TFT_English_large(0x2A);          //*
      i++;
    }
  TFT_string_large(1,8,White,Black,"비밀번호 변경 완료");
  Delay_ms(3000);
  success = 1;
  key = 0x0F;
  }
}

void Display_screen(void)			/* display test screen */
{

  TFT_clear_screen();				// clear screen
  TFT_landscape_mode();				// landscape mode

  TFT_string( 0, 0,White,Black,"메인 메뉴");
  TFT_string( 0, 3,White,   Black,"   (1) 보안 모드 진입");
  TFT_string( 0, 6,White, Black,"   (2) 암호 재설정");
  TFT_string( 0,28,White,   Black,"KEY1(실행)");
  TFT_string(20,28,White,   Black," KEY3( ) ");
  TFT_string(31,28,White,   Black," KEY4( ) ");
  TFT_string( 5,28,Red,     Black,"실행");
  TFT_xy(26,28); TFT_English(0x18);
  TFT_xy(37,28); TFT_English(0x19);
}
void Display_Cursor(U08 _cursor) {
  TFT_string(0, 3,Cyan,   Black,"  ");
  TFT_string(0, 6,Yellow, Black,"  ");
  TFT_string(0, 9,Cyan,   Black,"  ");
  TFT_string(0,12,Yellow, Black,"  ");
  TFT_string(0,15,Cyan,   Black,"  ");
  TFT_string(0,18,Yellow, Black,"  ");
  TFT_string(0,21,Cyan,   Black,"  ");
  TFT_string(0,24,Yellow, Black,"  ");
  TFT_string(0,_cursor*3, White,Black,">>");
}
/*
void secure_mode(void){
  if( 조건 만족) {
    sayPassword();
    타이머 on;
    타이머 인터럽트 on;
    타이머 15초 세팅;
    


  }
  
  
}
*/

void TIM1_UP_TIM10_IRQHandler(void)	        /* TIM1 interrupt function (2Hz)*/
{
  TIM1->SR = 0x0000;				// clear pending bit of TIM1 interrupt
  
  
  
  
  
  
}



int main(void){
  Initialize_MCU();
  Delay_ms(50);					// wait for system stabilization
  Initialize_TFT_LCD();	
  RCC->APB2ENR |= 0x00000001;			// enable SYSCFG	
  
  TIM1->PSC = 1079;				// 108MHz/(1079+1) = 100kHz
  TIM1->ARR = 49999;				// 100kHz/(49999+1) = 2Hz
  TIM1->DIER = 0x0001;				// enable update interrupt
  TIM1->CNT = 0;				// clear counter
  TIM1->CR1 = 0x0005;				// enable TIM1 and update event
  NVIC->ISER[0] |= 0x02000000;			// enable (25)TIM1 interrupt
  
  LCD_command(0x01);
  TFT_string_large(1,4,Cyan,   Black,"  OK-STM767 키트  V1.0  ");
  TFT_string_large(1,8,White,Magenta," 하드웨어 진단 프로그램 ");
  TFT_string(6,15,Cyan,   Black," 도서출판 Ohm사 / R&C Kit사 ");
  TFT_string(6,21,White,  Black,"    2017년 01월 01일(일)    ");
  TFT_string(6,24,Green,  Black,"        윤덕용  작성        ");
  Delay_ms(3000);
  
  while(!success){      //성공적으로 설정할때까지 반복
    setPassword();
  }
     
  Display_screen();
  Display_Cursor(menu_cursor);
  
  while(1) {
    while( (GPIOC->IDR & 0x0000000F) != no_key);         //debouncing
    while(key == no_key){			// wait key input
      key = Key_input();
    }
      if(key == KEY3)				// if KEY3, decrenment cursor number
       { key = no_key;
         menu_cursor--;
         if(menu_cursor == 0) menu_cursor = 2;
         Display_Cursor(menu_cursor);
       }
      else if(key == KEY4)			// if KEY4, increnment cursor number
       { key = no_key;
         menu_cursor++;
         if(menu_cursor > 2) menu_cursor = 1;
         Display_Cursor(menu_cursor);
       }
      else if(key == KEY2)			// if KEY2, ignore and skip
       key = no_key;
      else if(key == KEY1)			// if KEY1, run function number
       { key = no_key;
         if(menu_cursor == 1) ;//secure_mode();
         else if(menu_cursor == 2) setPassword();

         Display_screen();			// display cursor screen
         Display_Cursor(menu_cursor);		// display cursor number
       }
    }
}