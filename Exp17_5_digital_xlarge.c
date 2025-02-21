/* ============================================================================ */
/*    Exp17_5_digital_xlarge.c : 가장 큰 글씨로 표시한 디지털 시계 프로그그램	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"
#include "OK-STM767_xlarge.h"

void Screen1_initialize(void);			/* initialize screen 1 */
void Display_time(void);			/* display current time on screen 1 */
void Read_temperature(void);			/* read temperature from LM35DZ */

void Screen2_initialize(void);			/* initialize screen 2 */
void Display_screen2_time(void);		/* display current time on screen 2 */
void Screen2_cursor_right(void);		/* move cursor to right */
void Screen2_increment(void);			/* increment value */
void Screen2_decrement(void);			/* decrement value */

void Screen3_initialize(void);			/* initialize screen 3 */
void Alarm_initialize(void);			/* initialize alarm of DS3234 RTC */
void Display_screen3_alarm(void);		/* display alarm time on screen 3 */
void Screen3_cursor_right(void);		/* move cursor to right */
void Screen3_increment(void);			/* increment value */
void Screen3_decrement(void);			/* decrement value */
void DS3234_alarm_set(void);			/* set alarm of DS3234 RTC */

unsigned char screen_mode = 1;			// screen mode 1~4
unsigned char blink_count = 0;			// colon blink count
unsigned char year, month, date, weekday, hour, minute, second, second_old;

unsigned char alarm_set = 0x80;			// alarm set flag(0x80 = off, 0x00 = on)
unsigned char alarm_status = 0;
unsigned char alarm_ampm, alarm_hour, alarm_minute;

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned char key, date_flag = 0, hour_flag = 0;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  DS3234_initialize();				// initialize DS3234 RTC
  Alarm_initialize();				// initialize alarm of DS3234 RTC
  Beep();					// beep

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp17_5.c    ");

  GPIOA->MODER |= 0x000000C0;			// use ADC3
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock

  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00000200;			// sampling time of channel 3 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;			// total regular channel number = 1
  ADC1->SQR3 = 0x00000003;			// channel 3 (LM35DZ)

  Screen1_initialize();				// initialize screen 1

  while(1)
    { second = DS3234_read(0x00);		// new second ?
      if(second != second_old)			// if yes, read time
        { second_old = second;
          minute = DS3234_read(0x01);
          hour = DS3234_read(0x02) & 0x3F;
          weekday = DS3234_read(0x03);
          date = DS3234_read(0x04);
          month = DS3234_read(0x05);
          year = DS3234_read(0x06);

          if(alarm_status == 1)			// go alarm
            Beep_3times();

	  blink_count = 0;			// start colon blink count
          if(screen_mode == 1)
	    Read_temperature();			// read and display temperature
	}

      if((minute == 0x00) && (second == 0x00) && (hour_flag == 0)) // 매시 정각 경보
        { hour_flag = 1;
	  Beep();
	}
      else if(second >= 0x01)
        hour_flag = 0;

      if((hour == 0x12) && (minute == 0x00) && (second == 0x00) && (date_flag == 0)) // 새로운 날짜의 시작
        { date_flag = 1;
          screen_mode = 1;
	  Initialize_TFT_LCD();
          Screen1_initialize();
	}
      else if(second >= 0x01)
        date_flag = 0;

      if(alarm_set == 0x00)			// check alarm
        { if((DS3234_read(0x0F) & 0x01) == 0x01)
            alarm_status = 1;
	}
      if((alarm_set == 0x00) && (alarm_status == 1) && (second >= 0x05)) // end alarm
        { alarm_status = 0;
	  alarm_set = 0x80;
	  DS3234_alarm_set();
        }

      key = Key_input();			// key input
      if((key == KEY1) && (screen_mode == 1))	// ***** change screen_mode from 1 to 2
        { screen_mode = 2;
          Screen2_initialize();			// initialize screen 2
          key = no_key;
        }
      else if((key == KEY1) && (screen_mode == 2)) // ***** change screen_mode from 2 to 3
        { screen_mode = 3;
          TFT_cursor(Transparent);
          Screen3_initialize();			// initialize screen 3
          key = no_key;
        }
      else if((key == KEY1) && (screen_mode == 3)) // ***** change screen_mode from 3 to 1
        { DS3234_alarm_set();
	  screen_mode = 1;
          TFT_cursor(Transparent);
          Screen1_initialize();			// initialize screen 1
          key = no_key;
        }

      if(screen_mode == 1)			// ***** 화면1 : 시스템 상태
        { Display_time();			// display current time
          if(blink_count < 80)			// display colon for 0.5 sec
            { TFT_color(Cyan,Black);
              TFT_xy(13,14);
	      TFT_English_xlarge(':');
              TFT_xy(26,14);
	      TFT_English_xlarge(':');
              if(alarm_set == 0x00)
	        { TFT_string(10,5,White,Black," 알람 (");
                  if(alarm_ampm == 0x40) TFT_string(17,5,White,Black,"오전 ");
                  else                   TFT_string(17,5,White,Black,"오후 ");
                  TFT_hexadecimal(alarm_hour,2);
		  TFT_English(':');
		  TFT_hexadecimal(alarm_minute,2);
		  TFT_English(')');
		  TFT_English(' ');
		}
	    }
          else
	    { TFT_color(Cyan,Black);
	      TFT_xy(13,14);
	      TFT_English_xlarge('/');
              TFT_xy(26,14);
	      TFT_English_xlarge('/');
              if(alarm_set == 0x00)
                TFT_string(10,5,Green,Black,"*******************");
	    }
	}
      else if(screen_mode == 2)			// ***** 화면2 : 현재 날짜/시각 설정
        { if(key == KEY2)      Screen2_cursor_right();
          else if(key == KEY3) Screen2_increment();
          else if(key == KEY4) Screen2_decrement();
	  else                 Display_screen2_time();
        }
      else if(screen_mode == 3)			// ***** 화면3 : 알람 설정
        { if(key == KEY2)      Screen3_cursor_right();
          else if(key == KEY3) Screen3_increment();
          else if(key == KEY4) Screen3_decrement();
	  else                 Display_screen3_alarm();
        }

      Delay_ms(1);				// delay count for blink time
      blink_count++;
    }
}

