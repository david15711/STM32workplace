/* ============================================================================ */
/*		Exp14_4_organ(1).c : D/A 컨버터를 이용한 전자 오르간		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

#define	F3		1			// note F of octave 3
#define	F3X		2			// note F# of octave 3
#define	G3		3			// note G of octave 3
#define	G3X		4			// note G# of octave 3
#define	A3		5			// note A of octave 3
#define	A3X		6			// note A# of octave 3
#define	B3		7			// note B of octave 3
#define	C4		8			// note C of octave 4
#define	C4X		9			// note C# of octave 4
#define	D4		10			// note D of octave 4
#define	D4X		11			// note D# of octave 4
#define	E4		12			// note E of octave 4
#define	F4		13			// note F of octave 4
#define	F4X		14			// note F# of octave 4
#define	G4		15			// note G of octave 4
#define	G4X		16			// note G# of octave 4
#define	A4		17			// note A of octave 4
#define	A4X		18			// note A# of octave 4
#define	B4		19			// note B of octave 4
#define	C5		20			// note C of octave 5
#define	C5X		21			// note C# of octave 5
#define	D5		22			// note D of octave 5
#define	D5X		23			// note D# of octave 5
#define	E5		24			// note E of octave 5
#define	F5		25			// note F of octave 5
#define	F5X		26			// note F# of octave 5
#define	G5		27			// note G of octave 5
#define	G5X		28			// note G# of octave 5
#define	A5		29			// note A of octave 5
#define	A5X		30			// note A# of octave 5

#define	NOTE32		1*3			// define note length
#define	NOTE16		2*3
#define	NOTE16D		3*3
#define	NOTE16T		2*2
#define	NOTE8		4*3
#define	NOTE8D		6*3
#define	NOTE8T		4*2
#define	NOTE4		8*3
#define	NOTE4D		12*3
#define	NOTE4T		8*2
#define	NOTE2		16*3
#define	NOTE2D		24*3
#define	NOTE1		32*3

#define	REST32		1*3			// define rest length
#define	REST16		2*3
#define	REST16D		3*3
#define	REST8		4*3
#define	REST8D		6*3
#define	REST4		8*3
#define	REST4D		12*3
#define	REST2		16*3
#define	REST2D		24*3
#define	REST1		32*3

void SysTick_Handler(void);			/* SysTick interrupt function */
void Draw_organ(void);				/* draw organ screen */
void Draw_left(U16 x, U16 y, U16 color1, U16 color2);	/* display left arrow for touch screen */
void Draw_right(U16 x, U16 y, U16 color1, U16 color2);	/* display right arrow for touch screen */
void Draw_up(U16 x, U16 y, U16 color1, U16 color2);	/* display up arrow for touch screen */
void Draw_down(U16 x, U16 y, U16 color1, U16 color2);	/* display down arrow for touch screen */
void Draw_symbol(U16 xPos,U16 yPos, U16 colorfore,U16 colorback, U08 code); /* draw arrow for touch screen */
void Check_organ(void);				/* check organ key or icon */
void Play_note(U08 sound, U32 note);		/* play note */
void Play_rest(U32 rest);			/* play rest */
void Set_tempo(U08 tempo_value);		/* set tempo */
void Play_song1(void);				/* play song 1 - 고향의 봄*/
void Play_song2(void);				/* play song 2 - 산바람 강바람*/
void Play_song3(void);				/* play song 3 - 퐁당퐁당*/
void Play_song4(void);				/* play song 4 - 낭랑 18세*/
void Play_song5(void);				/* play song 5 - 돌아가는 삼각지*/
void Play_song6(void);				/* play song 6 - 칠갑산*/
void Play_song7(void);				/* play song 7 - 허공*/

unsigned char song;				// song for auto play (1~6)
unsigned char tempo;				// music tempo (1~5)
unsigned char organ_key;			// organ key

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

volatile unsigned char volume;			// speaker volume (1~5)
volatile unsigned char toggle_flag;

