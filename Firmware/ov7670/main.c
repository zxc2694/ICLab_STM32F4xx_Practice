/**
*	==========================================================================
*	main.c
*	(c) 2014-2015, Petr Machala
*
*	Description:
*	OptRec sensor system main file.
*	Optimized for 32F429IDISCOVERY board.
*
*   License:
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	==========================================================================
*/
 
#include "stm32f4xx.h"
#include "ov7670_control.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_ili9341.h"
#include "stm32f4xx_hal_sdram.h"
#include <stdio.h>
 
/* Main program loop */
int main(void){

	// System initialization
	SystemInit();
	FMC_SDRAM_Init();
	Camera_Init();
    TM_ILI9341_Init();
    
    /* Rotate LCD for 90 degrees */
    TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
    
    // OV7670 configuration
	TM_ILI9341_Fill(ILI9341_COLOR_WHITE);
    TM_ILI9341_Puts(65, 20, "Configuring camera", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_DrawRectangle(70, 150, 170, 150+18, ILI9341_COLOR_BLACK);
 
	if (OV7670_Config() != 0){
        TM_ILI9341_Puts(65, 130, "Failed", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
        TM_ILI9341_Puts(60, 150, "Push reset button", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
		while(1){
		}
	}
 
	// Infinite program loop
	while(1){
	  DCMI_CaptureCmd(ENABLE);
	}
}
 
/* DCMI DMA interrupt */
void DMA2_Stream1_IRQHandler(void){
    uint32_t n, i, buffer, *lcd_sdram1 = (uint32_t*) SDRAM_LCD_START1, *lcd_sdram2 = (uint32_t*) SDRAM_LCD_START2;
 
    // DMA complete
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) == SET){
        DMA_Cmd(DMA2_Stream1, DISABLE);
 
        //Wait for SPI to be ready
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI, SPI_I2S_FLAG_BSY));
 
		// Modify image to little endian
        i=0;
        for (n = 0; n < (IMG_HEIGHT*IMG_WIDTH)/2; n++) {
            buffer = lcd_sdram1[n];
 
            lcd_sdram2[n] = (((buffer & 0xff000000) >> 8) | ((buffer & 0x00ff0000) << 8) | ((buffer & 0x0000ff00) >> 8) | ((buffer & 0x000000ff) << 8));
        }
 
        // Prepare LCD for image
        LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_2);
        LCD_ILI9341_SetCursorPosition(0, 0, IMG_HEIGHT - 1, IMG_WIDTH - 1);
 
        LCD_ILI9341_SendCommand(ILI9341_GRAM);
 
        // SPI send
        ILI9341_WRX_SET;
        ILI9341_CS_RESET;
 
        SPI_DMA_init(lcd_sdram2);
        DMA_Cmd(DMA2_Stream4, ENABLE);
 
		// Clear IRQ flag
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
	}
}
 
/* LCD SPI interrupt */
void DMA2_Stream4_IRQHandler(void){
	static uint8_t n = 0;
	uint32_t *lcd_sdram2 = (uint32_t*) SDRAM_LCD_START2;
 
	// DMA_SPI complete
	if(DMA_GetITStatus(DMA2_Stream4,DMA_IT_TCIF4) == SET){
        DMA_Cmd(DMA2_Stream4, DISABLE);
 
        //Wait for SPI to be ready
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI, SPI_I2S_FLAG_BSY));
 
        switch(n){
            case 0:
                SPI_DMA_init(lcd_sdram2 + IMG_WIDTH*IMG_HEIGHT*BYTES_PER_PX/3/4);
                DMA_Cmd(DMA2_Stream4, ENABLE);
                n++;
                break;
            case 1:
                SPI_DMA_init(lcd_sdram2 + 2*IMG_WIDTH*IMG_HEIGHT*BYTES_PER_PX/3/4);
                DMA_Cmd(DMA2_Stream4, ENABLE);
                n++;
                break;
            case 2:
                ILI9341_CS_SET;
                DMA_Cmd(DMA2_Stream1, ENABLE);
                DCMI_CaptureCmd(ENABLE);
                n=0;
                break;
        }
 
        // Clear IRQ flag
        DMA_ClearITPendingBit(DMA2_Stream4,DMA_IT_TCIF4);
	}
}
