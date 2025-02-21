/* ============================================================================ */
/*    Exp17_4_digital_vlarge.c : 매우 큰 글씨로 표시한 디지털 시계 프로그그램	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"
#include "OK-STM767_xlarge.h"

void Display_time(void);			/* display current time */
void Time_increment(void);			/* increment time */
void Time_decrement(void);			/* decrement time */
void Cursor_left(void);				/* go cursor left */
void Cursor_right(void);			/* go cursor right */
unsigned char Icon_input(void);			/* input touch screen icon */

unsigned char year, month, date, weekday, hour, minute, second, second_old;
unsigned char blink_count = 0, hour_flag = 0;

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned int key = no_key;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  Initialize_touch_screen();			// initialize touch screen
  DS3234_initialize();				// initialize DS3234 RTC

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp17_4.c    ");
  Beep();

  TFT_string( 0,1,Green,  Black,"****************************************");
  TFT_string( 0,3,Magenta,Black,"       OK-STM767 키트 디지털 시계       ");
  TFT_string( 0,5,Green,  Black,"****************************************");

  TFT_string_large(6,8,Cyan,Black,"2017/01/01 (일)");
  TFT_string_large(2,13,Cyan,Black,"A");
  TFT_string_large(2,16,Cyan,Black,"M");
  Rectangle(12,96, 307,157, Cyan);
  Rectangle(13,97, 306,156, Cyan);
  TFT_xy(5,13);
  TFT_color(White,Black);
  TFT_hexadecimal_vlarge(0,2);
  TFT_English_vlarge(':');
  TFT_hexadecimal_vlarge(0,2);
  TFT_English_vlarge(':');
  TFT_hexadecimal_vlarge(0,2);

  TFT_cursor(Magenta);				// display cursor
  Xcursor = 39;
  Ycursor = 17;

  TFT_string(3,25,White,Blue,"커서");		// display key function
  TFT_string(3,27,White,Blue," 좌 ");
  TFT_string(13,25,White,Blue,"커서");
  TFT_string(13,27,White,Blue," 우 ");
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

          blink_count = 0;			// start colon blink count
        }

      if((minute == 0x00) && (second == 0x00) && (hour_flag == 0)) // 매시 정각 경보
        { hour_flag = 1;
	  Beep();
	}
      else if(second >= 0x01)
        hour_flag = 0;

      key = Key_input();			// key input
      if(key == no_key)				// if no key input, read touch screen icon
        key = Icon_input();

      switch(key)
        { case KEY1 : Cursor_left();		// if KEY1, go cursor left
                      break;
          case KEY2 : Cursor_right();		// if KEY2, go cursor right
                      break;
          case KEY3 : Time_increment();		// if KEY3, increment time
                      break;
          case KEY4 : Time_decrement();		// if KEY4, decrement time
                      break;
        }

      Display_time();				// display current time
      if(blink_count < 80)			// display colon for 0.5 sec
	{ TFT_color(Cyan,Black);
	  TFT_xy(13,13);
	  TFT_English_vlarge(':');
          TFT_xy(25,13);
	  TFT_English_vlarge(':');
        }
      else
	{ TFT_color(Cyan,Black);
          TFT_xy(13,13);
          TFT_English_vlarge(' ');
          TFT_xy(25,13);
          TFT_English_vlarge(' ');
        }

      Delay_ms(5);				// delay count for blink time
      blink_count++;
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Display_time(void)				/* display current time */
{
  TFT_color(Yellow,Black);			// display color
  TFT_xy(6,8);					// display year
  TFT_hexadecimal_large(0x20,2);
  TFT_hexadecimal_large(year,2);
  TFT_xy(11,8);		  			// display month
  TFT_hexadecimal_large(month,2);
  TFT_xy(14,8);					// display date
  TFT_hexadecimal_large(date,2);
  if(weekday == 0x01)      TFT_string_large(18,8, Yellow,Black, "일"); // display weekday
  else if(weekday == 0x02) TFT_string_large(18,8, Yellow,Black, "월");
  else if(weekday == 0x03) TFT_string_large(18,8, Yellow,Black, "화");
  else if(weekday == 0x04) TFT_string_large(18,8, Yellow,Black, "수");
  else if(weekday == 0x05) TFT_string_large(18,8, Yellow,Black, "목");
  else if(weekday == 0x06) TFT_string_large(18,8, Yellow,Black, "금");
  else if(weekday == 0x07) TFT_string_large(18,8, Yellow,Black, "토");

  if((hour & 0x20) == 0x00) TFT_string_large(2,13,Cyan,Black,"A"); // display AM/PM
  else                      TFT_string_large(2,13,Cyan,Black,"P");

  TFT_color(White,Black);			// display color
  TFT_xy(5,13);					// display hour
  TFT_hexadecimal_vlarge(hour & 0x1F,2);
  TFT_xy(17,13);				// display minute
  TFT_hexadecimal_vlarge(minute,2);
  TFT_xy(29,13);				// display second
  TFT_hexadecimal_vlarge(second,2);

  TFT_string(39,17, White,Black, " ");		// display home
}

