/* ============================================================================ */
/*    Exp17_1_Text-LCD(1).c : 텍스트 LCD에 표시한 디지털 시계 프로그그램(1)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void Display_time(void);			/* display DS3234 time */

/* ----- 메인 프로그램 -------------------------------------------------------- */

unsigned char year, month, date, weekday, hour, minute, second;
unsigned char beep_flag = 0;

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  DS3234_initialize();                          // initialize DS3234
  Beep();

  LCD_string(0x80,"2017/01/01 (SUN)");		// display initial screen
  LCD_string(0xC0,"  12:00:00  AM  ");

  while(1)
    { Display_time();				// display time
      Delay_ms(50);
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Display_time(void)				/* display DS3234 time */
{
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

  if((minute == 0x00) && (second == 0x00) && (beep_flag == 0))	// beep
    { beep_flag = 1;
      Beep();
    }
  else if(second != 0x00)
    beep_flag = 0;
}
