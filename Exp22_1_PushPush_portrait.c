/* ============================================================================ */
/*	Exp21_1_PushPush_portrait.c : 푸시푸시(PushPush) 게임			*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void SysTick_Handler(void);			// SysTick interrupt function by 2Hz
void Draw_symbol(U16 xPos,U16 yPos,U16 color,U08 code);	// draw 16x16 symbol
void Draw_arrow(U16 xPos,U16 yPos,U16 color,U08 code);	// draw 32x32 arrow
void Draw_board(void);				// draw game board
void Draw_control(void);			// draw game control
void Draw_previous(U16 color1, U16 color2);	// display previous stage for touch screen
void Draw_next(U16 color1, U16 color2);		// display previous stage for touch screen
void Draw_clear(U16 color1, U16 color2);	// display clear for touch screen
void Draw_left(U16 color1, U16 color2, U16 color3); // draw left arrow for touch screen
void Draw_right(U16 color1, U16 color2, U16 color3);// draw right arrow for touch screen
void Draw_up(U16 color1, U16 color2, U16 color3);   // draw up arrow for touch screen
void Draw_down(U16 color1, U16 color2, U16 color3); // draw down arrow for touch screen
unsigned char Icon_input(void);			// input touch screen icon
void PushPush(U16 direction);			// push box by direction arrow
unsigned char Check_OK(void);			// check game stage OK

/* ---------------------------------------------------------------------------- */

#define Left		KEY1			// direction by KEY1
#define Right		KEY2			// direction by KEY2
#define Up		KEY3			// direction by KEY3
#define Down		KEY4			// direction by KEY4

#define KEY5		0x11			// previous stage icon of touch screen
#define	KEY6		0x12			// next stage icon of touch screen
#define	KEY7		0x13			// clear icon of touch screen

#define	Xbase		 8			// x base position of board
#define	Ybase		23			// y base position of board

#define stage_max	35			// maximum stage number

unsigned char  beep_flag;			// beep on/off flag
unsigned char  stage;				// game stage
unsigned char  buffer[12][14];			// board buffer of a stage(x=12, y=12)
unsigned short xo, yo;				// current x, y position

/* ----- 게임 스테이지 정의 --------------------------------------------------- */