void Cursor_left(void)			        /* go cursor left */
{
  if     ((Xcursor == 39) && (Ycursor == 17)) { Xcursor = 33; Ycursor = 13; }
  else if((Xcursor == 33) && (Ycursor == 13)) { Xcursor = 29; Ycursor = 13; }
  else if((Xcursor == 29) && (Ycursor == 13)) { Xcursor = 21; Ycursor = 13; }
  else if((Xcursor == 21) && (Ycursor == 13)) { Xcursor = 17; Ycursor = 13; }
  else if((Xcursor == 17) && (Ycursor == 13)) { Xcursor = 9;  Ycursor = 13; }
  else if((Xcursor == 9)  && (Ycursor == 13)) { Xcursor = 5;  Ycursor = 13; }
  else if((Xcursor == 5)  && (Ycursor == 13)) { Xcursor = 2;  Ycursor = 13; }
  else if((Xcursor == 2)  && (Ycursor == 13)) { Xcursor = 18; Ycursor = 8; }
  else if((Xcursor == 18) && (Ycursor == 8))  { Xcursor = 15; Ycursor = 8; }
  else if((Xcursor == 15) && (Ycursor == 8))  { Xcursor = 14; Ycursor = 8; }
  else if((Xcursor == 14) && (Ycursor == 8))  { Xcursor = 12; Ycursor = 8; }
  else if((Xcursor == 12) && (Ycursor == 8))  { Xcursor = 11; Ycursor = 8; }
  else if((Xcursor == 11) && (Ycursor == 8))  { Xcursor = 9;  Ycursor = 8; }
  else if((Xcursor == 9)  && (Ycursor == 8))  { Xcursor = 8;  Ycursor = 8; }
  else if((Xcursor == 8)  && (Ycursor == 8))  { Xcursor = 39; Ycursor = 17; }
}

void Cursor_right(void)				/* go cursor right */
{
  if     ((Xcursor == 39) && (Ycursor == 17)) { Xcursor = 8;  Ycursor = 8; }
  else if((Xcursor == 8)  && (Ycursor == 8))  { Xcursor = 9;  Ycursor = 8; }
  else if((Xcursor == 9)  && (Ycursor == 8))  { Xcursor = 11; Ycursor = 8; }
  else if((Xcursor == 11) && (Ycursor == 8))  { Xcursor = 12; Ycursor = 8; }
  else if((Xcursor == 12) && (Ycursor == 8))  { Xcursor = 14; Ycursor = 8; }
  else if((Xcursor == 14) && (Ycursor == 8))  { Xcursor = 15; Ycursor = 8; }
  else if((Xcursor == 15) && (Ycursor == 8))  { Xcursor = 18; Ycursor = 8; }
  else if((Xcursor == 18) && (Ycursor == 8))  { Xcursor = 2;  Ycursor = 13; }
  else if((Xcursor == 2)  && (Ycursor == 13)) { Xcursor = 5;  Ycursor = 13; }
  else if((Xcursor == 5)  && (Ycursor == 13)) { Xcursor = 9;  Ycursor = 13; }
  else if((Xcursor == 9)  && (Ycursor == 13)) { Xcursor = 17; Ycursor = 13; }
  else if((Xcursor == 17) && (Ycursor == 13)) { Xcursor = 21; Ycursor = 13; }
  else if((Xcursor == 21) && (Ycursor == 13)) { Xcursor = 29; Ycursor = 13; }
  else if((Xcursor == 29) && (Ycursor == 13)) { Xcursor = 33; Ycursor = 13; }
  else if((Xcursor == 33) && (Ycursor == 13)) { Xcursor = 39; Ycursor = 17; }
}