/* ----- 화면1 관련 함수 (기본 화면) ------------------------------------------ */

void Screen1_initialize(void)			/* initialize screen 1 */
{
  TFT_clear_screen();				// clear screen

  TFT_string(0,1,Green,  Black,"****************************************");
  TFT_string(0,3,Magenta,Black,"       OK-STM767 키트 디지털 시계       ");
  TFT_string(0, 5,Green, Black,"****************************************");
  TFT_string(0,27,Green, Black,"****************************************");
  TFT_string(3,27,White, Black,"(설계/제작:윤덕용)");

  Rectangle(0,105, 319,171, Cyan);
  Rectangle(1,106, 318,170, Cyan);

  TFT_string_large(6,9,Cyan,Black,"2017/01/01 (일)");
  TFT_color(Cyan,Black);
  TFT_English_pixel_large(9,115,'A');
  TFT_English_pixel_large(9,139,'M');
  TFT_xy(3,14);
  TFT_color(White,Black);
  TFT_hexadecimal_xlarge(0,2);
  TFT_English_xlarge(':');
  TFT_hexadecimal_xlarge(0,2);
  TFT_English_xlarge(':');
  TFT_hexadecimal_xlarge(0,2);

  TFT_string_large(7,22,Yellow,Black,"온도 00.0  C");
  TFT_xy(17,22);
  TFT_English_large(0x7F);
}