const unsigned char board[stage_max][12][14]={	/* define board of each stage */
	{ "              ",			// stage 1
	  "              ",
	  "   XXXXXXXX   ",
	  "   XX@ XXXX   ",
	  "   XX *  XX   ",
	  "   XXX X XX   ",
	  "   X0X X  X   ",
	  "   X0*  X X   ",
	  "   X0   * X   ",
	  "   XXXXXXXX   ",
	  "              ",
	  "              " },
	{ "              ",			// stage 2
	  "  XXXXXXXXXX  ",
	  "  XX@  XXXXX  ",
	  "  XX **XX  X  ",
	  "  XX *     X  ",
	  "  XXXXXX   X  ",
	  "  XX   X XXX  ",
	  "  XX XXX  XX  ",
	  "  XX X000 XX  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 3
	  "              ",
	  "   XXXXXXXX   ",
	  "   XX00 0XX   ",
	  "   XX0* 0XX   ",
	  "   XX*   XX   ",
	  "   X *  *@X   ",
	  "   X XX*X X   ",
	  "   X      X   ",
	  "   XXXXXXXX   ",
	  "              ",
	  "              " },
	{ "              ",			// stage 4
	  "              ",
	  "   XXXXXXX    ",
	  "   XXX  XX    ",
	  "   X *   X    ",
	  "   X X X X    ",
	  "   X * X X    ",
	  "   X *@**X    ",
	  "   X00000X    ",
	  "   XXXXXXX    ",
	  "              ",
	  "              " },
	{ "              ",			// stage 5
	  "              ",
	  "              ",
	  "  XXXXXXXXXX  ",
	  "  X   XXXXXX  ",
	  "  X * 00*  X  ",
	  "  XX* 00**@X  ",
	  "  XX  00*  X  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              ",
	  "              " },
	{ "              ",			// stage 6
	  "              ",
	  "  XXXXXXXXXX  ",
	  "  XX     XXX  ",
	  "  XX*XXX   X  ",
	  "  X @ *  * X  ",
	  "  X 00X * XX  ",
	  "  XX00X   XX  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              ",
	  "              " },
	{ "              ",			// stage 7
	  "              ",
	  "  XXXXXXXXXX  ",
	  "  XXXX  X @X  ",
	  "  XXX   X  X  ",
	  "  XXX* * * X  ",
	  "  XXX *XX  X  ",
	  "  XXX * X XX  ",
	  "  X00000  XX  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 8
	  "  XXXXXXXXXX  ",
	  "  XXX    XXX  ",
	  "  XXX *    X  ",
	  "  XXX * XX X  ",
	  "  X000 *   X  ",
	  "  X000*X* XX  ",
	  "  XXXX X * X  ",
	  "  XXXX  @  X  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 9
	  "  XXXXXXXXX   ",
	  "  X       X  ",
	  "  X X**  XX  ",
	  "  X 000X XX  ",
	  "  XX000* XX  ",
	  "  XX XX * X  ",
	  "  XX*  *  X  ",
	  "  XX  X  @X  ",
	  "  XXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 10
	  "             ",
	  "  XXXXXXXXX  ",
	  "  XXXX0  @X  ",
	  "  X  ***  X  ",
	  "  X0XX0XX0X  ",
	  "  X   *   X  ",
	  "  X  *0X XX  ",
	  "  XXXX   XX  ",
	  "  XXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 11
	  " XXXXXXXXXXXX ",
	  " XXXX   XX  X ",
	  " X@*  00 X  X ",
	  " X   X0X XXXX ",
	  " XX X0 0    X ",
	  " XX XX*XX*X X ",
	  " XX * *     X ",
	  " XXXX   X  XX ",
	  " XXXXXXXXXXXX ",
	  "              ",
	  "              " },
	{ "              ",			// stage 12
	  " XXXXXXXXXXXX ",
	  " X   X   X  X ",
	  " X       X  X ",
	  " XX XXX XXXXX ",
	  " XX   000   X ",
	  " XX*XXX X X X ",
	  " X   *@   * X ",
	  " X   X  XXXXX ",
	  " XXXXXXXXXXXX ",
	  "              ",
	  "              " },
	{ "              ",			// stage 13
	  "              ",
	  "   XXXXXXXX   ",
	  "   X X00X X   ",
	  "   XXX 0XXX   ",
	  "   X   *0XX   ",
	  "   X  *  XX   ",
	  "   X  X** X   ",
	  "   X  @   X   ",
	  "   XXXXXXXX   ",
	  "              ",
	  "              " },
	{ "              ",			// stage 14
	  "              ",
	  "   XXXXXXXX   ",
	  "   XX0 00XX   ",
	  "   XX0 *0XX   ",
	  "   XXX  *XX   ",
	  "   X *  * X   ",
	  "   X X*XX X   ",
	  "   X   @  X   ",
	  "   XXXXXXXX   ",
	  "              ",
	  "              " },
	{ "              ",			// stage 15
	  "              ",
	  "  XXXXXXXXXX  ",
	  "  X  XXXXXXX  ",
	  "  XXXX    XX  ",
	  "  XX0 *XX XX  ",
	  "  X00* * @ X  ",
	  "  X00 * * XX  ",
	  "  XXXXXX  XX  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 16
	  "              ",
	  "   XXXXXXXX   ",
	  "   X0   0 X   ",
	  "   X X X  X   ",
	  "   X@*  *0X   ",
	  "   XXXXX *X   ",
	  "   X   X  X   ",
	  "   XXXXXXXX   ",
	  "              ",
	  "              ",
  	  "              " },
	{ "              ",			// stage 17
	  "              ",
	  "  XXXXXXXXX   ",
	  "  XX00*00XX   ",
	  "  XX00X00XX   ",
	  "  XX *** XX   ",
	  "  XX  *  XX   ",
	  "  XX *** XX   ",
	  "  XX  X@ XX   ",
	  "  XXXXXXXXX   ",
	  "              ",
  	  "              " },
	{ "              ",			// stage 18
	  "              ",
	  "  XXXXXXXXX   ",
	  "  X000000XX   ",
	  "  X  * X XX   ",
	  "  X * X * X   ",
	  "  XX* * * X   ",
	  "  XX  @   X   ",
	  "  XXXXXXXXX   ",
	  "              ",
	  "              ",
	  "              " },
	{ "              ",			// stage 19
	  "              ",
	  "   XXXXXXXX   ",
	  "   XXX    X   ",
	  "   XXX*** X   ",
	  "   X@ *00 X   ",
	  "   X *000XX   ",
	  "   XXXX  XX   ",
	  "   XXXXXXXX   ",
	  "              ",
	  "              ",
	  "              " },
	{ "              ",			// stage 20
	  "              ",
	  "  XXXXXXXXXX  ",
	  "  XXXXXXX  X  ",
	  "  XX    X  X  ",
	  "  XX ***XX X  ",
	  "  XX  X00XXX  ",
	  "  XXX  00* X  ",
	  "  XXX @    X  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 21
	  "              ",
	  "  XXXXXXXXXX  ",
	  "  XX0000 XXX  ",
	  "  XXX000*XXX  ",
	  "  X  *X* * X  ",
	  "  X **  X* X  ",
	  "  X    X   X  ",
	  "  XXXX @ XXX  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 22
	  "              ",
	  " XXXXXXXXXXX  ",
	  " X  X 000X X  ",
	  " XXXX0000X X  ",
	  " X  XXX* XXX  ",
	  " X * *  ** X  ",
	  " X@ * *    X  ",
	  " X   XXX   X  ",
	  " XXXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 23
	  "              ",
	  "  XXXXXXXXXX  ",
	  "  XXX   XXXX  ",
	  "  X   * *  X  ",
	  "  X *   * @X  ",
	  "  XXX**XXXXX  ",
	  "  X X  00X X  ",
	  "  X X0000X X  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 24
	  " XXXXXXXXXXX  ",
	  " X       X X  ",
	  " X  * * *X X  ",
	  " XX X*XX X X  ",
	  " XX 00 00XXX  ",
	  " XXX00 00 XX  ",
	  " X X XX*X XX  ",
	  " X X* * *  X  ",
	  " X X      @X  ",
	  " XXXXXXXXXXX  ",
	  "              " },
	{ "              ",			// stage 25
	  "  XXXXXXXXXX  ",
	  "  XX  XXXXXX  ",
	  "  XX* XX  XX  ",
	  "  X  *@*  XX  ",
	  "  X   XX* XX  ",
	  "  XXX0XX XXX  ",
	  "  XX000* * X  ",
	  "  XXX00    X  ",
	  "  XXXXXXXXXX  ",
	  "              ",
	  "              " },
	{ "              ",			// stage 26
	  " XXXXXXXXXXX  ",
	  " X    X    X  ",
	  " X *@***** X  ",
	  " X         X  ",
	  " XXXXX XXXXX  ",
	  " X  X0  X  X  ",
	  " X  X0  X  X  ",
	  " X  X000X  X  ",
	  " X  X0  X  X  ",
	  " XXXXXXXXXXX  ",
	  "              " },
	{ "              ",			// stage 27
	  "              ",
	  " XXXXXXXXXXX  ",
	  " XXXX    0 X  ",
	  " X  * * *0 X  ",
	  " X  0XXXX0XX  ",
	  " X *0* * @XX  ",
	  " X  0  XXXXX  ",
	  " XXXXXXXXXXX  ",
	  "              ",
	  "              ",
	  "              " },
	{ "              ",			// stage 28
	  " XXXXXXXXXXX  ",
	  " XXX  @   XX  ",
	  " X   * *  XX  ",
	  " X  * * * XX  ",
	  " XXXXX*XXXXX  ",
	  " X  XX 0 00X  ",
	  " X  X  0X  X  ",
	  " X  X 0   0X  ",
	  " X  XXXX  XX  ",
	  " XXXXXXXXXXX  ",
	  "              " },
	{ "              ",			// stage 29
	  " XXXXXXXXXXX  ",
	  " X @X  XXXXX  ",
	  " X **  *   X  ",
	  " X  X0XX*X X  ",
	  " XX*X000   X  ",
	  " XX 000XX*XX  ",
	  " X  XX0XX  X  ",
	  " X  *  *   X  ",
	  " X  X   X  X  ",
	  " XXXXXXXXXXX  ",
	  "              " },
	{ "              ",			// stage 30
	  " XXXXXXXXXXXX ",
	  " XXXXXX0   XX ",
	  " XX  X00XX XX ",
	  " XX  *00   XX ",
	  " XX  X 0X XXX ",
	  " XXX XX*X  XX ",
	  " X *    ** XX ",
	  " X X*X  X  XX ",
	  " X@  XXXXXXXX ",
	  " XXXXXXXXXXXX ",
	  "              " },
	{ "XXXXXXXXXXXXXX",			// stage 31
	  "X00000XXXX   X",
	  "X00000   XXXXX",
	  "X000 0X  X   X",
	  "XXX XXX XX * X",
	  "X **  *   *  X",
	  "X   *   *X   X",
	  "X *  *XX X * X",
	  "X  XX *    XXX",
	  "XX   *@* * X X",
	  "XXXXX    XXX X",
	  "XXXXXXXXXXXXXX" },
	{ "XXXXXXXXXXXXXX",			// stage 32
	  "XX   XXX  X XX",
	  "X  * X  *    X",
	  "X *    X@ *  X",
	  "X   XXXXXX XXX",
	  "XXX*XX  XX XXX",
	  "XX00 XXXX  * X",
	  "XX000  * * * X",
	  "XX000XXXX    X",
	  "XX000X  X ** X",
	  "XXXXXXXXXXX  X",
	  "XXXXXXXXXXXXXX" },
	{ "XXXXXXXXXXXXXX",			// stage 33
	  "X XXX    XXX X",
	  "XXX   *X   X X",
	  "X **   *   X X",
	  "X    *X ** X X",
	  "X *   *   XX X",
	  "XXXXX*X XXXXXX",
	  "X   X  *X @00X",
	  "X   XX*  00X0X",
	  "X    X X*0000X",
	  "X    X   0000X",
	  "XXXXXXXXXXXXXX" },
	{ "XXXXXXXXXXXXXX",			// stage 34
	  "XX    XXX   XX",
	  "XX*X *X * * XX",
	  "XX *  *   *  X",
	  "X0000  XX  X X",
	  "X0XX0  * XX  X",
	  "X0000    *   X",
	  "X0000X*X **  X",
	  "XX *X   X@   X",
	  "XX    * XXXXXX",
	  "XXX  XXXX    X",
	  "XXXXXXXXXXXXXX" },
	{ "XXXXXXXXXXXXXX",			// stage 35
	  "X XXX    XXX X",
	  "X X   X*   XXX",
	  "X X   *   ** X",
	  "X X ** X*    X",
	  "X XX   *   * X",
	  "XXXXXX X*XXXXX",
	  "X00@ X*  X   X",
	  "X0X00  *XX   X",
	  "X0000*X X    X",
	  "X0000   X    X",
	  "XXXXXXXXXXXXXX" }};

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

