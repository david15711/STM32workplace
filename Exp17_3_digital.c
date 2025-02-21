/* ============================================================================ */
/*	   Exp17_3_digital.c : DS3234를 이용한 디지털 시계 프로그그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"

void Digital_display_time(void);		/* display DS3234 time */
unsigned char BCD_increment(U08 number);	/* BCD increment */
unsigned char BCD_decrement(U08 number);	/* BCD decrement */
void Digital_cursor_left(void);			/* go cursor left */
void Digital_cursor_right(void);		/* go cursor right */
void Digital_increment(void);			/* increment time */
void Digital_decrement(void);			/* decrement time */
unsigned char Icon_input(void);			/* input touch screen icon */

/* ----- 메인 프로그램 -------------------------------------------------------- */

unsigned char year, month, date, weekday, hour, minute, second, second_old;
unsigned char beep_flag = 0;

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
  LCD_string(0xC0,"   Exp17_3.c    ");
  Beep();

  Rectangle(0,0, 319,239, Green);		// display title
  Rectangle(1,1, 318,238, Green);

  TFT_string( 5, 4,White,Magenta," DS3234를 이용한  디지털 시계 ");
  TFT_string(10,12,White,  Black,"2017년 01월 01일 (일)");
  TFT_string_large(7,15,White,Black,"오전 12:00:00");

  TFT_cursor(Red);				// display cursor
  Xcursor = 38;
  Ycursor = 16;

  while(1)
    { key = Key_input();			// key input
      if(key == no_key)				// if no key input, read touch screen icon
        key = Icon_input();

      switch(key)
        { case KEY1 : Digital_cursor_left();	// if KEY1, go cursor left
                      break;
          case KEY2 : Digital_cursor_right();	// if KEY2, go cursor right
                      break;
          case KEY3 : Digital_increment();	// if KEY3, increment time
                      break;
          case KEY4 : Digital_decrement();	// if KEY4, decrement time
                      break;
	  default   : Digital_display_time();	// display time
	              break;
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Digital_display_time(void)			/* display DS3234 time */
{
  TFT_xy(10,12); TFT_color(Cyan,Black);		// display year
  TFT_hexadecimal(0x20, 2);
  year = DS3234_read(0x06);
  TFT_hexadecimal(year,2);
  TFT_xy(17,12); TFT_color(Cyan,Black);		// display month
  month = DS3234_read(0x05);
  TFT_hexadecimal(month,2);
  TFT_xy(22,12); TFT_color(Cyan,Black);		// display date
  date = DS3234_read(0x04);
  TFT_hexadecimal(date,2);
  weekday = DS3234_read(0x03);			// display weekday
  if(weekday == 0x01)      TFT_string(28,12,Cyan,Black,"일");
  else if(weekday == 0x02) TFT_string(28,12,Cyan,Black,"월");
  else if(weekday == 0x03) TFT_string(28,12,Cyan,Black,"화");
  else if(weekday == 0x04) TFT_string(28,12,Cyan,Black,"수");
  else if(weekday == 0x05) TFT_string(28,12,Cyan,Black,"목");
  else if(weekday == 0x06) TFT_string(28,12,Cyan,Black,"금");
  else if(weekday == 0x07) TFT_string(28,12,Cyan,Black,"토");
  TFT_xy(12,15); TFT_color(Green,Black);	// display hour
  hour = DS3234_read(0x02);
  TFT_hexadecimal_large(hour & 0x1F,2);
  if((hour & 0x20) == 0x00) TFT_string_large(7,15,White,Black,"오전");
  else                      TFT_string_large(7,15,White,Black,"오후");
  TFT_xy(15,15); TFT_color(Green,Black);	// display minute
  minute = DS3234_read(0x01);
  TFT_hexadecimal_large(minute,2);
  TFT_xy(18,15); TFT_color(Green,Black);	// display second
  second = DS3234_read(0x00);
  TFT_hexadecimal_large(second & 0x7F,2);
  TFT_xy(38,16); TFT_color(Green,Black);	// display home
  TFT_English(' ');

  TFT_string( 3,25,White,Blue,"KEY1");		// display icon
  TFT_string(13,25,White,Blue,"KEY2");
  TFT_string(23,25,White,Blue,"KEY3");
  TFT_string(33,25,White,Blue,"KEY4");
  TFT_string( 3,27,Red, Black,"좌측");
  TFT_string(13,27,Red, Black,"우측");
  TFT_string(23,27,Red, Black,"증가");
  TFT_string(33,27,Red, Black,"감소");

  Rectangle( 18,196,  61,234, Cyan);
  Rectangle( 98,196, 141,234, Cyan);
  Rectangle(178,196, 221,234, Cyan);
  Rectangle(258,196, 301,234, Cyan);

  if((minute == 0x00) && (second == 0x00) && (beep_flag == 0))
    { beep_flag = 1;
      Beep();
    }
  else if(second != 0x00)
    beep_flag = 0;
}

unsigned char BCD_increment(U08 number)		/* BCD increment */
{
  unsigned char i;

  i = (number >> 4)*10 + (number & 0x0F);       // convert BCD to binary
  i++;                                          // increment
  i = ((i/10) << 4) + (i % 10);                 // convert binary to BCD
  return i;
}

unsigned char BCD_decrement(U08 number)		/* BCD decrement */
{
  unsigned char i;

  i = (number >> 4)*10 + (number & 0x0F);       // convert BCD to binary
  i--;                                          // decrement
  i = ((i/10) << 4) + (i % 10);                 // convert binary to BCD
  return i;
}

void Digital_cursor_left(void)		        /* go cursor left */
{
  if     ((Xcursor == 38) && (Ycursor == 16)) { Xcursor = 19; Ycursor = 15; }
  else if((Xcursor == 19) && (Ycursor == 15)) { Xcursor = 16; Ycursor = 15; }
  else if((Xcursor == 16) && (Ycursor == 15)) { Xcursor = 13; Ycursor = 15; }
  else if((Xcursor == 13) && (Ycursor == 15)) { Xcursor =  9; Ycursor = 15; }
  else if((Xcursor ==  9) && (Ycursor == 15)) { Xcursor = 28; Ycursor = 12; }
  else if((Xcursor == 28) && (Ycursor == 12)) { Xcursor = 23; Ycursor = 12; }
  else if((Xcursor == 23) && (Ycursor == 12)) { Xcursor = 18; Ycursor = 12; }
  else if((Xcursor == 18) && (Ycursor == 12)) { Xcursor = 13; Ycursor = 12; }
  else if((Xcursor == 13) && (Ycursor == 12)) { Xcursor = 38; Ycursor = 16; }
}

void Digital_cursor_right(void)			/* go cursor right */
{
  if     ((Xcursor == 38) && (Ycursor == 16)) { Xcursor = 13; Ycursor = 12; }
  else if((Xcursor == 13) && (Ycursor == 12)) { Xcursor = 18; Ycursor = 12; }
  else if((Xcursor == 18) && (Ycursor == 12)) { Xcursor = 23; Ycursor = 12; }
  else if((Xcursor == 23) && (Ycursor == 12)) { Xcursor = 28; Ycursor = 12; }
  else if((Xcursor == 28) && (Ycursor == 12)) { Xcursor =  9; Ycursor = 15; }
  else if((Xcursor ==  9) && (Ycursor == 15)) { Xcursor = 13; Ycursor = 15; }
  else if((Xcursor == 13) && (Ycursor == 15)) { Xcursor = 16; Ycursor = 15; }
  else if((Xcursor == 16) && (Ycursor == 15)) { Xcursor = 19; Ycursor = 15; }
  else if((Xcursor == 19) && (Ycursor == 15)) { Xcursor = 38; Ycursor = 16; }
}

void Digital_increment(void)			/* increment time */
{
  if((Xcursor == 13) && (Ycursor == 12))	// in case of year
    { year = DS3234_read(0x06);
      if(year == 0x99)      year = 0x00;
      else                  year = BCD_increment(year);
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 18) && (Ycursor == 12))	// in case of month
    { month = DS3234_read(0x05);
      if(month == 0x12)     month = 0x01;
      else                  month = BCD_increment(month);
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 23) && (Ycursor == 12))	// in case of date
    { date = DS3234_read(0x04);
      if(date == 0x31)      date = 0x01;
      else                  date = BCD_increment(date);
      DS3234_write(0x04,date);
    }
  else if((Xcursor == 28) && (Ycursor == 12))	// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday == 0x07)   weekday = 0x01;
      else                  weekday++;
      DS3234_write(0x03,weekday);
    }
  else if((Xcursor == 9) && (Ycursor == 15))	// in case of AM/PM
    { hour = DS3234_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 13) && (Ycursor == 15))	// in case of hour
    { hour = DS3234_read(0x02);
      if((hour & 0x1F) == 0x12) hour = (hour & 0xE0) + 0x01;
      else                      hour = BCD_increment(hour);
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 16) && (Ycursor == 15))	// in case of minute
    { minute = DS3234_read(0x01);
      if(minute == 0x59)    minute = 0x00;
      else                  minute = BCD_increment(minute);
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 19) && (Ycursor == 15))	// in case of second
    { second = DS3234_read(0x00);
      if(second == 0x59)    second = 0x00;
      else                  second = BCD_increment(second);
      DS3234_write(0x00,second);
    }
}

