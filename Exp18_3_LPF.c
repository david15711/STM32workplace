/* ============================================================================ */
/* 		Exp18_3_LPF.c : 전형적인 저역통과 디지털 필터			*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include <arm_math.h>				// 반드시 마지막에 인클루드할 것.

#define	Ts	0.00001				// sampling period = 1/100kHz = 10 us

void SysTick_Handler(void);			/* SysTick interrupt(100kHz) for DAC output */
void Display_waveform_screen(void);		/* display waveform screen */
void Draw_waveform_axis(void);			/* draw axis of waveform screen */
void Clear_waveform(void);			/* clear graphic screen */

unsigned char DAC_count, wave_mode, wave_flag;
unsigned short fc, wave_count;
unsigned int time_1sec = 0;

float DAC_buffer[300], ADC_buffer[300];
float K, k1, k2, yn, yo = 0.;

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void SysTick_Handler(void)			/* SysTick interrupt(100kHz) for DAC output */
{
  unsigned short DAC_data;

  if(wave_mode == 1)                            // calculate data of sin wave
    DAC_data = (unsigned short)(2047.*arm_sin_f32(DAC_count*3.6*3.141592/180.) + 2047.);
  else if(wave_mode == 2)                       // calculate data of triangular wave
    { if(DAC_count <= 49)
        DAC_data = (unsigned short)((4095./50.)*(float)DAC_count);
      else
        DAC_data = (unsigned short)((-4095./50.)*(float)DAC_count + 8190.);
    }
  else if(wave_mode == 3)                       // calculate data of square wave
    { if(DAC_count <= 49) DAC_data = 4095;
      else                DAC_data = 0;
    }
  else if(wave_mode == 4)			// calculate data of sin wave
    { DAC_data = (unsigned short)(1500.*arm_sin_f32(DAC_count*3.6*3.141592/180.) + 2047.);
      DAC_data += (unsigned short)(500.*arm_sin_f32(DAC_count*10.*3.6*3.141592/180.));
    }

  DAC->DHR12R1 = DAC_data;			// output data to D/A converter
  DAC_count++;
  if(DAC_count >= 100) DAC_count = 0;
  DAC_buffer[wave_count] = DAC_data;		// DAC output data

  ADC1->SQR3 = 0x00000006;			// channel 6
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
  yn = (float)ADC1->DR;				// ADC input data
  yn = k1*yo + k2*yn;				// calculate LPF
  yo = yn;
  DAC->DHR12R2 = (unsigned short)yn;		// output data to D/A converter

  ADC1->SQR3 = 0x00000007;			// channel 7
  ADC1->CR2 |= 0x40000000;			// start conversion by software
  while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
  ADC_buffer[wave_count] = ADC1->DR;		// ADC input data

  wave_count++;
  if(wave_count >= 300) wave_count = 0;

  time_1sec++;					// check 1sec time
  if(time_1sec >= 100000)
    { time_1sec = 0;
      wave_flag = 1;
    }
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned char key, start_flag;
  unsigned short i, y1, y1o = 0, y2, y2o = 0;	// graphic variable

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD modul

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp18_3.c    ");

  TFT_string(0,4,Green,Black,"****************************************");
  TFT_string(0,6,White,Black,"          전형적인 디지털 LPF           ");
  TFT_string(0,8,Green,Black,"****************************************");
  TFT_string(2,25,Cyan,Black, "파형");
  TFT_string(2,27,Cyan,Black, "선택");
  TFT_string(32,25,Magenta,Black, " fc ");
  TFT_string(32,27,Magenta,Black, "선택");

  Rectangle( 12,196,  52,235, Yellow);		// display outline of key
  Rectangle(252,196, 292,235, Yellow);
  Beep();

  key = no_key;					// wait for start
  while(key == no_key) key = Key_input();
  start_flag = 0;
  wave_mode = 1;
  fc = 8;
  K = 2.*PI*(float)fc*1000.*Ts;			// calculate LPF variables
  k1 = 1./(1.+K);
  k2 = K/(1.+K);

  GPIOA->MODER |= 0x00000F00;			// PA4/5 = analog mode(DAC_OUT1/2)
  RCC->APB1ENR |= 0x20000000;			// enable DAC clock
  DAC->CR = 0x00030003;				// DAC channel 1/2 enable, output buffer disable

  GPIOA->MODER |= 0x0000F000;			// PA6/7 = analog mode(ADC12_IN6/7)
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock
  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00240000;			// sampling time of channel 6~7 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;			// total regular channel number = 1

  SysTick->LOAD  = 269;				// 27MHz/(269+1) = 100kHz
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000003;			// 216MHz/8 = 27MHz, enable SysTick and interrupt

  while(1)
    { if(start_flag == 0)			// if initial start, skip key input
        start_flag = 1;
      else
        key = Key_input();

      if(key == KEY1)				// if KEY1, select wave mode
       { key = no_key;
         wave_count = 0;
	 DAC_count = 0;
	 wave_mode++;
         if(wave_mode > 4) wave_mode = 1;
	 Display_waveform_screen();
	 if(wave_mode == 1)
	   TFT_string(5,0,White,Magenta," 정현파(1kHz)의 입력/출력 파형 ");
	 else if(wave_mode == 2)
           TFT_string(5,0,White,Magenta," 삼각파(1kHz)의 입력/출력 파형 ");
	 else if(wave_mode == 3)
           TFT_string(5,0,White,Magenta," 구형파(1kHz)의 입력/출력 파형 ");
	 else if(wave_mode == 4)
	   TFT_string(5,0,White,Magenta," 합성파(1kHz)의 입력/출력 파형 ");
	 if(fc == 1)
	   TFT_string(15,2,Cyan,Black,"fc = 1kHz");
	 else if(fc == 2)
	   TFT_string(15,2,Cyan,Black,"fc = 2kHz");
	 else if(fc == 4)
	   TFT_string(15,2,Cyan,Black,"fc = 4kHz");
	 else if(fc == 8)
	   TFT_string(15,2,Cyan,Black,"fc = 8kHz");
        }
      else if(key == KEY4)			// if KEY4, select cutoff frequency
       { key = no_key;
         wave_count = 0;
	 DAC_count = 0;
         if(fc == 8)      fc = 4;
         else if(fc == 4) fc = 2;
         else if(fc == 2) fc = 1;
         else if(fc == 1) fc = 8;
         K = 2.*PI*(float)fc*1000.*Ts;
         k1 = 1./(1.+K);
         k2 = K/(1.+K);
	 Display_waveform_screen();
	 if(wave_mode == 1)
	   TFT_string(5,0,White,Magenta," 정현파(1kHz)의 입력/출력 파형 ");
         else if(wave_mode == 2)
           TFT_string(5,0,White,Magenta," 삼각파(1kHz)의 입력/출력 파형 ");
	 else if(wave_mode == 3)
           TFT_string(5,0,White,Magenta," 구형파(1kHz)의 입력/출력 파형 ");
	 else if(wave_mode == 4)
	   TFT_string(5,0,White,Magenta," 합성파(1kHz)의 입력/출력 파형 ");
	 if(fc == 1)
	   TFT_string(15,2,Cyan,Black,"fc = 1kHz");
	 else if(fc == 2)
	   TFT_string(15,2,Cyan,Black,"fc = 2kHz");
	 else if(fc == 4)
	   TFT_string(15,2,Cyan,Black,"fc = 4kHz");
	 else if(fc == 8)
	   TFT_string(15,2,Cyan,Black,"fc = 8kHz");
       }

      if(wave_flag == 1)			// display waveforms by 1sec period
        { wave_flag = 0;
	  Clear_waveform();
	  for(i = 0; i < 280; i++)
            { y1 = (unsigned short)(120. - DAC_buffer[i]*83./4095. + 0.5);
              if(i == 0) y1o = y1;
              Line(30+i,y1o, 31+i,y1, Green);
              y1o = y1;
          
              y2 = (unsigned short)(220. - ADC_buffer[i]*83./4095. + 0.5);
              if(i == 0) y2o = y2;
              Line(30+i,y2o, 31+i,y2, Cyan);
              y2o = y2;
            }
	}
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Display_waveform_screen(void)		/* display waveform screen */
{
  TFT_clear_screen();				// clear screen

  TFT_color(White,Black);
  TFT_English_pixel(18,213, '0');		// 0.0 for ADC waveform
  TFT_English_pixel(2, 188, '1');		// 1.0
  TFT_English_pixel(10,188, '.');
  TFT_English_pixel(18,188, '0');
  TFT_English_pixel(2, 163, '2');		// 2.0
  TFT_English_pixel(10,163, '.');
  TFT_English_pixel(18,163, '0');
  TFT_English_pixel(2, 138, '3');		// 3.0
  TFT_English_pixel(10,138, '.');
  TFT_English_pixel(18,138, '0');
  TFT_English_pixel(18,113, '0');		// 0.0 for DAC waveform
  TFT_English_pixel(2,  88, '1');		// 1.0
  TFT_English_pixel(10, 88, '.');
  TFT_English_pixel(18, 88, '0');
  TFT_English_pixel(2,  63, '2');		// 2.0
  TFT_English_pixel(10, 63, '.');
  TFT_English_pixel(18, 63, '0');
  TFT_English_pixel(2,  38, '3');		// 3.0
  TFT_English_pixel(10, 38, '.');
  TFT_English_pixel(18, 38, '0');
  TFT_color(Magenta,Black);
  TFT_English_pixel(2,  18, '[');               // [V]
  TFT_English_pixel(10, 18, 'V');
  TFT_English_pixel(18, 18, ']');

  TFT_color(White,Black);
  TFT_English_pixel(26,  222, '0');		// 0.0 for time
  TFT_English_pixel(68,  222, '0');		// 0.5
  TFT_English_pixel(76,  222, '.');
  TFT_English_pixel(84,  222, '5');
  TFT_English_pixel(118, 222, '1');		// 1.0
  TFT_English_pixel(126, 222, '.');
  TFT_English_pixel(134, 222, '0');
  TFT_English_pixel(168, 222, '1');		// 1.5
  TFT_English_pixel(176, 222, '.');
  TFT_English_pixel(184, 222, '5');
  TFT_English_pixel(218, 222, '2');		// 2.0
  TFT_English_pixel(226, 222, '.');
  TFT_English_pixel(234, 222, '0');
  TFT_color(Magenta,Black);
  TFT_English_pixel(285, 223, '[');		// [ms]
  TFT_English_pixel(293, 223, 'm');
  TFT_English_pixel(301, 223, 's');
  TFT_English_pixel(309, 223, ']');

  Draw_waveform_axis();				// draw axis of waveform screen
}

void Draw_waveform_axis(void)			/* draw axis of waveform screen */
{ 
  unsigned short x, y;
  
  Line(30,  220, 310, 220, White);		// x-axis line
  Line(305, 215, 310, 220, White);
  Line(305, 225, 310, 220, White);
  Line(30,  130, 30,  220, White);		// y-axis line
  Line(35,  135, 30,  130, White);
  Line(25,  135, 30,  130, White);

  Line(30,  120, 310, 120, White);		// x-axis line
  Line(305, 115, 310, 120, White);
  Line(305, 125, 310, 120, White);
  Line(30,  30,  30,  120, White);		// y-axis line
  Line(35,  35,  30,  30,  White);
  Line(25,  35,  30,  30,  White);
  
  for(x = 80; x <= 280; x += 50)		// x-axis scale
    { Line(x,218, x,222, White);
      for(y = 35; y <= 120; y += 5)		// vertical line
        TFT_pixel(x,y, White);

      Line(x,118, x,122, White);
      for(y = 135; y <= 220; y += 5)		// vertical line
        TFT_pixel(x,y, White);
    }

  for(y = 95; y >= 45; y -= 25)		        // y-axis scale
    { Line(28,y, 32,y, White);
      for(x = 30; x <= 310; x += 5)		// horizontal line
        TFT_pixel(x,y, White);
    }

  for(y = 195; y >= 145; y -= 25)		// y-axis scale
    { Line(28,y, 32,y, White);
      for(x = 30; x <= 310; x += 5)		// horizontal line
        TFT_pixel(x,y, White);
    }
}

void Clear_waveform(void)			/* clear waveform */
{ 
  unsigned short x, y;

  for(y = 30; y <= 120; y++)			// clear DAC waveform
    { TFT_GRAM_address(30,y);
      for(x = 30; x <= 310; x++)
        TFT_data(Black);
    }

  for(y = 130; y <= 220; y++)			// clear ADC waveform
    { TFT_GRAM_address(30,y);
      for(x = 30; x <= 310; x++)
        TFT_data(Black);
    }

  Draw_waveform_axis();				// draw axis of waveform screen
}