void SysTick_Handler(void)			/* SysTick interrupt function */
{
  toggle_flag++;
  toggle_flag %= 2;
  if(toggle_flag == 1)
    { DAC->DHR12R1 = 0;				// clear output data
      DAC->DHR12R2 = 0;
    }
  else
    { DAC->DHR12R1 = 150 * volume^2;		// output data
      DAC->DHR12R2 = 150 * volume^2;
    }
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  Initialize_touch_screen();			// initialize touch screen

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp14_4.c    ");
  Beep();

  GPIOA->MODER |= 0x00000F00;			// PA5, PA4 = analog mode
  RCC->APB1ENR |= 0x20000000;			// enable DAC clock

  DAC->CR = 0x00030003;				// DAC channel 2/1 enable, output buffer disable

  DAC->DHR12R1 = 0;				// clear output data
  DAC->DHR12R2 = 0;

  song = 0;					// initial value
  volume = 1;
  tempo = 3;

  Draw_organ();					// draw organ screen

  while(1)
    Check_organ();				// draw organ screen
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Draw_organ(void)				/* draw organ screen */
{
  unsigned int i, x;

  Rectangle(0,0, 319,145, Green);		// display outline
  Rectangle(1,1, 318,144, Green);

  TFT_string(1, 1,White,Magenta," OK-STM767 키트를 이용한  전자 오르간 "); // display title

  TFT_string(8,5,White,Black,"음 량");		// volume
  TFT_string(8,8,Silver,Black,"12345");
  TFT_string(8,8,Red,Black,"1");
  Rectangle(59,61, 107,81, Magenta);
  Rectangle(60,62, 106,80, Magenta);
  Draw_left(32,61, White,Cyan);
  Draw_right(111,61, White,Cyan);

  TFT_string(27,5,White,Black,"속 도");		// tempo
  TFT_string(27,8,Silver,Black,"12345");
  TFT_string(29,8, Red,Black,"3");
  Rectangle(211,61, 259,81, Magenta);
  Rectangle(212,62, 258,80, Magenta);
  Draw_left(184,61, White,Cyan);
  Draw_right(263,61, White,Cyan);

  TFT_string(12,12,White,Black," 자동 연주 기능 ");	// auto play
  TFT_string( 5,15,Cyan,Black," ---------------------------- ");
  Rectangle(38,118, 281,138, Cyan);
  Draw_up(11,118, White,Cyan);
  Draw_down(285,118, White,Cyan);

  for(i = 0; i < 16; i++)			// draw white key
    { x = 20*i + 1;
      Block(x,148, x+17,238, White,White);
    }

  for(i = 0; i < 16; i++)			// draw black key
    { x = 20*i + 11;
      if((i != 3) && (i != 6) && (i != 10) && (i != 13) && (i != 15))
        Block(x,148, x+17,205, Black,Black);
      else if(i == 15)
        Block(x,148, x+8,205, Black,Black);
    }
}

void Draw_left(U16 x, U16 y, U16 color1, U16 color2)	/* display left arrow for touch screen */
{
  Rectangle(x,y, x+23,y+20, color1);
  Draw_symbol(x+4,y+2, color2,Black, 0);
}

void Draw_right(U16 x, U16 y, U16 color1, U16 color2)	/* display right arrow for touch screen */
{
  Rectangle(x,y, x+23,y+20, color1);
  Draw_symbol(x+4,y+2, color2,Black, 1);
}

void Draw_up(U16 x, U16 y, U16 color1, U16 color2)	/* display up arrow for touch screen */
{
  Rectangle(x,y, x+23,y+20, color1);
  Draw_symbol(x+4,y+2, color2,Black, 2);
}

void Draw_down(U16 x, U16 y, U16 color1, U16 color2)	/* display down arrow for touch screen */
{
  Rectangle(x,y, x+23,y+20, color1);
  Draw_symbol(x+4,y+2, color2,Black, 3);
}

const unsigned short symbol[5][16] = {				/* 16x16 arrow */
 { 0x0000,0x0100,0x0300,0x0700,0x0F00,0x1F00,0x3FFF,0x7FFF,	// (0) left arrow
   0xFFFF,0x7FFF,0x3FFF,0x1F00,0x0F00,0x0700,0x0300,0x0100 },
 { 0x0000,0x0080,0x00C0,0x00E0,0x00F0,0x00F8,0xFFFC,0xFFFE,	// (1) right arrow
   0xFFFF,0xFFFE,0xFFFC,0x00F8,0x00F0,0x00E0,0x00C0,0x0080 },
 { 0x0100,0x0380,0x07C0,0x0FE0,0x1FF0,0x3FF8,0x7FFC,0xFFFE,	// (2) up arrow
   0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0 },
 { 0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,	// (3) down arrow
   0xFFFE,0x7FFC,0x3FF8,0x1FF0,0x0FE0,0x07C0,0x0380,0x0100 },
 { 0x00C0,0x00E0,0x00F0,0x00D8,0x00CC,0x00CC,0x00CC,0x00CC,	// (4) note4 symbol
   0x00C8,0x00D0,0x03C0,0x0FC0,0x0F80,0x1F80,0x1F00,0x0C00 }};

void Draw_symbol(U16 xPos,U16 yPos, U16 colorfore,U16 colorback, U08 code)	/* draw arrow for touch screen */
{
  unsigned char  x, y;
  unsigned short font;

  for(y = 0; y < 16; y++)
    { TFT_GRAM_address(xPos,yPos+y);
      font = symbol[code][y];
      for(x = 0; x < 16; x++)
        { if((font << x) & 0x8000) TFT_data(colorfore);	
	  else                     TFT_data(colorback);
	}
    }
}

unsigned char icon_flag = 0;			// icon touch flag

void Check_organ(void)				/* check organ key or icon */
{
  Touch_screen_input();				// input touch screen
  if((icon_flag == 0) && (x_touch >= 32) && (x_touch <= 55) && (y_touch >= 61) && (y_touch <= 81))
    { icon_flag = 1;				// volume down
      Draw_left(32,61, Magenta,Magenta);
      if(volume > 1)
	{ Beep();
	  volume--;
	}
      else
	Beep_3times();
      TFT_string(8,8, Silver,Black, "12345");
      if(volume == 1)      TFT_string( 8,8, Red,Black,"1");
      else if(volume == 2) TFT_string( 9,8, Red,Black,"2");
      else if(volume == 3) TFT_string(10,8, Red,Black,"3");
      else if(volume == 4) TFT_string(11,8, Red,Black,"4");
      else                 TFT_string(12,8, Red,Black,"5");
      Delay_ms(200);
      Draw_left(32,61, White,Cyan);
    }
  else if((icon_flag == 0) && (x_touch >= 111) && (x_touch <= 134) && (y_touch >= 61) && (y_touch <= 81))
    { icon_flag = 1;				// volume up
      Draw_right(111,61, Magenta,Magenta);
      if(volume < 5)
	{ Beep();
	  volume++;
	}
      else
	Beep_3times();
      TFT_string(8,8, Silver,Black, "12345");
      if(volume == 1)      TFT_string( 8,8, Red,Black,"1");
      else if(volume == 2) TFT_string( 9,8, Red,Black,"2");
      else if(volume == 3) TFT_string(10,8, Red,Black,"3");
      else if(volume == 4) TFT_string(11,8, Red,Black,"4");
      else                 TFT_string(12,8, Red,Black,"5");
      Delay_ms(200);
      Draw_right(111,61, White,Cyan);
    }
  else if((icon_flag == 0) && (x_touch >= 184) && (x_touch <= 207) && (y_touch >= 61) && (y_touch <= 81))
    { icon_flag = 1;				// tempo down
      Draw_left(184,61, Magenta,Magenta);
      if(tempo > 1)
	{ Beep();
	  tempo--;
	}
      else
	Beep_3times();
      TFT_string(27,8, Silver,Black, "12345");
      if(tempo == 1)      TFT_string(27,8, Red,Black,"1");
      else if(tempo == 2) TFT_string(28,8, Red,Black,"2");
      else if(tempo == 3) TFT_string(29,8, Red,Black,"3");
      else if(tempo == 4) TFT_string(30,8, Red,Black,"4");
      else                TFT_string(31,8, Red,Black,"5");
      Delay_ms(200);
      Draw_left(184,61, White,Cyan);
    }
  else if((icon_flag == 0) && (x_touch >= 263) && (x_touch <= 286) && (y_touch >= 61) && (y_touch <= 81))
    { icon_flag = 1;				// tempo up
      Draw_right(263,61, Magenta,Magenta);
      if(tempo < 5)
	{ Beep();
	  tempo++;
	}
      else
	Beep_3times();
      TFT_string(27,8, Silver,Black, "12345");
      if(tempo == 1)      TFT_string(27,8, Red,Black,"1");
      else if(tempo == 2) TFT_string(28,8, Red,Black,"2");
      else if(tempo == 3) TFT_string(29,8, Red,Black,"3");
      else if(tempo == 4) TFT_string(30,8, Red,Black,"4");
      else                TFT_string(31,8, Red,Black,"5");
      Delay_ms(200);
      Draw_right(263,61, White,Cyan);
    }
  else if((icon_flag == 0) && (x_touch >= 11) && (x_touch <= 34) && (y_touch >= 118) && (y_touch <= 138))
    { icon_flag = 1;				// previous song
      Draw_up(11,118, Magenta,Magenta);
      if(song > 1)
	{ Beep();
	  song--;
	}
      else
	Beep_3times();
      if(song == 1)      TFT_string( 5,15,Cyan,Black,"        (1) 고향의 봄         ");
      else if(song == 2) TFT_string( 5,15,Cyan,Black,"      (2) 산바람 강바람       ");
      else if(song == 3) TFT_string( 5,15,Cyan,Black,"         (3) 퐁당퐁당         ");
      else if(song == 4) TFT_string( 5,15,Cyan,Black,"        (4) 낭랑 18세         ");
      else if(song == 5) TFT_string( 5,15,Cyan,Black,"     (5) 돌아가는 삼각지      ");
      else if(song == 6) TFT_string( 5,15,Cyan,Black,"          (6) 칠갑산          ");
      else               TFT_string( 5,15,Cyan,Black,"           (7) 허공           ");
      Delay_ms(200);
      Draw_up(11,118, White,Cyan);
    }
  else if((icon_flag == 0) && (x_touch >= 285) && (x_touch <= 308) && (y_touch >= 118) && (y_touch <= 138))
    { icon_flag = 1;				// next song
      Draw_down(285,118, Magenta,Magenta);
      if(song < 7)
	{ Beep();
	  song++;
	}
      else
	Beep_3times();
      if(song == 1)      TFT_string( 5,15,Cyan,Black,"        (1) 고향의 봄         ");
      else if(song == 2) TFT_string( 5,15,Cyan,Black,"      (2) 산바람 강바람       ");
      else if(song == 3) TFT_string( 5,15,Cyan,Black,"         (3) 퐁당퐁당         ");
      else if(song == 4) TFT_string( 5,15,Cyan,Black,"        (4) 낭랑 18세         ");
      else if(song == 5) TFT_string( 5,15,Cyan,Black,"     (5) 돌아가는 삼각지      ");
      else if(song == 6) TFT_string( 5,15,Cyan,Black,"          (6) 칠갑산          ");
      else               TFT_string( 5,15,Cyan,Black,"           (7) 허공           ");
      Delay_ms(200);
      Draw_down(285,118, White,Cyan);
    }
  else if((icon_flag == 0) && (x_touch >= 80) && (x_touch <= 240) && (y_touch >= 118) && (y_touch <= 138))
    { Rectangle(38,118, 281,138, Magenta);	// auto play
      if(song == 1)      Play_song1();
      else if(song == 2) Play_song2();
      else if(song == 3) Play_song3();
      else if(song == 4) Play_song4();
      else if(song == 5) Play_song5();
      else if(song == 6) Play_song6();
      else if(song == 7) Play_song7();
      Rectangle(38,118, 281,138, Cyan);
    }
  else if((icon_flag == 0) && (x_touch >= 1) && (x_touch <= 18) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key F3
      organ_key = F3;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 11) && (x_touch <= 28) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key F3X
      organ_key = F3X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 21) && (x_touch <= 38) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key G3
      organ_key = G3;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 31) && (x_touch <= 48) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key G3X
      organ_key = G3X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 41) && (x_touch <= 58) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key A3
      organ_key = A3;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 51) && (x_touch <= 68) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key A3X
      organ_key = A3X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 61) && (x_touch <= 78) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key B3
      organ_key = B3;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 81) && (x_touch <= 98) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key C4
      organ_key = C4;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 91) && (x_touch <= 108) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key C4X
      organ_key = C4X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 101) && (x_touch <= 118) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key D4
      organ_key = D4;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 111) && (x_touch <= 128) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key D4X
      organ_key = D4X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 121) && (x_touch <= 138) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key E4
      organ_key = E4;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 141) && (x_touch <= 158) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key F4
      organ_key = F4;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 151) && (x_touch <= 168) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key F4X
      organ_key = F4X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 161) && (x_touch <= 178) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key G4
      organ_key = G4;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 171) && (x_touch <= 188) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key G4X
      organ_key = G4X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 181) && (x_touch <= 198) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key A4
      organ_key = A4;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 191) && (x_touch <= 208) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key A4X
      organ_key = A4X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 201) && (x_touch <= 218) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key B4
      organ_key = B4;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 221) && (x_touch <= 238) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key C5
      organ_key = C5;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 231) && (x_touch <= 248) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key C5X
      organ_key = C5X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 241) && (x_touch <= 258) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key D5
      organ_key = D5;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 251) && (x_touch <= 268) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key D5X
      organ_key = D5X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 261) && (x_touch <= 278) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key E5
      organ_key = E5;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 281) && (x_touch <= 298) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key F5
      organ_key = F5;
      Play_note(organ_key, 0);
    }
   else if((icon_flag == 0) && (x_touch >= 291) && (x_touch <= 308) && (y_touch >= 148) && (y_touch <= 205))
    { icon_flag = 1;				// key F5X
      organ_key = F5X;
      Play_note(organ_key, 0);
    }
  else if((icon_flag == 0) && (x_touch >= 301) && (x_touch <= 318) && (y_touch >= 206) && (y_touch <= 238))
    { icon_flag = 1;				// key G5
      organ_key = G5;
      Play_note(organ_key, 0);
    }

  else if((x_touch == 0) && (y_touch == 0))
    { icon_flag = 0;				// key off
      Play_note(organ_key, 0xFF);
    }
}

