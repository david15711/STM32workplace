/* ============================================================================ */
/*  Exp20_1_MP3(1).c : VS1053b를 이용한 MP3 플레이어(MP3/AAC/WMA/WAV/MIDI 재생)	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_SD_card.h"
#include "OK-STM767_VS1053b.h"

void TFT_filename(void);			// display MP3 file name, number, size
void TFT_volume(void);				// display volume
void TFT_bass(void);				// display bass
void TFT_treble(void);				// display treble
void Check_valid_increment_file(void);		// check if valid file for increment
void Check_valid_decrement_file(void);		// check if valid file for decrement

unsigned char total_file;			// total file number
unsigned char file_number = 0;			// current file number

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned char  i, key;
  unsigned char  func_mode = 0;			// function key mode
  unsigned char  play_flag = 0;			// play or stop flag
  unsigned short index = 512;			// byte index in a sector
  unsigned short loop = 0;			// MP3 play loop counter
  unsigned int   percentage;			// play percentage

  unsigned int   MP3_start_sector[MAX_FILE];	// MP3 file start sector
  unsigned int   MP3_start_backup[MAX_FILE];
  unsigned char  MP3buffer[512];   		// MP3 data from SD card
  unsigned int   MP3_end_sector;		// MP3 file end sector

  unsigned short time;				// decode time

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module
  Initialize_touch_screen();			// initialize touch screen

  LCD_string(0x80," OK-STM767 V1.0 ");		// display title
  LCD_string(0xC0,"   Exp20_1.c    ");

  TFT_string(0,4,Green,Black,"****************************************");
  TFT_string(0,6,White,Black,"  OK-STM767 키트를 이용한 MP3 플레이어  ");
  TFT_string(0,8,Green,Black,"****************************************");
  TFT_string(0,23,Cyan,Black,"           SD 카드 초기화...            ");
  Beep();
  Delay_ms(1000);
  TFT_clear_screen();

  Initialize_SD();				// initialize SD card
  Initialize_FAT32();				// initialize FAT32 file system
  Initialize_VS1053b();				// initialize VS1053b
  Delay_ms(1000);

  volume = 175;					// initial volume = 175/250(70%)
  VS1053b_SetVolume(volume);
  Delay_ms(1);
  bass = 10;					// initial bass = 10 and treble = 5
  treble = 5;
  VS1053b_SetBassTreble(bass,treble);

  TFT_clear_screen();				// display basic screen
  TFT_string(0, 0, White,Magenta, "  OK-STM767 키트를 이용한 MP3 플레이어  ");
  TFT_string(0, 3, Cyan,   Black, "----------------------------------------");
  TFT_string(0, 5, Magenta,Black, ">>");
  TFT_string(0, 7, Cyan,   Black, "----------------------------------------");
  TFT_string(0, 9, Cyan,   Black, "      파일 번호 : 000/000               ");
  TFT_string(0,11, Cyan,   Black, "      파일 용량 : 0000KB                ");
  TFT_string(0,13, Cyan,   Black, "      연주 진행 : 00:00(000%)           ");
  TFT_string(0,15, Cyan,   Black, "----------------------------------------");
  TFT_string(0,17, Green,  Black, "   음량(Volume) : 000% (000/250)        ");
  TFT_string(0,19, Green,  Black, "   저음(Bass)   :  00  (00 ~ 15)        ");
  TFT_string(0,21, Green,  Black, "   고음(Treble) :  00  (-8 ~ +7)        ");
  TFT_string(0,23, Cyan,   Black, "----------------------------------------");
  TFT_string(0,25, Cyan,   Black, "   KEY1      KEY2      KEY3      KEY4   ");
  TFT_string(0,27, Magenta,Black, "  (PLAY)    (FUNC)     (INC)     (DEC)  ");
  TFT_string(18,13, Magenta, Black, "00");
  TFT_string(21,13, Magenta, Black, "00");

  Rectangle( 12,196,  67,235, Yellow);		// display touch key outline
  Rectangle( 92,196, 147,235, Yellow);
  Rectangle(176,196, 231,235, Yellow);
  Rectangle(256,196, 311,235, Yellow);

  TFT_volume();					// display initial play value
  TFT_bass();
  TFT_treble();

  total_file = fatGetDirEntry(FirstDirCluster);	// get total file number

  for(i = 0; i < total_file; i++)		// get start address of all files
    { MP3_start_sector[i] = fatClustToSect(file_start_cluster[i]);
      MP3_start_backup[i] = MP3_start_sector[i];
    }

  TFT_xy(22,9);					// display total file number
  TFT_color(Yellow,Black);
  TFT_unsigned_decimal(total_file,1,3);
  file_number = 0;				// display first file name
  TFT_filename();
  Check_valid_increment_file();

  MP3_end_sector = (file_size[file_number] >> 9) + MP3_start_sector[file_number];

  while(1)
    { if(((GPIOC->IDR & 0x0080) == 0x0080) && (play_flag == 1))	// if VS1053b request MP3 data
        { if(index == 512)
            { if(MP3_end_sector == MP3_start_sector[file_number])
		{ if(file_number != (total_file - 1))
	  	    file_number++;
	 	  else if(file_number == (total_file - 1))
	  	    file_number = 0;

		  TFT_filename();
		  Check_valid_increment_file();

		  MP3_start_sector[file_number] = MP3_start_backup[file_number];
		  MP3_end_sector = (file_size[file_number] >> 9) + MP3_start_sector[file_number];
		  VS1053b_software_reset();	// VS1053b software reset to change music file
		}
	      index = 0;
              SD_read_sector(MP3_start_sector[file_number]++, MP3buffer);
            }

	  for(i = 0; i < 32; i++)		// send 32 data byte
            { GPIOC->BSRR = 0x00400000;		// -MP3_DCS = 0
              SPI3_write(MP3buffer[index++]); 	// write a byte of MP3 data to VS1053b
              GPIOC->BSRR = 0x00000040;		// -MP3_DCS = 1
	    }
	}

      loop++;					// display MP3 file bitrate or play percentage
      if((loop == 500) && (play_flag == 1))
	{ loop = 0;

          time = VS1053b_SCI_Read(0x04);	// decode time
          TFT_xy(18,13);
  	  TFT_color(Magenta, Black);
	  TFT_unsigned_decimal(time / 60,1,2);
          TFT_xy(21,13);
	  TFT_unsigned_decimal(time % 60,1,2);

	  percentage = MP3_end_sector - MP3_start_sector[file_number];
  	  percentage = (unsigned int)((float)percentage / (float)(file_size[file_number] >> 9) * 100.);
	  percentage = 100 - percentage;

  	  TFT_xy(24,13);			// display play percentage
	  TFT_color(Yellow,Black);
          if(percentage >= 100)     TFT_unsigned_decimal(percentage,0,3);
	  else if(percentage >= 10) TFT_unsigned_decimal(percentage,0,2);
	  else                      TFT_unsigned_decimal(percentage,0,1);
  	  TFT_color(Cyan,Black);
          TFT_English('%');
	  TFT_English(')');
          TFT_English(' ');
          TFT_English(' ');
	}

      key = Key_input();			// key input

      switch(key)
	{ case KEY1: play_flag ^= 0x01;		// toggle play or stop
                     break;

          case KEY2: if(func_mode == 0)		// select function
		       { func_mode = 1;
			 TFT_string(0, 5,Magenta,Black,"  ");
			 TFT_string(0,17,Magenta,Black,">>");
		       }
		     else if(func_mode == 1)
		       { func_mode = 2;
			 TFT_string(0,17,Magenta,Black,"  ");
			 TFT_string(0,19,Magenta,Black,">>");
		       }
		     else if(func_mode == 2)
		       { func_mode = 3;
			 TFT_string(0,19,Magenta,Black,"  ");
			 TFT_string(0,21,Magenta,Black,">>");
		       }
		     else
		       { func_mode = 0;
			 TFT_string(0,21,Magenta,Black,"  ");
			 TFT_string(0, 5,Magenta,Black,">>");
		       }
		     break;

	  case KEY3: if(func_mode == 0)		// if func_mode == 0, select next music
		       { if(file_number != (total_file - 1))
			   file_number++;
			 else
			   file_number = 0;

			 VS1053b_software_reset();
			 TFT_filename();
			 Check_valid_increment_file();

			 MP3_start_sector[file_number] = MP3_start_backup[file_number];
			 MP3_end_sector = (file_size[file_number] >> 9) + MP3_start_sector[file_number];
			 index = 512;
			 VS1053b_software_reset(); // VS1053b software reset to change music file
		       }
		     else if(func_mode == 1)	// if func_mode == 1, set volume for increment
		       { if(volume < 250)
			   { volume++;
    		             VS1053b_SetVolume(volume);
			     TFT_volume();
			   }
		       }
		     else if(func_mode == 2)	// if func_mode == 2, set bass for increment
		       { if(bass < 15)
			   { bass++;
    		             VS1053b_SetBassTreble(bass, treble);
			     TFT_bass();
			   }
		       }
		     else			// if func_mode == 3, set treble for increment
		       { if(treble < +7)
			   { treble++;
    		             VS1053b_SetBassTreble(bass, treble);
			     TFT_treble();
			   }
		       }
		     break;

	  case KEY4: if(func_mode == 0)		// if func_mode == 0, select previous music
		       { if(file_number != 0)
			   file_number--;
			 else
			   file_number = total_file - 1;

			 TFT_filename();
			 Check_valid_decrement_file();

			 MP3_start_sector[file_number] = MP3_start_backup[file_number];
			 MP3_end_sector = (file_size[file_number] >> 9) + MP3_start_sector[file_number];
			 index = 512;
			 VS1053b_software_reset(); // VS1053b software reset to change music file
		       }
		     else if(func_mode == 1)	// if func_mode == 1, set volume for decrement
		       { if(volume > 5)
			   { volume--;
    		             VS1053b_SetVolume(volume);
			     TFT_volume();
			   }
		       }
		     else if(func_mode == 2)	// if func_mode == 2, set bass for decrement
		       { if(bass != 0)
			   { bass--;
    		             VS1053b_SetBassTreble(bass, treble);
			     TFT_bass();
			   }
		       }
		     else			// if func_mode == 3, set treble for decrement
		       { if(treble > -8)
			   { treble--;
    		             VS1053b_SetBassTreble(bass, treble);
			     TFT_treble();
			   }
		       }
		     break;

          default:   break;
	}
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void TFT_filename(void)				/* display MP3 file name, number, size */
{
  unsigned char  file_flag;
  unsigned short file_KB;

  TFT_string(0,7, Cyan, Black, "----------------------------------------");
  TFT_string(3,5, Green,Black, "                                     ");

  file_flag = Get_long_filename(file_number);	// check file name

  if(file_flag == 0)				// short file name(8.3 format)
    TFT_short_filename(3,5, Green,Black);
  else if(file_flag == 1)			// long file name
    TFT_long_filename(3,5, Green,Black);
  else if(file_flag == 2)			// file name is longer than 195 characters
    TFT_string(3,5, Red,Black, "* 파일명 길이 초과 *");
  else						// file name error
    TFT_string(3,5, Red,Black, "*** 파일명 오류 ***");

  file_KB = file_size[file_number]/1024;	// calculate file size in KB
  if((file_size[file_number] % 1024) != 0)
    file_KB++;

  if(file_flag != 3)
    { TFT_xy(18,9);				// file number
      TFT_color(Magenta, Black);
      TFT_unsigned_decimal(file_number + 1,1,3);   
      TFT_xy(17,11);				// file size
      TFT_color(Magenta, Black);
      TFT_unsigned_decimal(file_KB,0,5);
    }

  TFT_xy(24,13);				// display percentage = 0
  TFT_color(Yellow,Black);
  TFT_English('0');
  TFT_color(Cyan,Black);
  TFT_English('%');
  TFT_English(')');
  TFT_English(' ');
  TFT_English(' ');
}