void Digital_decrement(void)			/* decrement time */
{
  if((Xcursor == 13) && (Ycursor == 12))	// in case of year
    { year = DS3234_read(0x06);
      if(year == 0x00)      year = 0x99;
      else                  year = BCD_decrement(year);
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 18) && (Ycursor == 12))	// in case of month
    { month = DS3234_read(0x05);
      if(month == 0x01)     month = 0x12;
      else                  month = BCD_decrement(month);
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 23) && (Ycursor == 12))	// in case of date
    { date = DS3234_read(0x04);
      if(date == 0x01)      date = 0x31;
      else                  date = BCD_decrement(date);
      DS3234_write(0x04,date);
    }
  else if((Xcursor == 28) && (Ycursor == 12))	// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday == 0x01)   weekday = 0x07;
      else                  weekday--;
      DS3234_write(0x03,weekday);
    }
  else if((Xcursor == 9) && (Ycursor == 15))	// in case of AM/PM
    { hour = DS3234_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 13) && (Ycursor == 15))	// in case of hour
    { hour = DS3234_read(0x02);
      if((hour & 0x1F) == 0x01) hour = (hour & 0xE0) + 0x12;
      else                      hour = BCD_decrement(hour);
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 16) && (Ycursor == 15))	// in case of minute
    { minute = DS3234_read(0x01);
      if(minute == 0x00)    minute = 0x59;
      else                  minute = BCD_decrement(minute);
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 19) && (Ycursor == 15))	// in case of second
    { second = DS3234_read(0x00);
      if(second == 0x00)    second = 0x59;
      else                  second = BCD_decrement(second);
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
      Rectangle( 18,196,  61,234, Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 98) && (x_touch <= 141) && (y_touch >= 196) && (y_touch <= 234))
    { icon = KEY2;
      icon_flag = 1;
      Rectangle( 98,196, 141,234, Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 178) && (x_touch <= 221) && (y_touch >= 196) && (y_touch <= 234))
    { icon = KEY3;
      icon_flag = 1;
      Rectangle(178,196, 221,234, Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 258) && (x_touch <= 301) && (y_touch >= 196) && (y_touch <= 234))
    { icon = KEY4;
      icon_flag = 1;
      Rectangle(258,196, 301,234, Magenta);
      Beep();
    }
  else if((icon_flag == 1) && (x_touch == 0) && (y_touch == 0))
    { icon = no_key;
      icon_flag = 0;
      Rectangle( 18,196,  61,234, Cyan);
      Rectangle( 98,196, 141,234, Cyan);
      Rectangle(178,196, 221,234, Cyan);
      Rectangle(258,196, 301,234, Cyan);
      Delay_ms(50);
    }
  else
    icon = no_key;

  return icon;
}