void Time_increment(void)			/* time increment */
{
  if((Xcursor == 8) && (Ycursor == 8))		// in case of year
    { year = DS3234_read(0x06);
      if((year & 0xF0) < 0x90) year += 0x10;
      else                     year &= 0x0F;
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 9) && (Ycursor == 8))	// in case of year
    { year = DS3234_read(0x06);
      if((year & 0x0F) < 0x09) year += 0x01;
      else                     year &= 0xF0;
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 11) && (Ycursor == 8))	// in case of month
    { month = DS3234_read(0x05);
      month ^= 0x10;
      if(month == 0x00) month += 0x01;
      if(month > 0x12)  month = 0x12;
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 12) && (Ycursor == 8))	// in case of month
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
  else if((Xcursor == 14) && (Ycursor == 8))	// in case of date
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
  else if((Xcursor == 15) && (Ycursor == 8))	// in case of date
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
  else if((Xcursor == 18) && (Ycursor == 8))	// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday < 0x07) weekday += 0x01;
      else               weekday = 0x01;
      DS3234_write(0x03,weekday);
    }
  else if((Xcursor == 2) && (Ycursor == 13))	// in case of AM/PM
    { hour = DS3234_read(0x02);
      hour ^= 0x20;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 5) && (Ycursor == 13))	// in case of hour
    { hour = DS3234_read(0x02);
      hour ^= 0x10;
      if((hour & 0x1F) == 0x00) hour += 0x01;
      if((hour & 0x1F) > 0x12)  hour = (hour & 0x60) + 0x12;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 9) && (Ycursor == 13))	// in case of hour
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
  else if((Xcursor == 17) && (Ycursor == 13))	// in case of minute
    { minute = DS3234_read(0x01);
      if((minute & 0xF0) < 0x50) minute += 0x10;
      else                       minute &= 0x0F;
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 21) && (Ycursor == 13))	// in case of minute
    { minute = DS3234_read(0x01);
      if((minute & 0x0F) < 0x09) minute += 0x01;
      else                       minute &= 0xF0;
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 29) && (Ycursor == 13))	// in case of second
    { second = DS3234_read(0x00);
      if((second & 0xF0) < 0x50) second += 0x10;
      else                       second &= 0x0F;
      DS3234_write(0x00,second);
    }
  else if((Xcursor == 33) && (Ycursor == 13))	// in case of second
    { second = DS3234_read(0x00);
      if((second & 0x0F) < 0x09) second += 0x01;
      else                       second &= 0xF0;
      DS3234_write(0x00,second);
    }
}