void TFT_volume(void)				/* display volume */
{
  TFT_xy(18,17);
  TFT_color(Magenta,Black);
  TFT_unsigned_decimal((unsigned int)(volume*100./250.+0.5), 0, 3);
  TFT_xy(24,17);
  TFT_color(Yellow,Black);
  TFT_unsigned_decimal(volume,1,3);
}

void TFT_bass(void)				/* display bass */
{
  TFT_xy(19,19);
  TFT_color(Magenta,Black);
  TFT_unsigned_decimal(bass,1,2);
}

void TFT_treble(void)				/* display treble */
{
  TFT_xy(19,21);
  TFT_color(Magenta,Black);
  TFT_signed_decimal(treble,0,1);
}

void Check_valid_increment_file(void)		/* check if valid file for increment */
{
  unsigned char file_OK_flag;

  file_OK_flag = 0;
  do{ if((extension != 0x004D5033) && (extension != 0x00414143) && (extension != 0x00574D41) && (extension != 0x004D4944))
	{ if(file_number != (total_file - 1))	// if not MP3/AAC/WMA/MID file, skip   
	    file_number++;
	  else
	    file_number = 0;
          TFT_filename();
	}
      else
        file_OK_flag = 1;
    } while(file_OK_flag == 0);
}

void Check_valid_decrement_file(void)		/* check if valid file for decrement */
{
  unsigned char file_OK_flag;

  file_OK_flag = 0;
  do{ if((extension != 0x004D5033) && (extension != 0x00414143) && (extension != 0x00574D41) && (extension != 0x004D4944))
	{ if(file_number != 0)			// if not MP3/AAC/WMA/MID file, skip   
	    file_number--;
	  else
	    file_number = total_file - 1;
          TFT_filename();
	}
      else
        file_OK_flag = 1;
    } while(file_OK_flag == 0);
}
