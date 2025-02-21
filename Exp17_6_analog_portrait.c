/* ============================================================================ */
/*   Exp17_6_analog_portrait.c : DS3234를 이용한 아날로그 시계 - 초상화 모드	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

unsigned char BCD_increment(U08 number);	/* BCD increment */
unsigned char BCD_decrement(U08 number);	/* BCD decrement */
void Analog_cursor_left(void);			/* go cursor left */
void Analog_cursor_right(void);			/* go cursor right */
void Analog_increment(void);			/* increment time */
void Analog_decrement(void);			/* decrement time */
void Draw_analog_clock(void);			/* draw analog clock */
void Analog_display_time(void);			/* display analog and digital time */
void Draw_pin(void);				/* draw hour, minute, second pin */

/* ----- 메인 프로그램 -------------------------------------------------------- */

unsigned char year, month, date, weekday, hour, minute, second, second_old;
unsigned char start_flag, tick_flag = 0;

int main(void)
{
  unsigned int key = no_key;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  TFT_portrait_mode();				// portrait mode
  DS3234_initialize();				// initialize DS3234 RTC

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp17_6.c    ");
  Beep();

  TFT_cursor(Magenta);				// display cursor by Magenta color
  Xcursor = 28;
  Ycursor = 37;
  start_flag = 0;

  TFT_string(0,1, White,Magenta," DS3234에 의한  아날로그 시계 "); // display title
  Draw_analog_clock();				// draw analog clock
  TFT_string( 8,37, Cyan,  Black, "오전");	// display digital clock
  TFT_string(13,37, Silver,Black, "12:00:00");

  while(1)
    { key = Key_input();			// key input

      switch(key)
        { case KEY1 : Analog_cursor_left();	// if KEY1, go cursor left
                      break;
          case KEY2 : Analog_cursor_right();	// if KEY2, go cursor right
                      break;
          case KEY3 : Analog_increment();	// if KEY3, increment time
                      break;
          case KEY4 : Analog_decrement();	// if KEY4, decrement time
                      break;
	  default   : Analog_display_time();	// display time
		      break;
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

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

void Analog_cursor_left(void)			/* go cursor left */
{
  if     ((Xcursor == 28) && (Ycursor == 37)) { Xcursor = 20; Ycursor = 37; }
  else if((Xcursor == 20) && (Ycursor == 37)) { Xcursor = 17; Ycursor = 37; }
  else if((Xcursor == 17) && (Ycursor == 37)) { Xcursor = 14; Ycursor = 37; }
  else if((Xcursor == 14) && (Ycursor == 37)) { Xcursor = 10; Ycursor = 37; }
  else if((Xcursor == 10) && (Ycursor == 37)) { Xcursor = 19; Ycursor = 24; }

  else if((Xcursor == 19) && (Ycursor == 24)) { Xcursor = 17; Ycursor = 24; }
  else if((Xcursor == 17) && (Ycursor == 24)) { Xcursor = 14; Ycursor = 24; }
  else if((Xcursor == 14) && (Ycursor == 24)) { Xcursor = 11; Ycursor = 24; }
  else if((Xcursor == 11) && (Ycursor == 24)) { Xcursor = 28; Ycursor = 37; }
}

void Analog_cursor_right(void)			/* go cursor right */
{
  if     ((Xcursor == 28) && (Ycursor == 37)) { Xcursor = 11; Ycursor = 24; }
  else if((Xcursor == 11) && (Ycursor == 24)) { Xcursor = 14; Ycursor = 24; }
  else if((Xcursor == 14) && (Ycursor == 24)) { Xcursor = 17; Ycursor = 24; }
  else if((Xcursor == 17) && (Ycursor == 24)) { Xcursor = 19; Ycursor = 24; }
  else if((Xcursor == 19) && (Ycursor == 24)) { Xcursor = 10; Ycursor = 37; }

  else if((Xcursor == 10) && (Ycursor == 37)) { Xcursor = 14; Ycursor = 37; }
  else if((Xcursor == 14) && (Ycursor == 37)) { Xcursor = 17; Ycursor = 37; }
  else if((Xcursor == 17) && (Ycursor == 37)) { Xcursor = 20; Ycursor = 37; }
  else if((Xcursor == 20) && (Ycursor == 37)) { Xcursor = 28; Ycursor = 37; }
}

void Analog_increment(void)			/* increment time */
{
  if((Xcursor == 10) && (Ycursor == 37))	// in case of AM/PM
    { hour = DS3234_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 14) && (Ycursor == 37))	// in case of hour
    { hour = DS3234_read(0x02);
      if((hour & 0x1F) == 0x12) hour = (hour & 0xE0) + 0x01;
      else                      hour = BCD_increment(hour);
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 17) && (Ycursor == 37))	// in case of minute
    { minute = DS3234_read(0x01);
      if(minute == 0x59)    minute = 0x00;
      else                  minute = BCD_increment(minute);
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 20) && (Ycursor == 37))	// in case of second
    { second = DS3234_read(0x00);
      if(second == 0x59)    second = 0x00;
      else                  second = BCD_increment(second);
      DS3234_write(0x00,second);
    }
  else if((Xcursor == 11) && (Ycursor == 24))	// in case of year
    { year = DS3234_read(0x06);
      if(year == 0x99)      year = 0x00;
      else                  year = BCD_increment(year);
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 14) && (Ycursor == 24))	// in case of month
    { month = DS3234_read(0x05);
      if(month == 0x12)     month = 0x01;
      else                  month = BCD_increment(month);
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 17) && (Ycursor == 24))	// in case of date
    { date = DS3234_read(0x04);
      if(date == 0x31)      date = 0x01;
      else                  date = BCD_increment(date);
      DS3234_write(0x04,date);
    }
  else if((Xcursor == 19) && (Ycursor == 24))	// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday == 0x07)   weekday = 0x01;
      else                  weekday++;
      DS3234_write(0x03,weekday);
    }
}

