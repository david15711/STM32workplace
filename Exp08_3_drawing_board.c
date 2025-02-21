/* ============================================================================ */
/*	Exp08_3_drawing_board.c : 터치스크린을 이용한 그림판 프로그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

#define icon_clear	0x01			// clear
#define icon_width1	0x02			// line width 1
#define icon_width2	0x03			// line width 2
#define icon_width3	0x04			// line width 3
#define icon_white	0x05			// white
#define icon_red	0x06			// red
#define icon_magenta	0x07			// magenta
#define icon_yellow	0x08			// yellow
#define icon_green	0x09			// green
#define icon_cyan	0x0A			// cyan
#define icon_blue	0x0B			// blue
#define no_icon		0x00			// no icon

void Draw_select(void);				/* draw selected line width and color */
void Title_screen(void);			/* draw title screen */
unsigned int Icon_input(void);			/* input touch screen icon */

unsigned char icon_flag = 0;
unsigned char line_width = 2;			// line width (default = middle)
unsigned int  user_color = White;		// user color (default = White)

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned int icon;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp08_3.c    ");
  Beep();

  Initialize_touch_screen();			// initialize touch screen
  Title_screen();				// draw title screen
  
  while(1)
    { icon = Icon_input();			// input touch screen icon
      if(icon == icon_width1)
        { line_width = 1;	Draw_select(); }
      else if(icon == icon_width2)
        { line_width = 2;	Draw_select(); }
      else if(icon == icon_width3)
        { line_width = 3;	Draw_select(); }
      else if(icon == icon_clear)
        { TFT_clear_screen();	Title_screen(); }
      else if(icon == icon_white)
        { user_color = White;	Draw_select(); }
      else if(icon == icon_red)
        { user_color = Red;	Draw_select(); }
      else if(icon == icon_yellow)
        { user_color = Yellow;	Draw_select(); }
      else if(icon == icon_green)
        { user_color = Green;	Draw_select(); }
      else if(icon == icon_cyan)
        { user_color = Cyan;	Draw_select(); }
      else if(icon == icon_blue)
        { user_color = Blue;	Draw_select(); }
      else if(icon == icon_magenta)
        { user_color = Magenta;	Draw_select(); }

      Touch_screen_input();			// input touch screen
      if((x_touch != 0) || (y_touch != 0))
	{ if((line_width == 1) && (x_touch >= 2) && (x_touch <= 317) && (y_touch >= 2) && (y_touch <= 210))
	    TFT_pixel(x_touch, y_touch, user_color);
          else if((line_width == 2) && (x_touch >= 3) && (x_touch <= 316) && (y_touch >= 3) && (y_touch <= 209))
            { TFT_pixel(x_touch, y_touch, user_color);
	      Rectangle(x_touch-1, y_touch-1, x_touch+1, y_touch+1, user_color);
            }
          else if((line_width == 3) && (x_touch >= 4) && (x_touch <= 315) && (y_touch >= 4) && (y_touch <= 208))
            { TFT_pixel(x_touch, y_touch, user_color);
	      Rectangle(x_touch-1, y_touch-1, x_touch+1, y_touch+1, user_color);
	      Rectangle(x_touch-2, y_touch-2, x_touch+2, y_touch+2, user_color);
            }
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Draw_select(void)				/* draw selected line width and color */
{
  if(line_width == 1)				// line_width = 1 ?
    { Rectangle( 8,212, 47,234, Black);
      Rectangle( 9,213, 46,233, user_color);
      Rectangle(10,214, 45,232, Black);
    }
  else if(line_width == 2)			// line_width = 2 ?
    { Rectangle( 8,212, 47,234, Black);
      Rectangle( 9,213, 46,233, user_color);
      Rectangle(10,214, 45,232, user_color);
    }
  else if(line_width == 3)			// line_width = 3 ?
    { Rectangle( 8,212, 47,234, user_color);
      Rectangle( 9,213, 46,233, user_color);
      Rectangle(10,214, 45,232, user_color);
    }
}

void Title_screen(void)				/* draw title screen */
{
  Rectangle(0,0, 319,239, Green);               // draw outline of sheet
  Rectangle(1,1, 318,238, Green);
   
  TFT_string(16,1, White,Magenta, " 그림판 ");	// display title

  TFT_string(3,27, Green,Black, "C");		// clear selection button
  Draw_select();				// select box

  Rectangle(58,213, 78,233, Green);		// line_width selection button
  Line(63,223, 73,223, White);

  Rectangle(83,213, 103,233, Green);
  Line(88,222, 98,222, White);
  Line(88,223, 98,223, White);
  Line(88,224, 98,224, White);
 
  Rectangle(108,213, 128,233, Green);
  Line(113,221, 123,221, White);
  Line(113,222, 123,222, White);
  Line(113,223, 123,223, White);
  Line(113,224, 123,224, White);
  Line(113,225, 123,225, White);

  Block(139,213, 159,233, White, White);	// color selection button
  Block(164,213, 184,233, White, Red);
  Block(189,213, 209,233, White, Magenta);
  Block(214,213, 234,233, White, Yellow);
  Block(239,213, 259,233, White, Green);
  Block(264,213, 284,233, White, Cyan);
  Block(289,213, 309,233, White, Blue);
}

unsigned int Icon_input(void)			/* input touch screen icon */
{
  unsigned int icon;

  Touch_screen_input();				// input touch screen

  if((icon_flag == 0) && (x_touch >= 10) && (x_touch <= 45) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_clear;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 58) && (x_touch <= 78) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_width1;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 83) && (x_touch <= 103) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_width2;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 108) && (x_touch <= 128) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_width3;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 139) && (x_touch <= 159) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_white;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 164) && (x_touch <= 184) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_red;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 189) && (x_touch <= 209) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_magenta;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 214) && (x_touch <= 234) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_yellow;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 239) && (x_touch <= 259) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_green;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 264) && (x_touch <= 284) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_cyan;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 289) && (x_touch <= 309) && (y_touch >= 213) && (y_touch <= 233))
    { icon = icon_blue;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 1) && (x_touch == 0) && (y_touch == 0))
    { icon = no_icon;
      icon_flag = 0;
      Delay_ms(50);
    }
  else
    icon = no_icon;

  return icon;
}
