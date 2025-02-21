/* ============================================================================ */
/*    Exp17_2_Text-LCD(2).c : 텍스트 LCD에 표시한 디지털 시계 프로그그램(2)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void Display_time(void);			/* display DS3234 time */
unsigned char BCD_increment(U08 number);	/* BCD increment */
unsigned char BCD_decrement(U08 number);	/* BCD decrement */
void Cursor_left(void);				/* go cursor left */
void Cursor_right(void);			/* go cursor right */
void Digital_increment(void);			/* increment time */
void Digital_decrement(void);			/* decrement time */

/* ----- 메인 프로그램 -------------------------------------------------------- */

unsigned char cursor_position, year, month, date, weekday, hour, minute, second;
unsigned char beep_flag = 0;

int main(void)
{
  unsigned char key;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  DS3234_initialize();                          // initialize DS3234
  Beep();

  cursor_position = 0xCF;			// initial cursor position
  LCD_command(0x0F);				// cursor ON

  LCD_string(0x80,"2017/01/01 (SUN)");		// display initial screen
  LCD_string(0xC0,"  12:00:00  AM  ");

  while(1)
    { key = Key_input();			// key input

      switch(key)
        { case KEY1 : Cursor_left();		// if KEY1, go cursor left
                      break;
          case KEY2 : Cursor_right();		// if KEY2, go cursor right
                      break;
          case KEY3 : Digital_increment();	// if KEY3, increment time
                      break;
	  case KEY4 : Digital_decrement();	// if KEY4, decrement time
                      break;
          default   : Display_time();		// display time
	              break;
        }
      Delay_ms(50);
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Display_time(void)				/* display DS3234 time */
{
  LCD_command(0x0C);				// cursor OFF

  LCD_command(0x80);				// display year
  LCD_hexadecimal(0x20,2);
  year = DS3234_read(0x06);
  LCD_hexadecimal(year,2);
  LCD_command(0x85);				// display month
  month = DS3234_read(0x05);
  LCD_hexadecimal(month,2);
  LCD_command(0x88);				// display date
  date = DS3234_read(0x04);
  LCD_hexadecimal(date,2);
  weekday = DS3234_read(0x03);			// display weekday
  if(weekday == 0x01)      LCD_string(0x8C,"SUN");
  else if(weekday == 0x02) LCD_string(0x8C,"MON");
  else if(weekday == 0x03) LCD_string(0x8C,"TUE");
  else if(weekday == 0x04) LCD_string(0x8C,"WED");
  else if(weekday == 0x05) LCD_string(0x8C,"THU");
  else if(weekday == 0x06) LCD_string(0x8C,"FRI");
  else if(weekday == 0x07) LCD_string(0x8C,"SAT");

  LCD_command(0xC2);				// display hour
  hour = DS3234_read(0x02);
  LCD_hexadecimal(hour & 0x1F,2);
  if((hour & 0x20) == 0x00) LCD_string(0xCC,"AM");
  else                      LCD_string(0xCC,"PM");
  LCD_command(0xC5);				// display minute
  minute = DS3234_read(0x01);
  LCD_hexadecimal(minute,2);
  LCD_command(0xC8);				// display second
  second = DS3234_read(0x00);
  LCD_hexadecimal(second & 0x7F,2);

  LCD_command(cursor_position);			// go cursor position
  LCD_command(0x0F);				// cursor ON

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

void Cursor_left(void)				/* go cursor left */
{
  if     (cursor_position == 0xCF) cursor_position = 0xCD;
  else if(cursor_position == 0xCD) cursor_position = 0xC9;
  else if(cursor_position == 0xC9) cursor_position = 0xC6;
  else if(cursor_position == 0xC6) cursor_position = 0xC3;
  else if(cursor_position == 0xC3) cursor_position = 0x8E;
  else if(cursor_position == 0x8E) cursor_position = 0x89;
  else if(cursor_position == 0x89) cursor_position = 0x86;
  else if(cursor_position == 0x86) cursor_position = 0x83;
  else if(cursor_position == 0x83) cursor_position = 0xCF;
}

void Cursor_right(void)				/* go cursor right */
{
  if     (cursor_position == 0xCF) cursor_position = 0x83;
  else if(cursor_position == 0x83) cursor_position = 0x86;
  else if(cursor_position == 0x86) cursor_position = 0x89;
  else if(cursor_position == 0x89) cursor_position = 0x8E;
  else if(cursor_position == 0x8E) cursor_position = 0xC3;
  else if(cursor_position == 0xC3) cursor_position = 0xC6;
  else if(cursor_position == 0xC6) cursor_position = 0xC9;
  else if(cursor_position == 0xC9) cursor_position = 0xCD;
  else if(cursor_position == 0xCD) cursor_position = 0xCF;
}

void Digital_increment(void)			/* increment time */
{
  if(cursor_position == 0x83)			// in case of year
    { year = DS3234_read(0x06);
      if(year == 0x99)      year = 0x00;
      else                  year = BCD_increment(year);
      DS3234_write(0x06,year);
    }
  else if(cursor_position == 0x86)		// in case of month
    { month = DS3234_read(0x05);
      if(month == 0x12)     month = 0x01;
      else                  month = BCD_increment(month);
      DS3234_write(0x05,month);
    }
  else if(cursor_position == 0x89)		// in case of date
    { date = DS3234_read(0x04);
      if(date == 0x31)      date = 0x01;
      else                  date = BCD_increment(date);
      DS3234_write(0x04,date);
    }
  else if(cursor_position == 0x8E)		// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday == 0x07)   weekday = 0x01;
      else                  weekday++;
      DS3234_write(0x03,weekday);
    }
  else if(cursor_position == 0xC3)		// in case of hour
    { hour = DS3234_read(0x02);
      if((hour & 0x1F) == 0x12) hour = (hour & 0xE0) + 0x01;
      else                      hour = BCD_increment(hour);
      DS3234_write(0x02,hour);
    }
  else if(cursor_position == 0xC6)		// in case of minute
    { minute = DS3234_read(0x01);
      if(minute == 0x59)    minute = 0x00;
      else                  minute = BCD_increment(minute);
      DS3234_write(0x01,minute);
    }
  else if(cursor_position == 0xC9)		// in case of second
    { second = DS3234_read(0x00);
      if(second == 0x59)    second = 0x00;
      else                  second = BCD_increment(second);
      DS3234_write(0x00,second);
    }
  else if(cursor_position == 0xCD)		// in case of AM/PM
    { hour = DS3234_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS3234_write(0x02,hour);
    }
}

void Digital_decrement(void)			/* decrement time */
{
  if(cursor_position == 0x83)			// in case of year
    { year = DS3234_read(0x06);
      if(year == 0x00)      year = 0x99;
      else                  year = BCD_decrement(year);
      DS3234_write(0x06,year);
    }
  else if(cursor_position == 0x86)		// in case of month
    { month = DS3234_read(0x05);
      if(month == 0x01)     month = 0x12;
      else                  month = BCD_decrement(month);
      DS3234_write(0x05,month);
    }
  else if(cursor_position == 0x89)		// in case of date
    { date = DS3234_read(0x04);
      if(date == 0x01)      date = 0x31;
      else                  date = BCD_decrement(date);
      DS3234_write(0x04,date);
    }
  else if(cursor_position == 0x8E)		// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday == 0x01)   weekday = 0x07;
      else                  weekday--;
      DS3234_write(0x03,weekday);
    }
  else if(cursor_position == 0xC3)		// in case of hour
    { hour = DS3234_read(0x02);
      if((hour & 0x1F) == 0x01) hour = (hour & 0xE0) + 0x12;
      else                      hour = BCD_decrement(hour);
      DS3234_write(0x02,hour);
    }
  else if(cursor_position == 0xC6)		// in case of minute
    { minute = DS3234_read(0x01);
      if(minute == 0x00)    minute = 0x59;
      else                  minute = BCD_decrement(minute);
      DS3234_write(0x01,minute);
    }
  else if(cursor_position == 0xC9)		// in case of second
    { second = DS3234_read(0x00);
      if(second == 0x00)    second = 0x59;
      else                  second = BCD_decrement(second);
      DS3234_write(0x00,second);
    }
  else if(cursor_position == 0xCD)		// in case of AM/PM
    { hour = DS3234_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS3234_write(0x02,hour);
    }
}