void Analog_decrement(void)			/* decrement time */
{
  if ((Xcursor == 10) && (Ycursor == 37))	// in case of AM/PM
    { hour = DS3234_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 14) && (Ycursor == 37))	// in case of hour
    { hour = DS3234_read(0x02);
      if((hour & 0x1F) == 0x01) hour = (hour & 0xE0) + 0x12;
      else                      hour = BCD_decrement(hour);
      DS3234_write(0x02,hour);
    }
  else if((Xcursor == 17) && (Ycursor == 37))	// in case of minute
    { minute = DS3234_read(0x01);
      if(minute == 0x00)    minute = 0x59;
      else                  minute = BCD_decrement(minute);
      DS3234_write(0x01,minute);
    }
  else if((Xcursor == 20) && (Ycursor == 37))	// in case of second
    { second = DS3234_read(0x00);
      if(second == 0x00)    second = 0x59;
      else                  second = BCD_decrement(second);
      DS3234_write(0x00,second);
    }
  else if((Xcursor == 11) && (Ycursor == 24))	// in case of year
    { year = DS3234_read(0x06);
      if(year == 0x00)      year = 0x99;
      else                  year = BCD_decrement(year);
      DS3234_write(0x06,year);
    }
  else if((Xcursor == 14) && (Ycursor == 24))	// in case of month
    { month = DS3234_read(0x05);
      if(month == 0x01)     month = 0x12;
      else                  month = BCD_decrement(month);
      DS3234_write(0x05,month);
    }
  else if((Xcursor == 17) && (Ycursor == 24))	// in case of date
    { date = DS3234_read(0x04);
      if(date == 0x01)      date = 0x31;
      else                  date = BCD_decrement(date);
      DS3234_write(0x04,date);
    }
  else if((Xcursor == 19) && (Ycursor == 24))	// in case of weekday
    { weekday = DS3234_read(0x03);
      if(weekday == 0x01)   weekday = 0x07;
      else                  weekday--;
      DS3234_write(0x03,weekday);
    }
}

unsigned short Xcenter = 120;			// center x,y of analog clock
unsigned short Ycenter = 160;

void Draw_analog_clock(void)			/* draw analog clock */
{
 unsigned short i; 
 unsigned short scale_out = 118;
 unsigned short scale_in1 = 112;
 unsigned short scale_in2 = 106;
 signed short Xstart, Ystart, Xend, Yend;

 for(i = 0; i < 60 ; i++)			// draw clock scale
   if((i % 5) != 0)
     { Xstart = Xcenter + (signed short)(scale_in1*sin(3.141592/180*(i*6)));
       Ystart = Ycenter - (signed short)(scale_in1*cos(3.141592/180*(i*6)));
       Xend   = Xcenter + (signed short)(scale_out*sin(3.141592/180*(i*6)));
       Yend   = Ycenter - (signed short)(scale_out*cos(3.141592/180*(i*6)));

       Line(Xstart,Ystart, Xend,Yend, LightGrey);
     }
 for(i = 0; i < 12 ; i++)
   { Xstart = Xcenter + (signed short)(scale_in2*sin(3.141592/180*(i*30)));
     Ystart = Ycenter - (signed short)(scale_in2*cos(3.141592/180*(i*30)));
     Xend   = Xcenter + (signed short)(scale_out*sin(3.141592/180*(i*30)));
     Yend   = Ycenter - (signed short)(scale_out*cos(3.141592/180*(i*30)));

     Line(Xstart,Ystart, Xend,Yend, White);
   }

  Circle(Xcenter,Ycenter, 119, Cyan);		// draw outline
  Circle(Xcenter,Ycenter, 118, Cyan);

  for(i = 4; i > 0; i--)			// draw center
    Circle(Xcenter,Ycenter, i, Yellow);
}

