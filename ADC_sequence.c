/* ============================================================================ */
/*		A/D 컨버터의 순차 실행     	*/
/* ============================================================================ */
/*					                */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned short result1,result2,result3,result4,result5,result6,result7;
  float temperature;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp13_1.c    ");
  //Beep();

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

  ADC->CCR = 0x00800000;			// enable channel 17~18, ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR1 = 0x07E00000;			// sampling time of channel 17~18 = 480 cycle
  ADC1->SMPR2 = 0x00000249;			// sampling time of channel 0~3 = 15 cycle
  ADC1->CR1 = 0x00000100;			// 12-bit resolution, scan mode
  ADC1->CR2 = 0x00000401;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00600000;			// total regular channel number = 1
  ADC1->SQR3 = 0x22310412;                      // 0b 00 10/001 0/0011 /0001/0 000/01 00/000 1/0010
  ADC1->SQR2 |= 0x00000006;                      // 0b 00 00000 00000 00000 00000 00000 10010

  while(1) {
    
    // channel 18 (temperature sensor)
    ADC->CCR &= 0xFFBFFFFF;			// set channel 18 = temperature sensor
    Delay_us(5);
    ADC1->CR2 |= 0x40000000;			// start conversion by software
    while(!(ADC1->SR & 0x00000002));            // wait for end of conversion EOC
    result1 = ADC1->DR;
    
    // channel 0 (+5.0V)
    while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
    result2= ADC1->DR;
    
    // channel 1 (+3.3V)
    while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
    result3= ADC1->DR;
    
    // channel 2 (VR1)
    while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
    result4= ADC1->DR;
    
    // channel 3 (LM35DZ)
    while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
    result5= ADC1->DR;
    
    // channel 17 (VREFINT = 1.21V)
    while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
    result6= ADC1->DR;

    // channel 18 (VBAT/4)
    ADC->CCR |= 0x00400000;			// set channel 18 = VBAT/4
    Delay_us(5);
    while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
    result7= ADC1->DR;
    
    //display
    temperature = (float)result1*3.3/4095.;	// calculate temperature
    temperature = (temperature-0.76)/0.0025 + 25.;
    TFT_xy(24,7);				// display temperature
    TFT_color(Magenta,Black);
    TFT_signed_float(temperature, 2, 1);
    
    TFT_xy(24,10);				// display voltage
    TFT_color(Cyan,Black);
    TFT_signed_float((float)result2*2.*3.3/4095., 1, 2);
    
    TFT_xy(24,13);				// display voltage
    TFT_color(Cyan,Black);
    TFT_signed_float((float)result3*2.*3.3/4095., 1, 2);

    TFT_xy(24,16);				// display voltage
    TFT_color(Cyan,Black);
    TFT_signed_float((float)result4*3.3/4095., 1, 2);
    
    TFT_xy(24,19);				// display temperature
    TFT_color(Cyan,Black);
    TFT_signed_float((float)result5*330./4095., 2, 1);
    
    TFT_xy(24,22);				// display voltage
    TFT_color(Magenta,Black);
    TFT_signed_float((float)result6*3.3/4095., 1, 2);
    
    TFT_xy(24,25);				// display VBAT voltage
    TFT_color(Magenta,Black);
    TFT_signed_float((float)result7*4.*3.3/4095., 1, 2);
    
    LED_toggle();				// blink LED1
    Delay_ms(200);
  }
}