void Play_note(U08 sound, U32 note)		/* play note */
{
  float frequency;
  unsigned long period;

  if(note == 0xFF)
    { SysTick->CTRL  = 0x00000001;		// 216MHz/8 = 27MHz, enable SysTick and no interrupt
      DAC->DHR12R1 = 0;				// clear output data
      DAC->DHR12R2 = 0;
      if(sound == F3)       Draw_symbol(2,210, White,White, 4);	// clear note4 symbol
      else if(sound == F3X) Draw_symbol(12,165, Black,Black, 4);
      else if(sound == G3)  Draw_symbol(22,210, White,White, 4);
      else if(sound == G3X) Draw_symbol(32,165, Black,Black, 4);
      else if(sound == A3)  Draw_symbol(42,210, White,White, 4);
      else if(sound == A3X) Draw_symbol(52,165, Black,Black, 4);
      else if(sound == B3)  Draw_symbol(62,210, White,White, 4);
      else if(sound == C4)  Draw_symbol(82,210, White,White, 4);
      else if(sound == C4X) Draw_symbol(92,165, Black,Black, 4);
      else if(sound == D4)  Draw_symbol(102,210, White,White, 4);
      else if(sound == D4X) Draw_symbol(112,165, Black,Black, 4);
      else if(sound == E4)  Draw_symbol(122,210, White,White, 4);
      else if(sound == F4)  Draw_symbol(142,210, White,White, 4);
      else if(sound == F4X) Draw_symbol(152,165, Black,Black, 4);
      else if(sound == G4)  Draw_symbol(162,210, White,White, 4);
      else if(sound == G4X) Draw_symbol(172,165, Black,Black, 4);
      else if(sound == A4)  Draw_symbol(182,210, White,White, 4);
      else if(sound == A4X) Draw_symbol(192,165, Black,Black, 4);
      else if(sound == B4)  Draw_symbol(202,210, White,White, 4);
      else if(sound == C5)  Draw_symbol(222,210, White,White, 4);
      else if(sound == C5X) Draw_symbol(232,165, Black,Black, 4);
      else if(sound == D5)  Draw_symbol(242,210, White,White, 4);
      else if(sound == D5X) Draw_symbol(252,165, Black,Black, 4);
      else if(sound == E5)  Draw_symbol(262,210, White,White, 4);
      else if(sound == F5)  Draw_symbol(282,210, White,White, 4);
      else if(sound == F5X) Draw_symbol(292,165, Black,Black, 4);
      else if(sound == G5)  Draw_symbol(302,210, White,White, 4);
      else if(sound == G5X) Draw_symbol(312,165, Black,Black, 4);
      return;
    }
  else if(sound == F3)  { Draw_symbol(2,210, Red,White, 4); frequency = 174.614; }
  else if(sound == F3X) { Draw_symbol(12,165, Red,Black, 4); frequency = 184.997; }
  else if(sound == G3)  { Draw_symbol(22,210, Red,White, 4); frequency = 195.998; }
  else if(sound == G3X) { Draw_symbol(32,165, Red,Black, 4); frequency = 207.652; }
  else if(sound == A3)  { Draw_symbol(42,210, Red,White, 4); frequency = 220.000; }
  else if(sound == A3X) { Draw_symbol(52,165, Red,Black, 4); frequency = 233.082; }
  else if(sound == B3)  { Draw_symbol(62,210, Red,White, 4); frequency = 246.942; }
  else if(sound == C4)  { Draw_symbol(82,210, Red,White, 4); frequency = 261.626; }
  else if(sound == C4X) { Draw_symbol(92,165, Red,Black, 4); frequency = 277.183; }
  else if(sound == D4)  { Draw_symbol(102,210, Red,White, 4); frequency = 293.665; }
  else if(sound == D4X) { Draw_symbol(112,165, Red,Black, 4); frequency = 311.127; }
  else if(sound == E4)  { Draw_symbol(122,210, Red,White, 4); frequency = 329.628; }
  else if(sound == F4)  { Draw_symbol(142,210, Red,White, 4); frequency = 349.228; }
  else if(sound == F4X) { Draw_symbol(152,165, Red,Black, 4); frequency = 369.994; }
  else if(sound == G4)  { Draw_symbol(162,210, Red,White, 4); frequency = 391.995; }
  else if(sound == G4X) { Draw_symbol(172,165, Red,Black, 4); frequency = 415.305; }
  else if(sound == A4)  { Draw_symbol(182,210, Red,White, 4); frequency = 440.000; }
  else if(sound == A4X) { Draw_symbol(192,165, Red,Black, 4); frequency = 466.164; }
  else if(sound == B4)  { Draw_symbol(202,210, Red,White, 4); frequency = 493.883; }
  else if(sound == C5)  { Draw_symbol(222,210, Red,White, 4); frequency = 523.251; }
  else if(sound == C5X) { Draw_symbol(232,165, Red,Black, 4); frequency = 554.365; }
  else if(sound == D5)  { Draw_symbol(242,210, Red,White, 4); frequency = 587.330; }
  else if(sound == D5X) { Draw_symbol(252,165, Red,Black, 4); frequency = 622.254; }
  else if(sound == E5)  { Draw_symbol(262,210, Red,White, 4); frequency = 659.255; }
  else if(sound == F5)  { Draw_symbol(282,210, Red,White, 4); frequency = 698.456; }
  else if(sound == F5X) { Draw_symbol(292,165, Red,Black, 4); frequency = 739.989; }
  else if(sound == G5)  { Draw_symbol(302,210, Red,White, 4); frequency = 783.991; }
  else if(sound == G5X) { Draw_symbol(312,165, Red,Black, 4); frequency = 830.609; }
  else if(sound == A5)  frequency = 880.000;
  else if(sound == A5X) frequency = 932.328;

  toggle_flag = 0;
  period = (unsigned long)(27000000./2./frequency + 0.5);
  SysTick->LOAD  = period - 1;			// N-1 for frequecny = 27MHz/2/frequency - 1
  SysTick->VAL   = 0;				// clear SysTick Counter
  SysTick->CTRL  = 0x00000003;			// 216MHz/8 = 27MHz, enable SysTick and interrupt

  if(note != 0)
    { Delay_ms(note*7*(7-tempo));		// note play time
      SysTick->CTRL  = 0x00000001;		// 216MHz/8 = 27MHz, enable SysTick and no interrupt
      DAC->DHR12R1 = 0;				// clear output data
      DAC->DHR12R2 = 0;
      if(sound == F3)       Draw_symbol(2,210, White,White, 4);	// clear note4 symbol
      else if(sound == F3X) Draw_symbol(12,165, Black,Black, 4);
      else if(sound == G3)  Draw_symbol(22,210, White,White, 4);
      else if(sound == G3X) Draw_symbol(32,165, Black,Black, 4);
      else if(sound == A3)  Draw_symbol(42,210, White,White, 4);
      else if(sound == A3X) Draw_symbol(52,165, Black,Black, 4);
      else if(sound == B3)  Draw_symbol(62,210, White,White, 4);
      else if(sound == C4)  Draw_symbol(82,210, White,White, 4);
      else if(sound == C4X) Draw_symbol(92,165, Black,Black, 4);
      else if(sound == D4)  Draw_symbol(102,210, White,White, 4);
      else if(sound == D4X) Draw_symbol(112,165, Black,Black, 4);
      else if(sound == E4)  Draw_symbol(122,210, White,White, 4);
      else if(sound == F4)  Draw_symbol(142,210, White,White, 4);
      else if(sound == F4X) Draw_symbol(152,165, Black,Black, 4);
      else if(sound == G4)  Draw_symbol(162,210, White,White, 4);
      else if(sound == G4X) Draw_symbol(172,165, Black,Black, 4);
      else if(sound == A4)  Draw_symbol(182,210, White,White, 4);
      else if(sound == A4X) Draw_symbol(192,165, Black,Black, 4);
      else if(sound == B4)  Draw_symbol(202,210, White,White, 4);
      else if(sound == C5)  Draw_symbol(222,210, White,White, 4);
      else if(sound == C5X) Draw_symbol(232,165, Black,Black, 4);
      else if(sound == D5)  Draw_symbol(242,210, White,White, 4);
      else if(sound == D5X) Draw_symbol(252,165, Black,Black, 4);
      else if(sound == E5)  Draw_symbol(262,210, White,White, 4);
      else if(sound == F5)  Draw_symbol(282,210, White,White, 4);
      else if(sound == F5X) Draw_symbol(292,165, Black,Black, 4);
      else if(sound == G5)  Draw_symbol(302,210, White,White, 4);
      else if(sound == G5X) Draw_symbol(312,165, Black,Black, 4);
    }
}