void Analog_display_time(void)			/* display analog and digital time */
{
  second = DS3234_read(0x00);			// new second ?
  if(second == second_old)
    return;
  else
    second_old = second;

  minute = DS3234_read(0x01);
  hour = DS3234_read(0x02);
  weekday = DS3234_read(0x03);
  date = DS3234_read(0x04);
  month = DS3234_read(0x05);
  year = DS3234_read(0x06);

  TFT_xy(13,37);				// display hour
  TFT_color(Green,Black);
  TFT_hexadecimal(hour & 0x1F,2);
  if((hour & 0x20) == 0x00) TFT_string(8,37, Cyan,Black, "오전");
  else                      TFT_string(8,37, Cyan,Black, "오후");
  TFT_xy(16,37);				// display minute
  TFT_color(Green,Black);
  TFT_hexadecimal(minute,2);
  TFT_xy(19,37);				// display second
  TFT_color(Green,Black);
  TFT_hexadecimal(second,2);

  TFT_xy(28,37); TFT_color(Green,Black);	// display home
  TFT_English(' ');

  Draw_pin();					// draw hour, minute, second pin
  Circle(Xcenter,Ycenter, 5, Green);

  if((minute == 0x00) && (second == 0x00) && (tick_flag == 0))
    { tick_flag = 1;
      Beep();
    }
  else if((minute != 0x00) || (second != 0x00))
    { tick_flag = 0;
      GPIOC->BSRR = 0x00008000;   		// buzzer on
      Delay_us(100);				// delay 100 us
      GPIOC->BSRR = 0x80000000;			// buzzer off
    }
}

signed short Xstart1_hour_old, Ystart1_hour_old, Xstart2_hour_old, Ystart2_hour_old, Xend_hour_old, Yend_hour_old;
signed short Xstart1_minute_old, Ystart1_minute_old, Xstart2_minute_old, Ystart2_minute_old, Xend_minute_old, Yend_minute_old;

signed short Xstart_second_old, Ystart_second_old, Xend_second_old, Yend_second_old;
signed short Xcircle_second_old, Ycircle_second_old;