void Display_time(void)				/* display current time on screen 1 */
{
  TFT_color(Yellow,Black);			// display color
  TFT_xy(6,9);					// display year
  TFT_hexadecimal_large(0x20,2);
  TFT_hexadecimal_large(year,2);
  TFT_xy(11,9);			  		// display month
  TFT_hexadecimal_large(month,2);
  TFT_xy(14,9);					// display date
  TFT_hexadecimal_large(date,2);
  if(weekday == 0x01)      TFT_string_large(18,9, Yellow,Black, "일"); // display weekday
  else if(weekday == 0x02) TFT_string_large(18,9, Yellow,Black, "월");
  else if(weekday == 0x03) TFT_string_large(18,9, Yellow,Black, "화");
  else if(weekday == 0x04) TFT_string_large(18,9, Yellow,Black, "수");
  else if(weekday == 0x05) TFT_string_large(18,9, Yellow,Black, "목");
  else if(weekday == 0x06) TFT_string_large(18,9, Yellow,Black, "금");
  else if(weekday == 0x07) TFT_string_large(18,9, Yellow,Black, "토");

  TFT_color(Cyan,Black);			// display color
  if((hour & 0x20) == 0x00) TFT_English_pixel_large(9,115,'A');	// display AM/PM
  else                      TFT_English_pixel_large(9,115,'P');

  TFT_color(White,Black);			// display color
  TFT_xy(3,14);					// display hour
  TFT_hexadecimal_xlarge(hour & 0x1F, 2);
  TFT_xy(16,14);				// display minute
  TFT_hexadecimal_xlarge(minute, 2);
  TFT_xy(29,14);				// display second
  TFT_hexadecimal_xlarge(second, 2);
}

unsigned char ADC_start_flag = 0, ADC_count = 0;
unsigned long result, ADC_buffer[32];

void Read_temperature(void)			/* read temperature from LM35DZ */
{
  unsigned char i;

  ADC1->CR2 |= 0x40000000;			// start A/D conversion by software
  while(!(ADC1->SR & 0x00000002));		// wait A/D conversion complete
  result = ADC1->DR;

  if(ADC_start_flag == 1)			// if not start new, store A/D result
    { ADC_buffer[ADC_count] = result;
      ADC_count++;
      ADC_count = ADC_count % 32;
    }
  else
    { for(i = 0; i < 32; i++)			// if start new, initialize buffer 
        ADC_buffer[i] = result;
      ADC_start_flag = 1;
    }

  result = 0;
  for(i = 0; i < 32; i++)			// calculate moving average of buffer
    result += ADC_buffer[i];
  result >>= 5;

  TFT_xy(12,22);				// display temperature
  TFT_color(Cyan,Black);
  TFT_unsigned_float_large((float)result*330./4096.,2,1);
}

/* ----- 화면2 관련 함수 (현재 날짜/시각 설정) -------------------------------- */

void Screen2_initialize(void)			/* initialize screen 2 */
{
  TFT_clear_screen();				// clear screen

  TFT_string(3,25,White,Blue,"화면");		// display key function
  TFT_string(3,27,White,Blue,"변경");
  TFT_string(13,25,White,Brown,"커서");
  TFT_string(13,27,White,Brown,"이동");
  TFT_string(23,25,White,Magenta," 값 ");
  TFT_string(23,27,White,Magenta,"증가");
  TFT_string(33,25,White,Magenta," 값 ");
  TFT_string(33,27,White,Magenta,"감소");

  Rectangle(18,194, 61,237, Cyan);
  Rectangle(19,195, 60,236, Cyan);
  Rectangle(98,194, 141,237, Cyan);
  Rectangle(99,195, 140,236, Cyan);
  Rectangle(178,194, 221,237, Cyan);
  Rectangle(179,195, 220,236, Cyan);
  Rectangle(258,194, 301,237, Cyan);
  Rectangle(259,195, 300,236, Cyan);

  TFT_string(0,3,White,Blue,"          현재 날짜/시각 설정           ");

  TFT_string_large(6,10,Green,Black,"2017/01/01 (일)");
  TFT_string_large(6,15,Green,Black," 오전 12:00:00 ");

  TFT_cursor(Magenta);				// display cursor
  Xcursor = 9;
  Ycursor = 15;
}