void Play_rest(U32 rest)			/* play rest */
{
  Delay_ms(rest*7*(7-tempo));			// rest play time
}

void Set_tempo(U08 tempo_value)			/* set tempo */
{
  tempo = tempo_value;

  TFT_string(27,8, Silver,Black, "12345");
  if(tempo == 1)      TFT_string(27,8, Red,Black,"1");
  else if(tempo == 2) TFT_string(28,8, Red,Black,"2");
  else if(tempo == 3) TFT_string(29,8, Red,Black,"3");
  else if(tempo == 4) TFT_string(30,8, Red,Black,"4");
  else                TFT_string(31,8, Red,Black,"5");
}

void Play_song1(void)				/* play song 1 - 고향의 봄*/
{
  Set_tempo(3);					// tempo = 3(normal speed)

  Play_note(G4,NOTE4);				// line 1
  Play_note(G4,NOTE4);
  Play_note(E4,NOTE8);
  Play_note(F4,NOTE8);
  Play_note(G4,NOTE4);
  Play_note(A4,NOTE4);
  Play_note(A4,NOTE4);
  Play_note(G4,NOTE4D);
  Play_rest(REST8);
  Play_note(G4,NOTE4);
  Play_note(C5,NOTE4);
  Play_note(E5,NOTE4);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE2D);
  Play_rest(REST4);
  Play_note(E5,NOTE4);				// line 2
  Play_note(E5,NOTE4);
  Play_note(D5,NOTE4);
  Play_note(D5,NOTE4);
  Play_note(C5,NOTE4);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(A4,NOTE4D);
  Play_rest(REST8);
  Play_note(G4,NOTE4);
  Play_note(G4,NOTE4);
  Play_note(G4,NOTE4);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(C4,NOTE2D);
  Play_rest(REST4);
  Play_note(D4,NOTE4);				// line 3
  Play_note(D4,NOTE4);
  Play_note(E4,NOTE4);
  Play_note(C4,NOTE4);
  Play_note(D4,NOTE4);
  Play_note(D4,NOTE4);
  Play_note(E4,NOTE4);
  Play_note(G4,NOTE4);
  Play_note(A4,NOTE4);
  Play_note(C5,NOTE4);
  Play_note(E5,NOTE4);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE2D);
  Play_rest(REST4);
  Play_note(E5,NOTE4);				// line 4
  Play_note(E5,NOTE4);
  Play_note(D5,NOTE4);
  Play_note(D5,NOTE4);
  Play_note(C5,NOTE4);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(A4,NOTE4);
  Play_note(A4,NOTE4);
  Play_note(G4,NOTE4);
  Play_note(G4,NOTE4);
  Play_note(G4,NOTE4);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(C4,NOTE2D);
  Play_rest(REST4);
}

