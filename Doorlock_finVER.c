#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"
#include "startup_stm32f767xx.s"
#include <stdio.h>

unsigned int password[6] = {KEY1,0x0E,0x0E,0x0E,0x0E,0x0E};
unsigned int passwordTemp[6] = {0};
unsigned int i = 0, success = 0, key, menu_cursor = 1;
unsigned int password_wrong = 0, time_out = 0;           //time_out = 15sec count
unsigned int time = 0, time_systick = 0;       //time = tim2 sonic counter, time_systick =systick 3sec
unsigned int secure_mode_clear = 0, ADC_result;
unsigned char RXD;

inline void send_c(void){
  Delay_ms(10);
  UART7->TDR = 'c';
  while(!(UART7->ISR & 0x80));
}

inline void send_s(void){
  Delay_ms(10);
  UART7->TDR = 's';
  while(!(UART7->ISR & 0x80));
}

inline void send_a(void){
  Delay_us(50);
  UART7->TDR = 'a';
  while(!(UART7->ISR & 0x80));
}

void TIM1_UP_TIM10_IRQHandler(void)           /* TIM1 interrupt function (100Hz)*/
{
  TIM1->SR = 0x0000;                // clear pending bit of TIM1 interrupt
  time++;                                //time 1500�Ǹ� 15��
 
  if(((time % 100) == 0) && (time < 1500)){
    TFT_color(Red, Black);
    TFT_xy(36,28);
    TFT_unsigned_decimal(15 - (time/100), 0, 2);
    TFT_color(White,Black);
  }
  else if(time == 1500){
    //STM���� ���� ���� ���
    TFT_string(30,28, White,Black, "�ð� �ʰ�");
    time_out = 1;
    Delay_ms(1000);
  }
  //ADC running
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));            // wait for end of conversion EOC
  ADC_result = ADC1->DR;
  if(ADC_result  >= 3072){
    send_s();
  }
}

void SysTick_Handler(void){
    time_systick++;   
    for(int i = 0 ; i < 2 ; i++){
        if(time_systick == ((i + 1) * 1000) ){
            TFT_color(White,Black);
            TFT_xy(8,10);
            TFT_unsigned_decimal(2 - i, 0, 1);
            break;
        }
    }
}

float UltraSonic_running(void){
      GPIOE->BSRR |= 0x00000200;
      Delay_us(12);
      GPIOE->BSRR |= 0x02000000;
 
      while (!(GPIOE->IDR & 0x0400));
      TIM2->CNT = 0; //TIM2 ī���� �ʱ�ȭ
      while (GPIOE->IDR & 0x0400);

      float distance1 = (TIM2->CNT) / 5800.; // cm
 
      TFT_xy(10, 28);
      TFT_color(Cyan, Black);
      TFT_signed_float((float)distance1, 6, 1);
      TFT_color(White,Black);
 
      Delay_ms(200);
     
      TFT_clear_screen();
      TFT_string(8,10, White,Black, "�۵� ��...");
     
      return distance1;
}

unsigned int sayPassword(void){
  success = 0;
  TFT_clear_screen();
  i = 0;
  TFT_string_large(1,8,White,Black,"��ȣ  �Է�");
  TFT_string_large(1,14,White,Black," _  _  _  _  _  _ ");
  while(i<6){
    //debouncing
    Delay_ms(100);
    while( ((GPIOC->IDR & 0x0000000F) == no_key) && !time_out )
      Delay_us(50);
    if(!time_out){
      key = (GPIOC->IDR & 0x0000000F);
      if(key == 0x0F) continue;
      Delay_ms(100);
    }
    while( ((GPIOC->IDR & 0x0000000F) != no_key) && !time_out)
      Delay_us(50);
    if(!time_out){
      passwordTemp[i] = key;
      TFT_xy(1,20);
      TFT_0x_hexadecimal_large(key,4);
      TFT_xy(2+3*i,14);
      TFT_English_large(0x2A);
      //ADC1  result print
      if(ADC1->CR2 & 0x00000001){
        TFT_xy(10, 26);
        TFT_color(White, Black);
        TFT_unsigned_decimal(ADC1->DR, 1, 4);
      }
    }
    i++;
  }
  Delay_us(5);
  if(!time_out){
      for(int i = 0;i<6;i++){
          if ( passwordTemp[i] != password[i] ){
              TFT_string_large(1,8,White,Black,"��ȣ Ʋ��  ");
             
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
             
              return 0; //wrong
          }
      }
      key = 0x0F;
      return 2;     //correct
  }
  return 1;     //time out
}

