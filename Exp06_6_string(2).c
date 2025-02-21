/* ============================================================================ */
/*    Exp06_6_string(2).c : TFT-LCD�� �� ���ڿ��� õõ�� ����ϴ� ���α׷�	*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

void TFT_string_slow(U08 xChar, U08 yChar, U16 colorfore, U16 colorback, char *str);	/* write TFT-LCD string */

/* ----- ���� ���α׷� -------------------------------------------------------- */

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

      TFT_string_slow(11, 2,White,Magenta," ���⿡ ���� �ͼ� ");
      Delay_ms(1000);
      TFT_string_slow(17, 6,Magenta,Black,"(����)");
      Delay_ms(1000);
      TFT_string_slow( 0, 9,Green,  Black,"  ���� �ڶ������� �±ر� �տ� �����Ӱ� ���Ƿο� ���ѹα��� ������ ������ ���Ͽ� �漺�� ���� ���� ���� �����մϴ�.");
      Delay_ms(1000);
      TFT_string_slow(15,18,Magenta,Black,"(�� ����)");
      Delay_ms(1000);
      TFT_string_slow( 0,21,Yellow, Black,"  ���� �ڶ����� �±ر� �տ� ������ ������ ������ ������ ���Ͽ� ���� ������ ���� �漺�� ���� ���� ���� �����մϴ�.");
      Delay_ms(3000);

      TFT_clear_screen();			// display screen 2
      TFT_portrait_mode();			// portrait mode

      TFT_string_slow( 9, 1,White, Magenta," ���޷� �� ");
      Delay_ms(1000);
      TFT_string_slow(18, 5,Green, Black,"��ҿ�");
      Delay_ms(1000);
      TFT_string_slow( 0, 9,Cyan,  Black,"�����Ⱑ ���ܿ�");
      Delay_ms(1000);
      TFT_string_slow( 0,11,Cyan,  Black,"���� ������");
      Delay_ms(1000);
      TFT_string_slow( 0,13,Cyan,  Black,"������ ���� �����帮������.");
      Delay_ms(1000);
      TFT_string_slow( 0,17,Yellow,Black,"������ ���");
      Delay_ms(1000);
      TFT_string_slow( 0,19,Yellow,Black,"���޷���");
      Delay_ms(1000);
      TFT_string_slow( 0,21,Yellow,Black,"�Ƹ����� ���� �濡 �Ѹ�������.");
      Delay_ms(1000);
      TFT_string_slow( 0,25,Cyan,  Black,"���ô� ���� ����");
      Delay_ms(1000);
      TFT_string_slow( 0,27,Cyan,  Black,"���� �� ����");
      Delay_ms(1000);
      TFT_string_slow( 0,29,Cyan,  Black,"����� ������ ���ÿɼҼ�.");
      Delay_ms(1000);
      TFT_string_slow( 0,33,Red,   Black,"�����Ⱑ ���ܿ�");
      Delay_ms(1000);
      TFT_string_slow( 0,35,Red,   Black,"���� ������");
      Delay_ms(1000);
      TFT_string_slow( 0,37,Red,   Black,"�׾ �ƴ� ���� �긮������.");
      Delay_ms(3000);
    }
}

/* ----- ����� ���� �Լ� ----------------------------------------------------- */

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
	  hangeul = KS_code_conversion(hangeul);// convert �ϼ��� to ������
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