void Display_screen2_time(void)			/* display current time on screen 2 */
{
  TFT_color(White,Black);			// display color
  TFT_xy(6,10);					// display year
  TFT_hexadecimal_large(0x20,2);
  TFT_hexadecimal_large(year,2);
  TFT_xy(11,10);	  			// display month
  TFT_hexadecimal_large(month,2);
  TFT_xy(14,10);				// display date
  TFT_hexadecimal_large(date,2);
  if(weekday == 0x01)      TFT_string_large(18,10, White,Black, "일"); // display weekday
  else if(weekday == 0x02) TFT_string_large(18,10, White,Black, "월");
  else if(weekday == 0x03) TFT_string_large(18,10, White,Black, "화");
  else if(weekday == 0x04) TFT_string_large(18,10, White,Black, "수");
  else if(weekday == 0x05) TFT_string_large(18,10, White,Black, "목");
  else if(weekday == 0x06) TFT_string_large(18,10, White,Black, "금");
  else if(weekday == 0x07) TFT_string_large(18,10, White,Black, "토");

  TFT_color(White,Black);			// display color
  if((hour & 0x20) == 0x00) TFT_string_large(7,15,White,Black,"오전"); // display AM/PM
  else                      TFT_string_large(7,15,White,Black,"오후");
  TFT_xy(12,15);				// display hour
  TFT_hexadecimal_large(hour & 0x1F,2);
  TFT_xy(15,15);				// display minute
  TFT_hexadecimal_large(minute,2);
  TFT_xy(18,15);				// display second
  TFT_hexadecimal_large(second,2);
}

void Screen2_cursor_right(void)			/* move cursor to right */
{
  if((Xcursor == 8) && (Ycursor == 10))       { Xcursor = 9;  Ycursor = 10; }
  else if((Xcursor == 9)  && (Ycursor == 10)) { Xcursor = 11; Ycursor = 10; }
  else if((Xcursor == 11) && (Ycursor == 10)) { Xcursor = 12; Ycursor = 10; }
  else if((Xcursor == 12) && (Ycursor == 10)) { Xcursor = 14; Ycursor = 10; }
  else if((Xcursor == 14) && (Ycursor == 10)) { Xcursor = 15; Ycursor = 10; }
  else if((Xcursor == 15) && (Ycursor == 10)) { Xcursor = 18; Ycursor = 10; }
  else if((Xcursor == 18) && (Ycursor == 10)) { Xcursor = 9;  Ycursor = 15; }
  else if((Xcursor == 9)  && (Ycursor == 15)) { Xcursor = 12; Ycursor = 15; }
  else if((Xcursor == 12) && (Ycursor == 15)) { Xcursor = 13; Ycursor = 15; }
  else if((Xcursor == 13) && (Ycursor == 15)) { Xcursor = 15; Ycursor = 15; }
  else if((Xcursor == 15) && (Ycursor == 15)) { Xcursor = 16; Ycursor = 15; }
  else if((Xcursor == 16) && (Ycursor == 15)) { Xcursor = 18; Ycursor = 15; }
  else if((Xcursor == 18) && (Ycursor == 15)) { Xcursor = 19; Ycursor = 15; }
  else if((Xcursor == 19) && (Ycursor == 15)) { Xcursor = 8;  Ycursor = 10; }
}