void Time_decrement(void)			/* time decrement */
{
  if((Xcursor == 8) && (Ycursor == 8))		// in case of year
    { year = DS3234_read(0x06);
      if((year & 0xF0) >= 0x10) year -= 0x10;
      else                      year += 0x90;
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 9) && (Ycursor == 8))	// in case of year
    { year = DS3234_read(0x06);
      if((year & 0x0F) >= 0x01) year -= 0x01;
      else                      year += 0x09;
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 11) && (Ycursor == 8))	// in case of month
    { month = DS3234_read(0x05);
      month ^= 0x10;
      if(month == 0x00) month += 0x01;
      if(month > 0x12)  month = 0x12;
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 12) && (Ycursor == 8))	// in case of month
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
  else if((Xcursor == 14) && (Ycursor == 8))	// in case of date
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
  else if((Xcursor == 15) && (Ycursor == 8))	// in case of date
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
  else if((Xcursor == 18) && (Ycursor == 8))	// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday > 0x01) weekday -= 0x01;
      else               weekday = 0x07;
      DS3234_write(0x03,weekday);
    }
  else if((Xcursor == 2) && (Ycursor == 13))	// in case of AM/PM
    { hour = DS3234_read(0x02);
      hour ^= 0x20;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 5) && (Ycursor == 13))	// in case of hour
    { hour = DS3234_read(0x02);
      hour ^= 0x10;
      if((hour & 0x1F) == 0x00) hour += 0x01;
      if((hour & 0x1F) > 0x12)  hour = (hour & 0x60) + 0x12;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 9) && (Ycursor == 13))	// in case of hour
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
  else if((Xcursor == 17) && (Ycursor == 13))	// in case of minute
    { minute = DS3234_read(0x01);
      if((minute & 0xF0) >= 0x10) minute -= 0x10;
      else                        minute += 0x50;
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 21) && (Ycursor == 13))	// in case of minute
    { minute = DS3234_read(0x01);
      if((minute & 0x0F) >= 0x01) minute -= 0x01;
      else                        minute += 0x09;
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 29) && (Ycursor == 13))	// in case of second
    { second = DS3234_read(0x00);
      if((second & 0xF0) >= 0x10) second -= 0x10;
      else                        second += 0x50;
      DS3234_write(0x00,second);
    }
  else if((Xcursor == 33) && (Ycursor == 13))	// in case of second
    { second = DS3234_read(0x00);
      if((second & 0x0F) >= 0x01) second -= 0x01;
      else                        second += 0x09;
      DS3234_write(0x00,second);
    }
}

unsigned char icon_flag = 0;

unsigned char Icon_input(void)			/* input touch screen icon */
{
  unsigned char icon;

  Touch_screen_input();				// input touch screen

  if((icon_flag == 0) && (x_touch >= 18) && (x_touch <= 61) && (y_touch >= 196) && (y_touch <= 234))
    { icon = KEY1;
      icon_flag = 1;
      Rectangle(18,194, 61,237, Magenta);
      Rectangle(19,195, 60,236, Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 98) && (x_touch <= 141) && (y_touch >= 196) && (y_touch <= 234))
    { icon = KEY2;
      icon_flag = 1;
      Rectangle(98,194, 141,237, Magenta);
      Rectangle(99,195, 140,236, Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 178) && (x_touch <= 221) && (y_touch >= 196) && (y_touch <= 234))
    { icon = KEY3;
      icon_flag = 1;
      Rectangle(178,194, 221,237, Magenta);
      Rectangle(179,195, 220,236, Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 258) && (x_touch <= 301) && (y_touch >= 196) && (y_touch <= 234))
    { icon = KEY4;
      icon_flag = 1;
      Rectangle(258,194, 301,237, Magenta);
      Rectangle(259,195, 300,236, Magenta);
      Beep();
    }
  else if((icon_flag == 1) && (x_touch == 0) && (y_touch == 0))
    { icon = no_key;
      icon_flag = 0;
      Rectangle(18,194, 61,237, Cyan);
      Rectangle(19,195, 60,236, Cyan);
      Rectangle(98,194, 141,237, Cyan);
      Rectangle(99,195, 140,236, Cyan);
      Rectangle(178,194, 221,237, Cyan);
      Rectangle(179,195, 220,236, Cyan);
      Rectangle(258,194, 301,237, Cyan);
      Rectangle(259,195, 300,236, Cyan);
      Delay_ms(50);
    }
  else
    icon = no_key;

  return icon;
}
