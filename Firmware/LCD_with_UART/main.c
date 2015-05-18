#include "stm32f4xx.h" 
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
 //#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
//#include "tm_stm32f4_usart_dma.h"
#include "string.h"

int main(void) {

	char write, read, c;
	int i=0,j=0,k=0,row=1;
	SystemInit();
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
	TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
	TM_USART_Init(USART3, TM_USART_PinsPack_1, 9600); // TX= PB10 (White line); RX= PB11 (Green line)
	TM_USART_Puts(USART3, "Test the LCD with UART\n\n\r\r");
	TM_ILI9341_Puts(5, 10, "Test the LCD with UART", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);

	/* test ASCII for LCD's display */
	int a=72,a1,a2;  // Input a value 72 to show the screen to test int(資料型態)

    a1=a*0.1; // 從int轉換為轉char
    a2=a%10;

    a1=a1+48; // 從char轉換為ascii
    a2=a2+48;

    TM_ILI9341_Putc(265, 10, a1, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
    TM_ILI9341_Putc(275, 10, a2, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);

	while (1) {
		c = TM_USART_Getc(USART3);
		if (c) {
			TM_USART_Putc(USART3, c);
			TM_ILI9341_Putc(10+i, 50+j, c, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_MAGENTA);
			i=i+20;
			k++;
			if (k==14*row){
				j=j+20;
				i=0;
				row=row+1;
				if (row==9){
					TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
					TM_ILI9341_Puts(30, 10, "Test the LCD with UART", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
					i=0,j=0,k=0,row=1;
				}
			}
		}
	}
}