void Screen2_increment(void)			/* increment value */
{
  if((Xcursor == 8) && (Ycursor == 10))		// in case of year
    { year = DS3234_read(0x06);
      if((year & 0xF0) < 0x90) year += 0x10;
      else                     year &= 0x0F;
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 9) && (Ycursor == 10))	// in case of year
    { year = DS3234_read(0x06);
      if((year & 0x0F) < 0x09) year += 0x01;
      else                     year &= 0xF0;
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 11) && (Ycursor == 10))	// in case of month
    { month = DS3234_read(0x05);
      month ^= 0x10;
      if(month == 0x00) month += 0x01;
      if(month > 0x12)  month = 0x12;
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 12) && (Ycursor == 10))	// in case of month
    { month = DS3234_read(0x05);
      if((month & 0x0F) < 0x09)
        { month += 0x01;
	  if(month > 0x12) month &= 0xF0;
	}
      else
        { month &= 0xF0;
	  if(month == 0x00) month = 0x01;
        }
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 14) && (Ycursor == 10))	// in case of date
    { date = DS3234_read(0x04);
      if((date & 0xF0) < 0x30)
        { date += 0x10;
	  if(date > 0x31) date = 0x31;
        }
      else
        { date &= 0x0F;
	  if(date == 0x00) date = 0x01;
        }
      DS3234_write(0x04,date);
    }
  else if((Xcursor == 15) && (Ycursor == 10))	// in case of date
    { date = DS3234_read(0x04);
      if((date & 0x0F) < 0x09)
        { date += 0x01;
	  if(date > 0x31) date &= 0xF0;
	}
      else
        { date &= 0xF0;
	  if(date == 0x00) date = 0x01;
        }
      DS3234_write(0x04,date);
    }
  else if((Xcursor == 18) && (Ycursor == 10))	// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday < 0x07) weekday += 0x01;
      else               weekday = 0x01;
      DS3234_write(0x03,weekday);
    }
  else if((Xcursor == 9) && (Ycursor == 15))	// in case of AM/PM
    { hour = DS3234_read(0x02);
      hour ^= 0x20;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 12) && (Ycursor == 15))	// in case of hour
    { hour = DS3234_read(0x02);
      hour ^= 0x10;
      if((hour & 0x1F) == 0x00) hour += 0x01;
      if((hour & 0x1F) > 0x12)  hour = (hour & 0x60) + 0x12;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 13) && (Ycursor == 15))	// in case of hour
    { hour = DS3234_read(0x02);
      if((hour & 0x0F) < 0x09)
        { hour += 0x01;
	  if((hour & 0x1F) > 0x12) hour &= 0xF0;
	}
      else
        { hour &= 0xF0;
	  if((hour & 0x1F) == 0x00) hour += 0x01;
        }
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 15) && (Ycursor == 15))	// in case of minute
    { minute = DS3234_read(0x01);
      if((minute & 0xF0) < 0x50) minute += 0x10;
      else                       minute &= 0x0F;
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 16) && (Ycursor == 15))	// in case of minute
    { minute = DS3234_read(0x01);
      if((minute & 0x0F) < 0x09) minute += 0x01;
      else                       minute &= 0xF0;
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 18) && (Ycursor == 15))	// in case of second
    { second = DS3234_read(0x00);
      if((second & 0xF0) < 0x50) second += 0x10;
      else                       second &= 0x0F;
      DS3234_write(0x00,second);
    }
  else if((Xcursor == 19) && (Ycursor == 15))	// in case of second
    { second = DS3234_read(0x00);
      if((second & 0x0F) < 0x09) second += 0x01;
      else                       second &= 0xF0;
      DS3234_write(0x00,second);
    }
}

