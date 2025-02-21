/* ============================================================================ */
/*		A/D 컨버터의 순차 실행     	*/
/* ============================================================================ */
/*					                */

#include "stm32f767xx.h"
#include "OK-STM767.h"
int time, time_out;
float temperature;

void TIM1_UP_TIM10_IRQHandler(void)           /* TIM1 interrupt function (10Hz)*/
{
  TIM1->SR = 0x0000;            // clear pending bit of TIM1 interrupt
  time++;                       //time 150 되면 15초
 
  if(((time % 10) == 0) && (time < 150)){
    for(int i = 15;i > 0;i--){
      if((i * 10) == time){
          TFT_color(Red, Black); TFT_xy(36,28); TFT_unsigned_decimal(15 - i, 0, 2);
          break;
      }
    }
  }
  else if(time == 150){
    /*STM에 있는 RGB_value 휴대폰으로 전송 >> 무선 통신*/
    TFT_string(30,28, White,Black, "시간 초과");
    time_out = 1;
    Delay_ms(1000);
  }
  //ADC
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));            // wait for end of conversion EOC
  if(1){
    TFT_xy(24,16);				// display voltage
    TFT_color(Cyan,Black);
    TFT_signed_float((float)ADC1->DR*3.3/4095., 1, 2);
  }
}

void secure_mode_TIM1_15sec(void){
  RCC->APB2ENR |= 0x00000001;         // enable TIMER1  >> 클록 넣어줘서 타이머 동작 시작
  TIM1->PSC = 1079;            // 108MHz/(1079+1) = 100kHz
  TIM1->ARR = 9999;            // 100kHz/(9999+1) = 10Hz
  TIM1->DIER = 0x0000;            // enable update interrupt
  NVIC->ISER[0] |= 0x02000000;         // enable (25)TIM1 interrupt
  TIM1->CNT = 0;            // clear counter
  TIM1->CR1 = 0x0005;            // enable TIM1 and update event
}

int main(void)
{
  unsigned short result1,result2,result3,result4,result5,result6,result7;
  float temperature;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  TFT_string(0, 3,White,Magenta,"      A/D 컨버터의 순차변환 모드        ");
  TFT_string(8,7,White,Black,"ADC18 (Temp.) = +00.0 C");
  TFT_xy(29,7);        //degree char
  TFT_English(0x7F);
  TFT_string(8,10,White,Black,"ADC0  (+5.0V) = +0.00V");
  TFT_string(8,13,White,Black,"ADC1  (+3.3V) = +0.00V");
  TFT_string(8,16,White,Black,"ADC2  ( VR1 ) = +0.00V");
  TFT_string(8,19,White,Black,"ADC3 (LM35DZ) = +00.0 C");
  TFT_xy(29,19);
  TFT_English(0x7F);
  TFT_string(8,22,White,Black,"ADC17 (+1.2V) = +0.00V");
  TFT_string(8,25,White,Black,"ADC18 (+VBAT) = +0.00V");

  GPIOA->MODER |= 0x000000FF;			// use ADC0~ADC3
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  //secure_mode_TIM1_15sec();
  GPIOA->MODER |= 0x00003000;			// use PA6, ADC1
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock
  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000000;     		// sampling time of channel 6 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;
  ADC1->SQR3 = 0x00000006;			// total regular channel number = 1
  
  while(1){
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));            // wait for end of conversion EOC 
  result1 = ADC1->DR;
  temperature = (float)result1*3.3/4095.;	// calculate temperature
  temperature = (temperature-0.76)/0.0025 + 25.;
  TFT_xy(24,16);				// display voltage
  TFT_color(Cyan,Black);
  TFT_signed_float(temperature, 2, 1);
  Delay_ms(10);
  }
  
}
