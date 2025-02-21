/* ============================================================================ */
/*    Exp06_6_string(2).c : TFT-LCD에 긴 문자열을 천천히 출력하는 프로그램	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void TFT_string_slow(U08 xChar, U08 yChar, U16 colorfore, U16 colorback, char *str);	/* write TFT-LCD string */

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
  Initialize_LCD();				// initialize text LCD module
  Initialize_TFT_LCD();				// initialize TFT-LCD module

  LCD_string(0x80," OK-STM767 V1.0 ");          // display title
  LCD_string(0xC0,"   Exp06_6.c    ");
  Beep();

  while(1)
    { TFT_clear_screen();			// display screen 1
      TFT_landscape_mode();			// landscape mode

      TFT_string_slow(11, 2,White,Magenta," 국기에 대한 맹세 ");
      Delay_ms(1000);
      TFT_string_slow(17, 6,Magenta,Black,"(현행)");
      Delay_ms(1000);
      TFT_string_slow( 0, 9,Green,  Black,"  나는 자랑스러운 태극기 앞에 자유롭고 정의로운 대한민국의 무궁한 영광을 위하여 충성을 다할 것을 굳게 다짐합니다.");
      Delay_ms(1000);
      TFT_string_slow(15,18,Magenta,Black,"(옛 버전)");
      Delay_ms(1000);
      TFT_string_slow( 0,21,Yellow, Black,"  나는 자랑스런 태극기 앞에 조국과 민족의 무궁한 영광을 위하여 몸과 마음을 바쳐 충성을 다할 것을 굳게 다짐합니다.");
      Delay_ms(3000);

      TFT_clear_screen();			// display screen 2
      TFT_portrait_mode();			// portrait mode

      TFT_string_slow( 9, 1,White, Magenta," 진달래 꽃 ");
      Delay_ms(1000);
      TFT_string_slow(18, 5,Green, Black,"김소월");
      Delay_ms(1000);
      TFT_string_slow( 0, 9,Cyan,  Black,"나보기가 역겨워");
      Delay_ms(1000);
      TFT_string_slow( 0,11,Cyan,  Black,"가실 때에는");
      Delay_ms(1000);
      TFT_string_slow( 0,13,Cyan,  Black,"말없이 고이 보내드리오리다.");
      Delay_ms(1000);
      TFT_string_slow( 0,17,Yellow,Black,"영변에 약산");
      Delay_ms(1000);
      TFT_string_slow( 0,19,Yellow,Black,"진달래꽃");
      Delay_ms(1000);
      TFT_string_slow( 0,21,Yellow,Black,"아름따다 가실 길에 뿌리오리다.");
      Delay_ms(1000);
      TFT_string_slow( 0,25,Cyan,  Black,"가시는 걸음 걸음");
      Delay_ms(1000);
      TFT_string_slow( 0,27,Cyan,  Black,"놓인 그 꽃을");
      Delay_ms(1000);
      TFT_string_slow( 0,29,Cyan,  Black,"사뿐히 즈려밟고 가시옵소서.");
      Delay_ms(1000);
      TFT_string_slow( 0,33,Red,   Black,"나보기가 역겨워");
      Delay_ms(1000);
      TFT_string_slow( 0,35,Red,   Black,"가실 때에는");
      Delay_ms(1000);
      TFT_string_slow( 0,37,Red,   Black,"죽어도 아니 눈물 흘리오리다.");
      Delay_ms(3000);
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void TFT_string_slow(U08 xChar, U08 yChar, U16 colorfore, U16 colorback, char *str)	/* write TFT-LCD string */
{
  unsigned char ch1, ch2;
  unsigned short hangeul;

  Xcharacter = xChar;
  Ycharacter = yChar;

  foreground = colorfore;
  background = colorback;

  while(*str)
    { ch1 = *str;
      str++;

      if(ch1 < 0x80)				// English ASCII character
	TFT_English(ch1);
      else					// Korean
	{ ch2 = *str;
	  hangeul = (ch1 << 8) + ch2;
          str++;
	  hangeul = KS_code_conversion(hangeul);// convert 완성형 to 조합형
	  TFT_Korean(hangeul);
	}

      if(ch1 != ' ')
        { GPIOC->BSRR = 0x00008000;   		// buzzer on
	  Delay_ms(1);				// delay 1 ms
          GPIOC->BSRR = 0x80000000;		// buzzer off
	}

      Delay_ms(200);				// delay for slow display
   }
}