void Screen2_decrement(void)			/* decrement value */
{
  if((Xcursor == 8) && (Ycursor == 10))		// in case of year
    { year = DS3234_read(0x06);
      if((year & 0xF0) >= 0x10) year -= 0x10;
      else                      year += 0x90;
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 9) && (Ycursor == 10))	// in case of year
    { year = DS3234_read(0x06);
      if((year & 0x0F) >= 0x01) year -= 0x01;
      else                      year += 0x09;
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 11) && (Ycursor == 10))	// in case of month
    { month = DS3234_read(0x05);
      month ^= 0x10;
      if(month == 0x00) month += 0x01;
      if(month > 0x12)  month = 0x12;
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 12) && (Ycursor == 10))	// in case of month
    { month = DS3234_read(0x05);
      if((month & 0x0F) >= 0x01)
        { month -= 0x01;
          if(month == 0x00) month = 0x09;
	}
      else
        { month += 0x09;
	  if(month > 0x12) month = 0x12;
        }
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 14) && (Ycursor == 10))	// in case of date
    { date = DS3234_read(0x04);
      if((date & 0xF0) >= 0x10)
        { date -= 0x10;
          if(date == 0x00) date = 0x01;
	}
      else
        { date += 0x30;
	  if(date > 0x31) date = 0x31;
        }
      DS3234_write(0x04,date);
    }
  else if((Xcursor == 15) && (Ycursor == 10))	// in case of date
    { date = DS3234_read(0x04);
      if((date & 0x0F) >= 0x01)
        { date -= 0x01;
	  if(date == 0x00) date = 0x09;
	}
      else
        { date += 0x09;
	  if(date > 0x31) date = 0x31;
        }
      DS3234_write(0x04,date);
    }
  else if((Xcursor == 18) && (Ycursor == 10))	// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday > 0x01) weekday -= 0x01;
      else               weekday = 0x07;
      DS3234_write(0x03,weekday);
    }
  else if((Xcursor == 9) && (Ycursor == 15))	// in case of AM/PM
    { hour = DS3234_read(0x02);
      hour ^= 0x20;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 12) && (Ycursor == 15))	// in case of hour
    { hour = DS3234_read(0x02);
      hour ^= 0x10;
      if((hour & 0x1F) == 0x00) hour += 0x01;
      if((hour & 0x1F) > 0x12)  hour = (hour & 0x60) + 0x12;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 13) && (Ycursor == 15))	// in case of hour
    { hour = DS3234_read(0x02);
      if((hour & 0x0F) >= 0x01)
        { hour -= 0x01;
	  if((hour & 0x1F) == 0x00) hour = (hour & 0xE0) + 0x09;
	}
      else
        { hour += 0x09;
	  if((hour & 0x1F) > 0x12) hour = (hour & 0xE0) + 0x12;
        }
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 15) && (Ycursor == 15))	// in case of minute
    { minute = DS3234_read(0x01);
      if((minute & 0xF0) >= 0x10) minute -= 0x10;
      else                        minute += 0x50;
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 16) && (Ycursor == 15))	// in case of minute
    { minute = DS3234_read(0x01);
      if((minute & 0x0F) >= 0x01) minute -= 0x01;
      else                        minute += 0x09;
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 18) && (Ycursor == 15))	// in case of second
    { second = DS3234_read(0x00);
      if((second & 0xF0) >= 0x10) second -= 0x10;
      else                        second += 0x50;
      DS3234_write(0x00,second);
    }
  else if((Xcursor == 19) && (Ycursor == 15))	// in case of second
    { second = DS3234_read(0x00);
      if((second & 0x0F) >= 0x01) second -= 0x01;
      else                        second += 0x09;
      DS3234_write(0x00,second);
    }
}

/* ----- 화면3 관련 함수 (알람 설정) ------------------------------------------ */

void Screen3_initialize(void)			/* initialize screen 3 */
{
  TFT_clear_screen();				// clear screen

  TFT_string(3,25,White,Blue,"화면");		// display key function
  TFT_string(3,27,White,Blue,"변경");
  TFT_string(13,25,White,Brown,"커서");
  TFT_string(13,27,White,Brown,"이동");
  TFT_string(23,25,White,Magenta," 값 ");
  TFT_string(23,27,White,Magenta,"증가");
  TFT_string(33,25,White,Magenta," 값 ");
  TFT_string(33,27,White,Magenta,"감소");

  Rectangle(18,194, 61,237, Cyan);
  Rectangle(19,195, 60,236, Cyan);
  Rectangle(98,194, 141,237, Cyan);
  Rectangle(99,195, 140,236, Cyan);
  Rectangle(178,194, 221,237, Cyan);
  Rectangle(179,195, 220,236, Cyan);
  Rectangle(258,194, 301,237, Cyan);
  Rectangle(259,195, 300,236, Cyan);

  TFT_string(0,3,White,Blue,"               알람 설정                ");

  TFT_string_large(8,10,Green,Black," 알람 OFF ");
  TFT_string_large(8,15,Green,Black,"오전 12:00");

  TFT_cursor(Magenta);				// display cursor
  Xcursor = 16;
  Ycursor = 10;

  Alarm_initialize();				// initialize alarm of DS3234 RTC
}