void Draw_pin(void)				/* draw hour, minute, second pin */
{
  unsigned short i;
  unsigned short pin_hour, length_hour = 70;
  signed short Xstart1_hour, Ystart1_hour, Xstart2_hour, Ystart2_hour, Xend_hour, Yend_hour;

  unsigned short pin_minute, length_minute = 90;
  signed short Xstart1_minute, Ystart1_minute, Xstart2_minute, Ystart2_minute, Xend_minute, Yend_minute;

  unsigned short pin_second, length_second = 100, center_second = 80;
  signed short Xstart_second, Ystart_second, Xend_second, Yend_second;
  signed short Xcircle_second, Ycircle_second;

  pin_hour = ((hour & 0x10) >> 4)*10 + (hour & 0x0F);	// convert hour to binary
  pin_minute = (minute >> 4)*10 + (minute & 0x0F);	// convert minute to binary
  pin_second = (second >> 4)*10 + (second & 0x0F);	// convert second to binary

						// calculate hour variables
  Xstart1_hour = Xcenter + (signed short)(6.*sin((pin_hour*30.+pin_minute/2.-30.)*3.141592/180.)+0.5);
  Ystart1_hour = Ycenter - (signed short)(6.*cos((pin_hour*30.+pin_minute/2.-30.)*3.141592/180.)+0.5);

  Xstart2_hour = Xcenter + (signed short)(6.*sin((pin_hour*30.+pin_minute/2.+30.)*3.141592/180.)+0.5);
  Ystart2_hour = Ycenter - (signed short)(6.*cos((pin_hour*30.+pin_minute/2.+30.)*3.141592/180.)+0.5);

  Xend_hour = Xcenter + (signed short)(length_hour*sin((pin_hour*30.+pin_minute/2.)*3.141592/180.)+0.5);
  Yend_hour = Ycenter - (signed short)(length_hour*cos((pin_hour*30.+pin_minute/2.)*3.141592/180.)+0.5);

						 // calculate minute variables
  Xstart1_minute = Xcenter + (signed short)(6.*sin((pin_minute*6.+pin_second/10.-30.)*3.141592/180.)+0.5);
  Ystart1_minute = Ycenter - (signed short)(6.*cos((pin_minute*6.+pin_second/10.-30.)*3.141592/180.)+0.5);

  Xstart2_minute = Xcenter + (signed short)(6.*sin((pin_minute*6.+pin_second/10.+30.)*3.141592/180.)+0.5);
  Ystart2_minute = Ycenter - (signed short)(6.*cos((pin_minute*6.+pin_second/10.+30.)*3.141592/180.)+0.5);

  Xend_minute = Xcenter + (signed short)(length_minute*sin((pin_minute*6.+pin_second/10.)*3.141592/180.)+0.5);
  Yend_minute = Ycenter - (signed short)(length_minute*cos((pin_minute*6.+pin_second/10.)*3.141592/180.)+0.5);

						// calculate second variables
  Xstart_second = Xcenter + (signed short)(6.*sin(3.141592/180.*(pin_second*6.))+0.5);
  Ystart_second = Ycenter - (signed short)(6.*cos(3.141592/180.*(pin_second*6.))+0.5);

  Xend_second = Xcenter + (signed short)(length_second*sin(3.141592/180.*(pin_second*6.))+0.5);
  Yend_second = Ycenter - (signed short)(length_second*cos(3.141592/180.*(pin_second*6.))+0.5);
	
  Xcircle_second = Xcenter + (signed short)(center_second*sin(3.141592/180.*(pin_second*6.))+0.5);
  Ycircle_second = Ycenter - (signed short)(center_second*cos(3.141592/180.*(pin_second*6.))+0.5);

  if(start_flag == 0)				// initialize old value
    { start_flag = 1;

      Xstart1_hour_old = Xstart1_hour;
      Ystart1_hour_old = Ystart1_hour;
      Xstart2_hour_old = Xstart2_hour;
      Ystart2_hour_old = Ystart2_hour;
      Xend_hour_old = Xend_hour;
      Yend_hour_old = Yend_hour;

      Xstart1_minute_old = Xstart1_minute;
      Ystart1_minute_old = Ystart1_minute;
      Xstart2_minute_old = Xstart2_minute;
      Ystart2_minute_old = Ystart2_minute;
      Xend_minute_old = Xend_minute;
      Yend_minute_old = Yend_minute;

      Xstart_second_old = Xstart_second;
      Ystart_second_old = Ystart_second;
      Xend_second_old = Xend_second;
      Yend_second_old = Yend_second;
      Xcircle_second_old = Xcircle_second;
      Ycircle_second_old = Ycircle_second;
    }

						// delete old hour and minute pin
  if((Xend_hour != Xend_hour_old) || (Yend_hour != Yend_hour_old) || (Xend_minute != Xend_minute_old) || (Yend_minute != Yend_minute_old))
    { Line(Xstart1_hour_old,Ystart1_hour_old, Xend_hour_old,Yend_hour_old, Black);
      Line(Xstart2_hour_old,Ystart2_hour_old, Xend_hour_old,Yend_hour_old, Black);

      Line(Xstart1_hour_old+1,Ystart1_hour_old, Xend_hour_old,Yend_hour_old, Black);
      Line(Xstart1_hour_old-1,Ystart1_hour_old, Xend_hour_old,Yend_hour_old, Black);
      Line(Xstart2_hour_old+1,Ystart2_hour_old, Xend_hour_old,Yend_hour_old, Black);
      Line(Xstart2_hour_old-1,Ystart2_hour_old, Xend_hour_old,Yend_hour_old, Black);

      Line(Xstart1_hour_old,Ystart1_hour_old+1, Xend_hour_old,Yend_hour_old, Black);
      Line(Xstart1_hour_old,Ystart1_hour_old-1, Xend_hour_old,Yend_hour_old, Black);
      Line(Xstart2_hour_old,Ystart2_hour_old+1, Xend_hour_old,Yend_hour_old, Black);
      Line(Xstart2_hour_old,Ystart2_hour_old-1, Xend_hour_old,Yend_hour_old, Black);

      Line(Xstart1_minute_old,Ystart1_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line(Xstart2_minute_old,Ystart2_minute_old, Xend_minute_old,Yend_minute_old, Black);

      Line(Xstart1_minute_old+1,Ystart1_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line(Xstart1_minute_old-1,Ystart1_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line(Xstart2_minute_old+1,Ystart2_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line(Xstart2_minute_old-1,Ystart2_minute_old, Xend_minute_old,Yend_minute_old, Black);

      Line(Xstart1_minute_old,Ystart1_minute_old+1, Xend_minute_old,Yend_minute_old, Black);
      Line(Xstart1_minute_old,Ystart1_minute_old-1, Xend_minute_old,Yend_minute_old, Black);
      Line(Xstart2_minute_old,Ystart2_minute_old+1, Xend_minute_old,Yend_minute_old, Black);
      Line(Xstart2_minute_old,Ystart2_minute_old-1, Xend_minute_old,Yend_minute_old, Black);
    }

  for(i = 3; i > 0; i--)			// delete old second pin
    Circle(Xcircle_second_old,Ycircle_second_old, i, Black);
  Line(Xstart_second_old,Ystart_second_old, Xend_second_old,Yend_second_old, Black);

  TFT_color(White,Black);			// display hour number
  TFT_English_pixel(112,54, '1'); TFT_English_pixel(120,54, '2');
  TFT_English_pixel(62,68, '1'); TFT_English_pixel(70,68, '1');
  TFT_English_pixel(164,68, '1');
  TFT_English_pixel(28,104, '1'); TFT_English_pixel(36,104, '0');
  TFT_English_pixel(202,104, '2');
  TFT_English_pixel(18,152, '9');
  TFT_English_pixel(216,152, '3');
  TFT_English_pixel(32,202, '8');
  TFT_English_pixel(202,202, '4');
  TFT_English_pixel(68,238, '7');
  TFT_English_pixel(164,238, '5');
  TFT_English_pixel(117,252, '6');

  TFT_color(Silver,Black);			// *** display calendar
  TFT_xy(12,24);				// display slash and ()
  TFT_English('/');
  TFT_xy(15,24);
  TFT_English('/');
  TFT_xy(18,24);
  TFT_English('(');
  TFT_xy(21,24);
  TFT_English(')');
  TFT_string(8,24, Green,Black, "20");		// display year
  TFT_hexadecimal(year,2);
  TFT_xy(13,24);				// display month
  TFT_hexadecimal(month,2);
  TFT_xy(16,24);				// display date
  TFT_hexadecimal(date,2);
  if(weekday == 0x01)      TFT_string(19,24, Red,Black, "일");	// display weekday
  else if(weekday == 0x02) TFT_string(19,24, Red,Black, "월");
  else if(weekday == 0x03) TFT_string(19,24, Red,Black, "화");
  else if(weekday == 0x04) TFT_string(19,24, Red,Black, "수");
  else if(weekday == 0x05) TFT_string(19,24, Red,Black, "목");
  else if(weekday == 0x06) TFT_string(19,24, Red,Black, "금");
  else if(weekday == 0x07) TFT_string(19,24, Red,Black, "토");

  Rectangle(59,189, 179,209, DarkGrey);
  Rectangle(60,190, 178,208, Red);

  Line(Xstart1_minute,Ystart1_minute, Xend_minute,Yend_minute, Cyan); // draw new minute pin
  Line(Xstart2_minute,Ystart2_minute, Xend_minute,Yend_minute, Cyan);

  Line(Xstart1_hour,Ystart1_hour, Xend_hour, Yend_hour, Magenta); // draw new hour pin
  Line(Xstart2_hour,Ystart2_hour, Xend_hour, Yend_hour, Magenta);

  for(i = 3; i > 0; i--)			// draw new second pin
    Circle(Xcircle_second,Ycircle_second, i, Yellow);
  Line(Xstart_second,Ystart_second, Xend_second,Yend_second, Yellow);

  Xstart1_hour_old = Xstart1_hour;		// store old value
  Ystart1_hour_old = Ystart1_hour;
  Xstart2_hour_old = Xstart2_hour;
  Ystart2_hour_old = Ystart2_hour;
  Xend_hour_old = Xend_hour;
  Yend_hour_old = Yend_hour;

  Xstart1_minute_old = Xstart1_minute;
  Ystart1_minute_old = Ystart1_minute;
  Xstart2_minute_old = Xstart2_minute;
  Ystart2_minute_old = Ystart2_minute;
  Xend_minute_old = Xend_minute;
  Yend_minute_old = Yend_minute;

  Xstart_second_old = Xstart_second;
  Ystart_second_old = Ystart_second;
  Xend_second_old = Xend_second;
  Yend_second_old = Yend_second;
  Xcircle_second_old = Xcircle_second;
  Ycircle_second_old = Ycircle_second;
}