volatile unsigned char count2Hz, second, minute;

void SysTick_Handler(void)			/* SysTick interrupt function by 2Hz */
{
  count2Hz++;
  if(count2Hz == 2)
    { count2Hz = 0;
      second++;					// increment second
      if(second == 60)				// if second = 60, second = 0
        { second = 0;
          minute++;				// increment minute
          if(minute == 60)			// if minute = 60, minute = 0
            minute = 0;
        }
    }
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)					/* main routine of PushPush game */
{
  unsigned char x, y, key, direction = 0;

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  TFT_portrait_mode();				// portrait mode
  Initialize_touch_screen();			// initialize touch screen

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp21_1.c    ");
  Beep();

  stage = 0;					// initial stage
  beep_flag = 1;				// beep on
  count2Hz = 0;					// initialize time
  second = 0;
  minute = 0;

  SysTick->LOAD  = 13499999;			// 27MHz/(13499999+1) = 2Hz
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000003;			// 216MHz/8 = 27MHz, enable SysTick and interrupt

  while(1)
    { TFT_clear_screen();			// clear screen for new stage

      for(y = 0; y < 12; y++)			// get new board and initial @ position
	for(x = 0; x < 14; x++) 
	  { buffer[y][x] = board[stage][y][x];
	    if(buffer[y][x] == '@') 
	      { xo = x;
	        yo = y;
	        buffer[y][x] = ' ';
	      }
          }

      second = 0;				// initialize time
      minute = 0;
      Draw_board();				// draw new board
      Draw_control();				// draw game control

      while(1)					// game of current stage
        { direction = 0;

          key = Key_input();			// key input
          if(key == no_key)			// if no key input, read touch screen icon
            key = Icon_input();

          if(key == KEY1)      direction = Left;
          else if(key == KEY2) direction = Right;
          else if(key == KEY3) direction = Up;
          else if(key == KEY4) direction = Down;
          else if(key == KEY5)			// go previous stage
            { if(stage != 0) stage--;
              else           stage = stage_max - 1; 
	      break;
	    }
          else if(key == KEY6)			// go next stage
            { if(stage != (stage_max-1)) stage++;
              else                       stage = 0;
	      break;
	    }
          else if(key == KEY7)			// clear current stage
	    break;

          PushPush(direction);			// go to push

          TFT_color(White,Black);		// display time
          TFT_xy(6,30); TFT_unsigned_decimal(minute,1,2);
          TFT_xy(9,30); TFT_unsigned_decimal(second,1,2);

          if(Check_OK()) 			// check if stage is OK
            { TFT_string(1,28, Green,Black, "  축하합니다!  ");
              Delay_ms(300);
	      if(beep_flag == 1) Beep_3times();
              Delay_ms(2000);
              if(stage != (stage_max-1)) stage++;
              else                       stage = 0;
	      break;
	    }
	}
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

const unsigned short Symbol[6][16] = {				/* define board symbol */
 { 0x07E0,0x0FF0,0x3FFC,0x3FFC,0x7FFE,0xF3CF,0xF3CF,0xFFFF,	// (0) 사람
   0xFFFF,0xEFF7,0xE7E7,0x700E,0x3C3C,0x3FFC,0x0FF0,0x07E0 },
 { 0xFC7F,0xFEFF,0xFEFF,0xFEFF,0xFEFF,0xFEFF,0xFC7F,0x0000,	// (1) 벽
   0x7FFC,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0x7FFC,0x0000 },
 { 0x0000,0x7FFE,0x7FFE,0x781E,0x7C3E,0x6E76,0x67E6,0x63C6,	// (2) 박스
   0x63C6,0x67E6,0x6E76,0x7C3E,0x781E,0x7FFE,0x7FFE,0x0000 },
 { 0x0000,0x0000,0x0000,0x0000,0x0000,0x07E0,0x07E0,0x07E0,	// (3) 목표점
   0x07E0,0x07E0,0x07E0,0x0000,0x0000,0x0000,0x0000,0x0000 },
 { 0x0000,0x0100,0x0300,0x0700,0x0F00,0x1F00,0x3FFF,0x7FFF,	// (4) left arrow
   0xFFFF,0x7FFF,0x3FFF,0x1F00,0x0F00,0x0700,0x0300,0x0100 },
 { 0x0000,0x0080,0x00C0,0x00E0,0x00F0,0x00F8,0xFFFC,0xFFFE,	// (5) right arrow
   0xFFFF,0xFFFE,0xFFFC,0x00F8,0x00F0,0x00E0,0x00C0,0x0080 }};

void Draw_symbol(U16 xPos,U16 yPos, U16 color, U08 code)	/* draw 16x16 symbol */
{
  unsigned char  x, y;
  unsigned short font;

  for(y = 0; y < 16; y++)
    { TFT_GRAM_address(xPos,yPos+y);
      font = Symbol[code][y];
      for(x = 0; x < 16; x++)
        { if((font << x) & 0x8000) TFT_data(color);	
	  else                     TFT_data(Black);
	}
    }
}

const unsigned short Arrow[4][64] = {				/* define arrow of touch screen */
 { 0x0000,0x0000,0x0000,0x0100,0x0000,0x0300,0x0000,0x0700,	// (0) left arrow
   0x0000,0x0F00,0x0000,0x1F00,0x0000,0x3F00,0x0000,0x7F00, 
   0x0000,0xFF00,0x0001,0xFF00,0x0003,0xFF00,0x0007,0xFF00, 
   0x000F,0xFF00,0x001F,0xFF00,0x003F,0xFF00,0x007F,0xFF00, 
   0x00FF,0xFF00,0x007F,0xFF00,0x003F,0xFF00,0x001F,0xFF00, 
   0x000F,0xFF00,0x0007,0xFF00,0x0003,0xFF00,0x0001,0xFF00, 
   0x0000,0xFF00,0x0000,0x7F00,0x0000,0x3F00,0x0000,0x1F00, 
   0x0000,0x0F00,0x0000,0x0700,0x0000,0x0300,0x0000,0x0100 },
 { 0x0000,0x0000,0x0080,0x0000,0x00C0,0x0000,0x00E0,0x0000,	// (1) right arrow
   0x00F0,0x0000,0x00F8,0x0000,0x00FC,0x0000,0x00FE,0x0000, 
   0x00FF,0x0000,0x00FF,0x8000,0x00FF,0xC000,0x00FF,0xE000, 
   0x00FF,0xF000,0x00FF,0xF800,0x00FF,0xFC00,0x00FF,0xFE00, 
   0x00FF,0xFF00,0x00FF,0xFE00,0x00FF,0xFC00,0x00FF,0xF800, 
   0x00FF,0xF000,0x00FF,0xE000,0x00FF,0xC000,0x00FF,0x8000, 
   0x00FF,0x0000,0x00FE,0x0000,0x00FC,0x0000,0x00F8,0x0000, 
   0x00F0,0x0000,0x00E0,0x0000,0x00C0,0x0000,0x0080,0x0000 },
 { 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	// (2) down arrow
   0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, 
   0x7FFF,0xFFFF,0x3FFF,0xFFFE,0x1FFF,0xFFFC,0x0FFF,0xFFF8, 
   0x07FF,0xFFF0,0x03FF,0xFFE0,0x01FF,0xFFC0,0x00FF,0xFF80, 
   0x007F,0xFF00,0x003F,0xFE00,0x001F,0xFC00,0x000F,0xF800, 
   0x0007,0xF000,0x0003,0xE000,0x0001,0xC000,0x0000,0x8000, 
   0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, 
   0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000 },
 { 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	// (3) up arrow
   0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, 
   0x0000,0x8000,0x0001,0xC000,0x0003,0xE000,0x0007,0xF000,
   0x000F,0xF800,0x001F,0xFC00,0x003F,0xFE00,0x007F,0xFF00,
   0x00FF,0xFF80,0x01FF,0xFFC0,0x03FF,0xFFE0,0x07FF,0xFFF0,
   0x0FFF,0xFFF8,0x1FFF,0xFFFC,0x3FFF,0xFFFE,0x7FFF,0xFFFF,
   0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, 
   0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000 }};

void Draw_arrow(U16 xPos,U16 yPos,U16 color,U08 code)	/* draw 32x32 arrow */
{
  unsigned char x, y;
  unsigned int  font;

  for(y = 0; y < 64; y += 2)
    { TFT_GRAM_address(xPos,yPos+(y/2));
      font = (Arrow[code][y] << 16) | Arrow[code][y+1]; 
      for(x = 0; x < 32; x++)
        { if((font << x) & 0x80000000) TFT_data(color);
	  else                         TFT_data(Black);
	}
    }
}

void Draw_board(void)				/* draw game board */
{
  unsigned short x, y;

  for(y = 0; y < 12; y++)
    { for(x = 0; x < 14; x++)
        { switch(buffer[y][x])
	    { case 'X': Draw_symbol(x*16 + Xbase, y*16 + Ybase, Magenta, 1); // 벽
			break;
	      case '*': Draw_symbol(x*16 + Xbase, y*16 + Ybase, Yellow,  2); // 박스
			break;
	      case '0': Draw_symbol(x*16 + Xbase, y*16 + Ybase, Green,   3); // 목표점
			break;
	      case '@': Draw_symbol(x*16 + Xbase, y*16 + Ybase, Cyan,    0); // 사람
			break;
	      default:  Draw_symbol(x*16 + Xbase, y*16 + Ybase, Black,   0); // 여백
			break;
	    }
	}
      Draw_symbol(xo*16 + Xbase, yo*16 + Ybase, Cyan, 0);
    }

  Draw_symbol(xo*16 + Xbase, yo*16 + Ybase, Cyan, 0);

  Rectangle(4,19, 235,216, White);		// draw outline
}

void Draw_control(void)				/* draw game control */
{
  TFT_string(1,0,White,Blue, "       PushPush 게임        "); // display title

  TFT_string(3,28,Blue,Cyan," 효과음");		// beep on/off
  if(beep_flag == 1)
    TFT_string(10,28,Red,Cyan," ON ");
  else
    TFT_string(10,28,Red,Cyan,"OFF ");

  TFT_string(5,30,White,  Black, "(00:00)");	// display time

  TFT_string(6,32,Magenta,Black, "STAGE");	// display stage
  TFT_string(6,34,Green,  Black, "00/00");
  TFT_color(Green,Black);
  TFT_xy( 6,34); TFT_unsigned_decimal(stage + 1, 1, 2);
  TFT_xy( 9,34); TFT_unsigned_decimal(stage_max, 1, 2);

  Draw_previous(Magenta, Green);		// display stage change
  Draw_next(Magenta, Green);

  Draw_clear(White, Yellow);			// display clear of current stage

  Draw_left(White, Cyan, Yellow);		// draw left (KEY1)
  Draw_right(White, Cyan, Yellow);		// draw right(KEY2)
  Draw_up(White, Cyan, Yellow);			// draw up   (KEY3)
  Draw_down(White, Cyan, Yellow);		// draw down (KEY4)
}

void Draw_previous(U16 color1, U16 color2)	/* display previous stage for touch screen */
{
  Rectangle(14,261, 35,281, color1);
  Draw_symbol(17, 263, color2, 4);
}

void Draw_next(U16 color1, U16 color2)		/* display next stage for touch screen */
{
  Rectangle(101,261, 122,281, color1);
  Draw_symbol(104, 263, color2, 5);
}

void Draw_clear(U16 color1, U16 color2)		/* display clear for touch screen */
{
  Rectangle(45,292, 91,314, color1);
  TFT_string(6,37,color2,Black, "CLEAR");
}

void Draw_left(U16 color1, U16 color2, U16 color3)	/* display left arrow for touch screen */
{
  Draw_arrow(139,255, color2, 0);
  TFT_string(21,33, color3,Black,  "1");
  Line(144,254, 168,254, color1);
  Line(168,254, 185,271, color1);
  Line(185,271, 168,288, color1);
  Line(168,288, 144,288, color1);
  Line(144,288, 144,254, color1);
}

void Draw_right(U16 color1, U16 color2, U16 color3)	/* display right arrow for touch screen */
{
  Draw_arrow(205,255, color2, 1);
  TFT_string(25,33, color3,Black,  "2");
  Line(231,254, 207,254, color1);
  Line(207,254, 190,271, color1);
  Line(190,271, 207,288, color1);
  Line(207,288, 231,288, color1);
  Line(231,288, 231,255, color1);
}

void Draw_up(U16 color1, U16 color2, U16 color3)	/* display up arrow for touch screen */
{
  Draw_arrow(172,222, color2, 3);
  TFT_string(23,31, color3,Black,  "3");
  Line(171,227, 171,251, color1);
  Line(171,251, 188,268, color1);
  Line(188,268, 205,251, color1);
  Line(205,251, 205,227, color1);
  Line(205,227, 172,227, color1);
}

void Draw_down(U16 color1, U16 color2, U16 color3)	/* display down arrow for touch screen */
{
  Draw_arrow(172,288, color2, 2);
  TFT_string(23,35, color3,Black,  "4");
  Line(171,314, 171,290, color1);
  Line(171,290, 188,273, color1);
  Line(188,273, 205,290, color1);
  Line(205,290, 205,314, color1);
  Line(205,314, 171,314, color1);
}

unsigned char icon_flag = 0;

unsigned char Icon_input(void)			/* input touch screen icon */
{
  unsigned char icon;

  Touch_screen_input();				// input touch screen

  if((icon_flag == 0) && (x_touch >= 24) && (x_touch <= 111) && (y_touch >= 224) && (y_touch <= 240))
    { icon = no_key;				// beep on/off
      icon_flag = 1;
      Beep();
      if(beep_flag == 0)
        { beep_flag = 1;
	  TFT_string(10,28,Red, Cyan," ON ");
        }
      else
        { beep_flag = 0;
          TFT_string(10,28,Red, Cyan,"OFF ");
        }
    }
  else if((icon_flag == 0) && (x_touch >= 14) && (x_touch <= 35) && (y_touch >= 261) && (y_touch <= 281))
    { icon = KEY5;				// previous stage
      icon_flag = 1;
      Draw_previous(White, White);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 101) && (x_touch <= 122) && (y_touch >= 261) && (y_touch <= 281))
    { icon = KEY6;				// next stage
      icon_flag = 1;
      Draw_next(White, White);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 45) && (x_touch <= 91) && (y_touch >= 292) && (y_touch <= 314))
    { icon = KEY7;				// clear
      icon_flag = 1;
      Draw_clear(Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 144) && (x_touch <= 172) && (y_touch >= 254) && (y_touch <= 288))
    { icon = KEY1;				// left
      icon_flag = 1;
      Draw_left(Magenta, Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 203) && (x_touch <= 231) && (y_touch >= 254) && (y_touch <= 288))
    { icon = KEY2;				// right
      icon_flag = 1;
      Draw_right(Magenta, Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 171) && (x_touch <= 205) && (y_touch >= 227) && (y_touch <= 255))
    { icon = KEY3;				// up
      icon_flag = 1;
      Draw_up(Magenta, Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 171) && (x_touch <= 205) && (y_touch >= 286) && (y_touch <= 314))
    { icon = KEY4;				// down
      icon_flag = 1;
      Draw_down(Magenta, Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 1) && (x_touch == 0) && (y_touch == 0))
    { icon = no_key;
      icon_flag = 0;
      Draw_previous(Magenta, Green);
      Draw_next(Magenta, Green);
      Draw_clear(White, Yellow);
      Draw_left(White, Cyan, Yellow);
      Draw_right(White, Cyan, Yellow);
      Draw_up(White, Cyan, Yellow);
      Draw_down(White, Cyan, Yellow);
      Delay_ms(50);
    }
  else
    icon = no_key;

  return icon;
}