void Alarm_initialize(void)			/* initialize alarm of DS3234 RTC */
{
  unsigned char A1M123, A1M4;

  A1M123 = DS3234_read(0x07) & 0x80;		// read alarm_set
  A1M123 &= (DS3234_read(0x08) & 0x80);
  A1M123 &= (DS3234_read(0x09) & 0x80);
  A1M4 = (DS3234_read(0x0A) & 0x80);

  if((A1M123 == 0x00) && (A1M4 == 0x80))	// alarm = on
    alarm_set = 0x00;
  else if((A1M123 == 0x80) && (A1M4 == 0x80))	// alarm = off
    alarm_set = 0x80;
  else
    alarm_set = 0x80;

  if(alarm_set == 0x80)				// if alarm off, initialize alarm value
    { DS3234_write(0x07, 0x80 + 0x00);		// second = 0x00
      DS3234_write(0x08, 0x80 + 0x00);		// minute = 0x00
      DS3234_write(0x09, 0x80 + 0x52);		// hour = 0x12 AM
      DS3234_write(0x0A, 0x80 + 0x01);		// date = 0x01
      DS3234_write(0x0A, 0x80 + 0x41);		// weekday = 0x01
    }

  alarm_ampm = DS3234_read(0x09) & 0x60;	// read alarm_ampm
  alarm_hour = DS3234_read(0x09) & 0x1F;	// read alarm_hour
  alarm_minute = DS3234_read(0x08) & 0x7F;	// read alarm_minute
}

void Display_screen3_alarm(void)		/* display alarm time on screen 3 */
{
  if(alarm_set == 0x80)				// display alarm on/off
    TFT_string_large(14,10,White,Black,"OFF");
  else
    TFT_string_large(14,10,White,Black," ON");

  if(alarm_ampm == 0x40) TFT_string_large(8,15,White,Black,"오전"); // display alarm AM/PM
  else                   TFT_string_large(8,15,White,Black,"오후");
  TFT_xy(13,15);				// display alarm hour
  TFT_hexadecimal_large(alarm_hour,2);
  TFT_xy(16,15);				// display alarm minute
  TFT_hexadecimal_large(alarm_minute,2);
}

void Screen3_cursor_right(void)			/* move cursor to right */
{
  if((Xcursor == 16) && (Ycursor == 10))
    { Xcursor = 10;
      Ycursor = 15;
    }
  else if((Xcursor == 10) && (Ycursor == 15))
    { Xcursor = 13;
      Ycursor = 15;
    }
  else if((Xcursor == 13) && (Ycursor == 15))
    { Xcursor = 14;
      Ycursor = 15;
    }
  else if((Xcursor == 14) && (Ycursor == 15))
    { Xcursor = 16;
      Ycursor = 15;
    }
  else if((Xcursor == 16) && (Ycursor == 15))
    { Xcursor = 17;
      Ycursor = 15;
    }
  else if((Xcursor == 17) && (Ycursor == 15))
    { Xcursor = 16;
      Ycursor = 10;
    }
}