void setPassword(void){
  while( (GPIOC->IDR & 0x0000000F) != no_key);
  if( !sayPassword() ){
    return;
  } else {
    TFT_string_large(1,8,White,Black,"�� ��ȣ �Է�");
    i = 0;
    TFT_string_large(1,14,White,Black," _  _  _  _  _  _ ");
    while(i<6){
      Delay_ms(100);
      while( (GPIOC->IDR & 0x0000000F) == no_key);
      key = (GPIOC->IDR & 0x0000000F);
      if(key == 0x0F) continue;
      Delay_ms(100);
      while( (GPIOC->IDR & 0x0000000F) != no_key);
      password[i] = key;
      TFT_xy(2+3*i,14);
      TFT_English_large(0x2A);          
      i++;
    }
  TFT_string_large(1,8,White,Black,"��й�ȣ ���� �Ϸ�");
  Delay_ms(3000);
  success = 1;
  key = 0x0F;
  }
}

void Display_screen(void)         /* display test screen */
{

  TFT_clear_screen();            // clear screen
  TFT_landscape_mode();            // landscape mode

  TFT_string( 0, 0,White,Black,"���� �޴�");
  TFT_string( 0, 3,White,   Black,"   (1) ���� ��� ����");
  TFT_string( 0, 6,White, Black,"   (2) ��ȣ �缳��");
  TFT_string( 0,28,White,   Black,"KEY1(����)");
  TFT_string(20,28,White,   Black," KEY3( ) ");
  TFT_string(31,28,White,   Black," KEY4( ) ");
  TFT_string( 5,28,Red,     Black,"����");
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

void secure_mode(void){
    TFT_clear_screen();
    TFT_string(8,10, White,Black, "3�� �� ���ȸ�� ����");      //����ڰ� ������ �����κ��� �־��� �ð� Ȯ��. 3��.
    SysTick->CTRL = 0x00000003;
    SysTick->LOAD = 26999;      //27MHz / 27000 = 1000Hz
    SysTick->VAL = 0;
    while(!(time_systick>=3000)){
      Delay_us(5);
    }
    SysTick->CTRL &= 0x00000000; time_systick = 0;                      ////�ý�ƽ off, �ʱ�ȭ
    while(1){
        //UltraSonic_TIM2
        TIM2->PSC = 54 - 1; // 1us
        TIM1->CNT = 0;            // clear counter
        TIM2->CR1 = 0x00000001;
        ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1			// right alignment, single conversion, ADON = 1
        //������ polling
        while(!(UltraSonic_running() <= 30))
          Delay_us(10);
        send_c();                         //�̶�, �Ƶ��̳뿡�� �Կ�
        //������ ����(TIM2) ����
        TIM2->CR1 = 0x00000000;
        TIM1->DIER = 0x0001;
        TIM1->CNT = 0;            // clear TIM1 counter
        while(1){
            int return_value = sayPassword();
            //wrong
            if(return_value == 0) password_wrong++;
            //correct
            if(return_value == 2){
                secure_mode_clear = 1;
                TFT_clear_screen();
                TFT_string(8,10, White,Black, "����");
                break;
            }
           //wrong 3times
            if(password_wrong == 3){
                /*ESP���� �Կ� �� ���� >> ���� ���*/
                send_c();
                send_s();
                TFT_clear_screen();
                TFT_string(8,10, White,Black, "3ȸ Ʋ�Ƚ��ϴ�.");
                break;
            }
            else if(time_out){
                TFT_clear_screen();
                TFT_string(8,10, White,Black, "�ð� �ʰ�");
                send_s();
                break;
            }
        }
         Delay_ms(1000);
         TIM1->DIER = 0x0000;
         password_wrong = 0; time_out = 0; time = 0;
         ADC1->CR2 = 0x00000000;
         ADC_result = 0;
         if(secure_mode_clear){
            secure_mode_clear = 0;
            break;
         }
         else{
            TFT_clear_screen();
            TFT_string(8,10, White,Black, "3�� �� ���ȸ�� �����");
            SysTick->CTRL = 0x00000003;
            SysTick->LOAD = 26999;      //27MHz / 27000 = 1000Hz
            SysTick->VAL = 0;
            while(!(time_systick >= 3000))
              Delay_us(50);
            TFT_clear_screen();
            }
            SysTick->CTRL &= 0x00000000; time_systick = 0;                  //�ý�ƽ off, �ʱ�ȭ
        }
}


int main(void){
  Initialize_MCU();
  Delay_ms(50);               // wait for system stabilization
  Initialize_TFT_LCD();  
 
  //TIM1
  RCC->APB2ENR |= 0x00000001;         // enable TIMER1  >> Ŭ�� �־��༭ Ÿ�̸� ���� ����
  TIM1->PSC = 1079;            // 108MHz/(1079+1) = 100kHz
  TIM1->ARR = 999;            // 100kHz/(999+1) = 100Hz
  TIM1->DIER = 0x0000;            // enable update interrupt
  NVIC->ISER[0] |= 0x02000000;         // enable (25)TIM1 interrupt
  TIM1->CNT = 999;            // clear counter
  TIM1->CR1 = 0x0005;            // enable TIM1 and update event
  //ADC1
  GPIOA->MODER |= 0x00003000;			// use PA6, ADC1
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock
  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000000;     		// sampling time of channel 6 = 3 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->SQR1 = 0x00000000;
  //ADC1->SQR3 = 0x00000006;			// total regular channel number = 1
  ADC1->SQR3 = 0x00000002;              //instead use VR1
  //PA8 == UART7_RX
  GPIOA->MODER &= 0xFFFCFFFF;
  GPIOA->MODER |= 0x00020000;
  GPIOA->AFR[1] &= 0xFFFFFFF0;
  GPIOA->AFR[1] |= 0x0000000C;
  //PE8 == UART7_TX
  RCC->AHB1ENR |= 0x00000010;
  GPIOE->MODER &= 0xFFFCFFFF;
  GPIOE->MODER |= 0x00020000;
  GPIOE->AFR[1] &= 0xFFFFFFF0;
  GPIOE->AFR[1] |= 0x00000008;
  //UART7 enable
  RCC->APB1ENR |= 0x40000000;
  UART7->CR2 = 0x00000000;			// asynchronous mode, 1 stop bit
  UART7->CR3 = 0x00000000;		// 3sampling bit
  UART7->CR1 = 0x0000000D;
  UART7->BRR = 469;                    // 54000000/469 =115200
  Delay_ms(1);
  RXD = UART7->RDR;
  //TIM2 clock
  RCC->APB1ENR |= 0x00000001;
  //PE9~10 trigger and echo
  GPIOE->MODER &= 0xFFC3FFFF;
  GPIOE->MODER |= 0x00040000;
  
  LCD_command(0x01);
  /*
  TFT_string_large(1,4,Cyan,   Black,"  OK-STM767 ŰƮ  V1.0  ");
  TFT_string_large(1,8,White,Magenta," �ϵ���� ���� ���α׷� ");
  TFT_string(6,15,Cyan,   Black," �������� Ohm�� / R&C Kit�� ");
  TFT_string(6,21,White,  Black,"    2017�� 01�� 01��(��)    ");
  TFT_string(6,24,Green,  Black,"        ������  �ۼ�        ");
  */
  GPIOD->MODER |= 0x04;
  GPIOD->ODR |= 0x02;
  Delay_ms(1000);
 
  while(!success){      //���������� �����Ҷ����� �ݺ�
    setPassword();
  }
   
  Display_screen();
  Display_Cursor(menu_cursor);
 
  while(1) {
    while( (GPIOC->IDR & 0x0000000F) != no_key);         //debouncing
    while(key == no_key){         // wait key input
      key = Key_input();
    }
      if(key == KEY3)            // if KEY3, decrenment cursor number
       { key = no_key;
         menu_cursor--;
         if(menu_cursor == 0) menu_cursor = 2;
         Display_Cursor(menu_cursor);
       }
      else if(key == KEY4)         // if KEY4, increnment cursor number
       { key = no_key;
         menu_cursor++;
         if(menu_cursor > 2) menu_cursor = 1;
         Display_Cursor(menu_cursor);
       }
      else if(key == KEY2)         // if KEY2, ignore and skip
       key = no_key;
      else if(key == KEY1)         // if KEY1, run function number
       { key = no_key;
         if(menu_cursor == 1) secure_mode();
         else if(menu_cursor == 2) setPassword();

         Display_screen();         // display cursor screen
         Display_Cursor(menu_cursor);      // display cursor number
       }
    }
}