void Play_song2(void)				/* play song 2 - 산바람 강바람*/
{
  Set_tempo(3);					// tempo = 3(normal speed)

  Play_note(A4,NOTE4D);				// line 1
  Play_note(B4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(E4,NOTE4D);
  Play_note(F4X,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(F4X,NOTE4D);
  Play_note(A4,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(B4,NOTE8);
  Play_note(A4,NOTE2);
  Play_rest(REST4);
  Play_note(F4X,NOTE4D);			// line 2
  Play_note(A4,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(B4,NOTE4D);
  Play_note(A4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE4D);
  Play_note(G4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE2);
  Play_rest(REST4);
  Play_note(E4,NOTE2);				// line 3
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(F4X,NOTE4D);
  Play_note(F4X,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(G4,NOTE4D);
  Play_note(F4X,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(A4,NOTE2);
  Play_rest(REST4);
  Play_note(F4X,NOTE4D);			// line 4
  Play_note(A4,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(B4,NOTE4D);
  Play_note(A4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE4D);
  Play_note(G4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE2);
  Play_rest(REST4);
}

void Play_song3(void)				/* play song 3 - 퐁당퐁당*/
{
  Set_tempo(3);					// tempo = 3(normal speed)

  Play_note(C4,NOTE8);				// line 1
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(C4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(G4,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(C4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(C4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(G4,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(A4,NOTE4);      			// line 2
  Play_note(G4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(A4,NOTE4);
  Play_note(G4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(C4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(G4,NOTE4);
  Play_note(A4,NOTE8D);	           	 	// line 3
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE8);
  Play_rest(REST8);
  Play_note(G4,NOTE8D);
  Play_note(G4,NOTE16);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(C4,NOTE4D);
  Play_rest(REST8);
  Play_note(D4,NOTE8D);	        	    	// line 4
  Play_note(E4,NOTE16);
  Play_note(C4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(G4,NOTE4);
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE16);
  Play_note(F4,NOTE16);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(C4,NOTE4D);
  Play_rest(REST8);
}

void Play_song4(void)				/* play song 4 - 낭랑 18세*/
{
  Set_tempo(4);					// tempo = 4(fast speed)

  Play_note(G4,NOTE4D);		            	// line 1
  Play_note(G4,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(C4,NOTE4);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(C5,NOTE8);
  Play_note(B4,NOTE8);
  Play_note(A4,NOTE2);
  Play_note(G4,NOTE4D);		            	// line 2
  Play_note(G4,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(C4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(D4,NOTE2);
  Play_note(G4,NOTE4D);		            	// line 3
  Play_note(G4,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(C4,NOTE4);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(C5,NOTE8);
  Play_note(B4,NOTE8);
  Play_note(A4,NOTE2);
  Play_note(G4,NOTE8);		             	// line 4
  Play_note(E5,NOTE4);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE8D);
  Play_note(C5,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE2);
  Play_note(G4,NOTE4D);				// line 5
  Play_note(E5,NOTE8);
  Play_note(C5,NOTE4);
  Play_note(G4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(C4,NOTE2D);
  Play_rest(REST8);
  Play_note(G4,NOTE4D);				// line 6
  Play_note(G5,NOTE8);
  Play_note(E5,NOTE4);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(C5,NOTE2D);
  Play_note(G4,NOTE4D);				// line 7
  Play_note(G4,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(C4,NOTE4);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(C5,NOTE8);
  Play_note(B4,NOTE8);
  Play_note(A4,NOTE2);
  Play_note(G4,NOTE8);	         	    	// line 8
  Play_note(E5,NOTE4);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE8D);
  Play_note(C5,NOTE16);
  Play_note(G4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(C5,NOTE2);
}

void Play_song5(void)				/* play song 5 - 돌아가는 삼각지*/
{
  Set_tempo(2);					// tempo = 2(slow speed)

  Play_note(C4,NOTE4);	  	           	// prelude
  Play_note(C4,NOTE16);
  Play_note(D4,NOTE16);
  Play_note(E4,NOTE16);
  Play_note(G4,NOTE16);
  Play_note(E5,NOTE4D);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE4);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE16);
  Play_note(E4,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE2);
  Play_note(C5,NOTE8);
  Play_note(A4,NOTE16);
  Play_note(C5,NOTE16);
  Play_note(A4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(E4,NOTE4);
  Play_note(G4,NOTE16);
  Play_note(E4,NOTE16);
  Play_note(D4,NOTE16);
  Play_note(E4,NOTE16);
  Play_note(C4,NOTE2D);
  Play_rest(REST8);	        	       	// line 1
  Play_note(C4,NOTE8);
  Play_note(C4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE4);
  Play_note(G4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(G4,NOTE1);
  Play_note(E4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(E5,NOTE4);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE2D);
  Play_rest(REST8);
  Play_note(E5,NOTE4D);		            	// line 2
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE4);
  Play_note(A4,NOTE16);
  Play_note(C5,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(G4,NOTE16);
  Play_rest(REST8);
  Play_note(E4,NOTE16);
  Play_note(G4,NOTE16);
  Play_note(A4,NOTE16);
  Play_note(C5,NOTE16);
  Play_note(A4,NOTE8);
  Play_note(G4,NOTE2);
  Play_rest(REST8);
  Play_note(A3,NOTE8);
  Play_note(A3,NOTE8);
  Play_note(C4,NOTE8);
  Play_note(D4,NOTE4);
  Play_note(E4,NOTE16);
  Play_note(D4,NOTE16);
  Play_note(C4,NOTE16);
  Play_note(A3,NOTE16);
  Play_note(C4,NOTE2D);
  Play_rest(REST8);
  Play_note(D4,NOTE4D);				// line 3
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE4);
  Play_note(C4,NOTE8);
  Play_note(D4,NOTE8);
  Play_rest(REST8);
  Play_note(E4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(C5,NOTE2);
  Play_rest(REST8);
  Play_note(G4,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(E5,NOTE4);
  Play_note(D5,NOTE16T);
  Play_note(E5,NOTE16T);
  Play_note(D5,NOTE16T);
  Play_note(C5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(E5,NOTE2D);
  Play_rest(REST8);
  Play_note(F5,NOTE4D);	 	           	// line 4
  Play_note(E5,NOTE8);
  Play_note(D5,NOTE4);
  Play_note(C5,NOTE4);
  Play_rest(REST8);
  Play_note(A4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE4);
  Play_note(A4,NOTE8);
  Play_note(G4,NOTE8);
  Play_rest(REST8);
  Play_note(E4,NOTE16);
  Play_note(E4,NOTE16);
  Play_note(D4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(G4,NOTE4);
  Play_note(E4,NOTE16T);
  Play_note(F4,NOTE16T);
  Play_note(E4,NOTE16T);
  Play_note(D4,NOTE8);
  Play_note(C4,NOTE2D);
}

void Play_song6(void)				/* play song 6 - 칠갑산*/
{
  Set_tempo(3);					// tempo = 3(normal speed)

  Play_note(G5,NOTE16);				// prelude line 1
  Play_note(A5X,NOTE16);
  Play_note(G5,NOTE2);
  Play_note(G5,NOTE16);
  Play_note(F5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(F5,NOTE16);
  Play_note(G5,NOTE16);
  Play_note(A5X,NOTE16);
  Play_note(G5,NOTE2D);
  Play_note(D5,NOTE16);
  Play_note(F5,NOTE16);
  Play_note(D5,NOTE2);
  Play_note(D5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(C5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(G5,NOTE16);
  Play_note(F5,NOTE16);
  Play_note(D5,NOTE2D);
  Play_rest(REST32);
  Play_note(D4,NOTE4D);	   	         	// prelude line 2
  Play_note(D4,NOTE8);
  Play_note(F4,NOTE8);
  Play_note(D4,NOTE16);
  Play_note(F4,NOTE16);
  Play_note(G4,NOTE2);
  Play_note(A4X,NOTE4);
  Play_note(G4,NOTE2);
  Play_note(G5,NOTE16);
  Play_note(F5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(F5,NOTE16);
  Play_note(G5,NOTE16);
  Play_note(A5X,NOTE16);
  Play_note(G5,NOTE2);
  Play_rest(REST8);
  Play_note(D5,NOTE4);				// line 1
  Play_note(D5,NOTE2);
  Play_note(D5,NOTE16);
  Play_note(F5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(C5,NOTE16);
  Play_note(D5,NOTE2);
  Play_note(C5,NOTE8);
  Play_note(A4X,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(A4X,NOTE8T);
  Play_note(A4,NOTE8T);
  Play_note(G4,NOTE8T);
  Play_note(D4,NOTE2);
  Play_rest(REST8);
  Play_note(D4,NOTE8);	     	        	// line 2
  Play_note(F4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(A4X,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE2);
  Play_note(C5,NOTE8T);
  Play_note(A4X,NOTE8T);
  Play_note(C5,NOTE8T);
  Play_note(D5,NOTE1);
  Play_rest(REST4);
  Play_note(D5,NOTE4);				// line 3
  Play_note(D5,NOTE4);
  Play_note(F5,NOTE4);
  Play_note(G5,NOTE2);
  Play_note(F5,NOTE4);
  Play_note(D5,NOTE4);
  Play_note(F5,NOTE4);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE2);
  Play_rest(REST8);
  Play_note(C5,NOTE8);	     	        	// line 4
  Play_note(A4X,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(A4X,NOTE8T);
  Play_note(A4,NOTE8T);
  Play_note(G4,NOTE8T);
  Play_note(D4,NOTE2);
  Play_note(A4X,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(G4,NOTE1);
  Play_rest(REST4);
  Play_note(A4,NOTE4D);	    	        	// line 5
  Play_note(A4X,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(A4X,NOTE8);
  Play_note(A4,NOTE4D);
  Play_note(D5,NOTE8);
  Play_note(D5,NOTE4);
  Play_note(G4,NOTE4);
  Play_note(G4,NOTE4);
  Play_note(G4,NOTE8);
  Play_note(F4,NOTE8);
  Play_note(G4,NOTE2);
  Play_rest(REST4);
  Play_note(G4,NOTE4);	    	         	// line 6
  Play_note(A4X,NOTE4);
  Play_note(C5,NOTE4);
  Play_note(D5,NOTE4D);
  Play_note(F5,NOTE8);
  Play_note(G5,NOTE8);
  Play_note(F5,NOTE8);
  Play_note(D5,NOTE1);
  Play_rest(REST4);
  Play_note(G5,NOTE4D);	  	          	// line 7
  Play_note(F5,NOTE8);
  Play_note(G5,NOTE8);
  Play_note(F5,NOTE8);
  Play_note(D5,NOTE2D);
  Play_note(C5,NOTE8);
  Play_note(A4X,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(A4X,NOTE8T);
  Play_note(A4,NOTE8T);
  Play_note(G4,NOTE8T);
  Play_note(D4,NOTE2);
  Play_rest(REST8);
  Play_note(D4,NOTE8);	 	            	// line 8
  Play_note(F4,NOTE8);
  Play_note(G4,NOTE8);
  Play_note(A4X,NOTE8);
  Play_note(C5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(C5,NOTE2);
  Play_note(A4X,NOTE8T);
  Play_note(C5,NOTE8T);
  Play_note(A4X,NOTE8T);
  Play_note(G4,NOTE1);
}

void Play_song7(void)				/* play song 7 - 허공*/
{
  Set_tempo(3);					// tempo = 3(normal speed)

  Play_note(A4,NOTE8);	  	           	// line 1
  Play_note(B4,NOTE8);
  Play_note(F4X,NOTE4D);
  Play_note(E4,NOTE16);
  Play_note(D4,NOTE16);
  Play_note(F4X,NOTE2D);
  Play_note(E4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(B3,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(A3,NOTE2D);
  Play_note(D4,NOTE8);	  	    	       	// line 2
  Play_note(A3,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(E4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(B4,NOTE2);
  Play_note(F4X,NOTE8);
  Play_note(A4,NOTE1);
  Play_rest(REST4);
  Play_note(D4,NOTE8);	         	    	// line 3
  Play_note(D4,NOTE8);
  Play_note(D5,NOTE4D);
  Play_note(D5,NOTE8);
  Play_note(D5,NOTE2D);
  Play_note(B4,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(B4,NOTE4D);
  Play_note(A4,NOTE8);
  Play_note(F4X,NOTE2D);
  Play_note(E4,NOTE8);	        	     	// line 4
  Play_note(E4,NOTE8);
  Play_note(E4,NOTE4D);
  Play_note(B4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(F4X,NOTE4D);
  Play_note(E4,NOTE8);
  Play_note(D4,NOTE1);
  Play_rest(REST4);
  Play_note(D5,NOTE8);	     	        	// line 5
  Play_note(E5,NOTE8);
  Play_note(E5,NOTE4D);
  Play_note(E5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(E5,NOTE8);
  Play_note(F5X,NOTE2);
  Play_note(E5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(B4,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(E5,NOTE8);
  Play_note(F5X,NOTE8);
  Play_note(E5,NOTE2D);
  Play_note(F5X,NOTE8);	    	        	// line 6
  Play_note(F5X,NOTE8);
  Play_note(F5X,NOTE4D);
  Play_note(E5,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(E5,NOTE16);
  Play_note(D5,NOTE16);
  Play_note(B4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(F4X,NOTE4);
  Play_note(B4,NOTE8);
  Play_note(E5,NOTE8);
  Play_note(D5,NOTE4);
  Play_note(B4,NOTE8);
  Play_note(D5,NOTE8);
  Play_note(A4,NOTE2D);
  Play_note(A3,NOTE8);	    	         	// line 7
  Play_note(B3,NOTE8);
  Play_note(D4,NOTE4D);
  Play_note(E4,NOTE8);
  Play_note(F4X,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(A4,NOTE2);
  Play_note(D4,NOTE8);
  Play_note(D4,NOTE8);
  Play_note(D5,NOTE4D);
  Play_note(D5,NOTE8);
  Play_note(B4,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(F4X,NOTE2);
  Play_note(A4,NOTE8);	    	         	// line 8
  Play_note(F5X,NOTE8);
  Play_note(F5X,NOTE4);
  Play_note(F5X,NOTE4);
  Play_note(E5,NOTE4D);
  Play_note(F5X,NOTE8);
  Play_note(A4,NOTE8);
  Play_note(B4,NOTE8);
  Play_note(D5,NOTE1);
}