void Screen3_increment(void)			/* increment value */
{
  if((Xcursor == 16) && (Ycursor == 10))	// in case of alarm_set
    { if(alarm_set == 0x80) alarm_set = 0x00;
      else                  alarm_set = 0x80;
      DS3234_alarm_set();
    }
  else if((Xcursor == 10) && (Ycursor == 15))	// in case of alarm_ampm
    { if(alarm_ampm == 0x40) alarm_ampm = 0x60;
      else                   alarm_ampm = 0x40;
    }
  else if((Xcursor == 13) && (Ycursor == 15))	// in case of hour
    { alarm_hour ^= 0x10;
      if((alarm_hour & 0x1F) == 0x00) alarm_hour += 0x01;
      if((alarm_hour & 0x1F) > 0x12)  alarm_hour = (alarm_hour & 0x60) + 0x12;
    }
  else if((Xcursor == 14) && (Ycursor == 15))	// in case of hour
    { if((alarm_hour & 0x0F) < 0x09)
        { alarm_hour += 0x01;
	  if((alarm_hour & 0x1F) > 0x12) alarm_hour &= 0xF0;
	}
      else
        { alarm_hour &= 0xF0;
	  if((alarm_hour & 0x1F) == 0x00) alarm_hour = (alarm_hour & 0x60) + 0x01;
        }
    }
  else if((Xcursor == 16) && (Ycursor == 15))	// in case of minute
    { if((alarm_minute & 0xF0) < 0x50) alarm_minute += 0x10;
      else                             alarm_minute &= 0x0F;
    }
  else if((Xcursor == 17) && (Ycursor == 15))	// in case of minute
    { if((alarm_minute & 0x0F) < 0x09) alarm_minute += 0x01;
      else                             alarm_minute &= 0xF0;
    }
}

void Screen3_decrement(void)			/* decrement value */
{
  if((Xcursor == 16) && (Ycursor == 10))	// in case of alarm_set
    { if(alarm_set == 0x80) alarm_set = 0x00;
      else                  alarm_set = 0x80;
      DS3234_alarm_set();
    }
  else if((Xcursor == 10) && (Ycursor == 15))	// in case of alarm_ampm
    { if(alarm_ampm == 0x40) alarm_ampm = 0x60;
      else                   alarm_ampm = 0x40;
    }
  else if((Xcursor == 13) && (Ycursor == 15))	// in case of hour
    { alarm_hour ^= 0x10;
      if((alarm_hour & 0x1F) == 0x00) alarm_hour += 0x01;
      if((alarm_hour & 0x1F) > 0x12)  alarm_hour = (alarm_hour & 0x60) + 0x12;
    }
  else if((Xcursor == 14) && (Ycursor == 15))	// in case of hour
    { if((alarm_hour & 0x0F) >= 0x01)
        { alarm_hour -= 0x01;
	  if((alarm_hour & 0x1F) == 0x00) alarm_hour = (alarm_hour & 0xE0) + 0x09;
	}
      else
        { alarm_hour += 0x09;
	  if((alarm_hour & 0x1F) > 0x12) alarm_hour = (alarm_hour & 0xE0) + 0x12;
        }
    }
  else if((Xcursor == 16) && (Ycursor == 15))	// in case of minute
    { if((alarm_minute & 0xF0) >= 0x10) alarm_minute -= 0x10;
      else                              alarm_minute += 0x50;
    }
  else if((Xcursor == 17) && (Ycursor == 15))	// in case of minute
    { if((alarm_minute & 0x0F) >= 0x01) alarm_minute -= 0x01;
      else                              alarm_minute += 0x09;
    }
}

void DS3234_alarm_set(void)			/* set alarm of DS3234 RTC */
{
  if(alarm_set == 0x80)
    { DS3234_write(0x07, 0x80);			// second = alarm off
      DS3234_write(0x08, 0x80 + alarm_minute);	// minute = alarm off
      DS3234_write(0x09, 0x80 + alarm_ampm + alarm_hour); // hour = alarm off
      DS3234_write(0x0A, 0x80 + 0x01);		// date = alarm off
      DS3234_write(0x0A, 0x80 + 0x41);		// weekday = alarm off
      DS3234_write(0x0F, 0x00);			// control register
    }
  else
    { DS3234_write(0x07, 0x00);			// second = 00 = alarm on
      DS3234_write(0x08, alarm_minute);		// minute = alarm on
      DS3234_write(0x09, alarm_ampm + alarm_hour); // hour = alarm on
      DS3234_write(0x0A, 0x80 + 0x01);		// date = alarm off
      DS3234_write(0x0A, 0x80 + 0x41);		// weekday = alarm off
      DS3234_write(0x0F, 0x00);			// control register
    }
}