/* ----- 게임 알고리즘 함수 --------------------------------------------------- */

void PushPush(U16 direction)			/* push box by direction arrow */
{
  signed char mx = 0, my = 0;

  if(direction == Left)       mx = -1;		// check direction
  else if(direction == Right) mx = +1;
  else if(direction == Up)    my = -1;
  else if(direction == Down)  my = +1;
  else return;

  if(buffer[yo+my][xo+mx] != 'X')		// 이동할 곳이 벽이 아닌 경우에만
    { if(buffer[yo+my][xo+mx] == '*')		// 이동할 곳에 박스가 있는 경우에만
	{ if((buffer[yo+my*2][xo+mx*2] == ' ') || (buffer[yo+my*2][xo+mx*2] == '0')) // 박스가 갈 곳
	    { if(board[stage][yo+my][xo+mx] == '0')
		buffer[yo+my][xo+mx] = '0';
	      else
	        buffer[yo+my][xo+mx] = ' ';
	      buffer[yo+my*2][xo+mx*2] = '*';
	    }
	  else return;
	}

      if(buffer[yo][xo] == '0')			// 지나간 자리
        Draw_symbol(xo*16 + Xbase, yo*16 + Ybase, Green, 3);
      else if(buffer[yo][xo] == ' ')
        Draw_symbol(xo*16 + Xbase, yo*16 + Ybase, Black, 0);

      Draw_symbol((xo+mx)*16 + Xbase, (yo+my)*16 + Ybase, Cyan, 0); // 사람

      if(buffer[yo+my*2][xo+mx*2] == '*')	// 박스
        Draw_symbol((xo+mx*2)*16 + Xbase, (yo+my*2)*16 + Ybase, Yellow,  2);

      xo += mx;					// 사람이 이동한 새 위치
      yo += my;
    }
}

unsigned char Check_OK(void)			/* check game stage OK */
{
  unsigned char x, y;

  for(y = 0; y < 12; y++)
    for(x = 0; x < 14; x++)
      if((board[stage][y][x] == '0') && (buffer[y][x] != '*'))
	return 0;

  return 1;	
}
