
#include "stm32f4xx.h"
#include "ov7670_control.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_spi_dma.h"
#include "string.h"
#include <stdio.h>
 
/* Main program loop */
int main(void){

	SystemInit();
    TM_SDRAM_Init();
	Camera_Init();
    TM_ILI9341_Init();
    TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);

	TM_ILI9341_Fill(ILI9341_COLOR_WHITE);
    TM_ILI9341_Puts(65, 20, "Configuring camera", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_DrawRectangle(70, 150, 170, 150+18, ILI9341_COLOR_BLACK);
 
	if (OV7670_Config() != 0){
        TM_ILI9341_Puts(65, 130, "Failed", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
        TM_ILI9341_Puts(60, 150, "Push reset button", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
		while(1){
		}
	}
    TM_ILI9341_Puts(20, 50, "A", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	// Infinite program loop
	while(1){
        TM_ILI9341_Puts(30, 50, "B", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	    DCMI_CaptureCmd(ENABLE);
	}
}
/* SDRAM_LCD_START1 = (uint32_t)0x60020000; */
/* SDRAM_LCD_START2 = (uint32_t)0x00000000; */
/* DCMI DMA interrupt */
void DMA2_Stream1_IRQHandler(void){
    uint32_t n, i, buffer, *lcd_sdram1 = (uint32_t*) (uint32_t)0x60020000, *lcd_sdram2 = (uint32_t*) 0x00000000;
    TM_ILI9341_Puts(40, 50, "C", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
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
        TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
        TM_ILI9341_SetCursorPosition(0, 0, IMG_HEIGHT - 1, IMG_WIDTH - 1);
        TM_ILI9341_SendCommand(0x2C); // ILI9341_GRAM = 0x2C
 
        // SPI send
        GPIO_SetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN); // ILI9341_WRX_SET;      
        GPIO_ResetBits(ILI9341_CS_PORT, ILI9341_CS_PIN); // ILI9341_CS_RESET;
//     TM_SPI_DMA_Transmit(SPI5, NULL, lcd_sdram2, strlen(lcd_sdram2));
//      TM_SPI_DMA_Init(lcd_sdram2);
//      SPI_DMA_init(lcd_sdram2);
        DMA_Cmd(DMA2_Stream4, ENABLE);
 
		// Clear IRQ flag
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
        TM_ILI9341_Puts(50, 50, "D", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	}
}
 
/* LCD SPI interrupt */
void DMA2_Stream4_IRQHandler(void){
	static uint8_t n = 0;
	uint32_t *lcd_sdram2 = (uint32_t*)0x00000000;
 
	// DMA_SPI complete
	if(DMA_GetITStatus(DMA2_Stream4,DMA_IT_TCIF4) == SET){
        DMA_Cmd(DMA2_Stream4, DISABLE);
 
        //Wait for SPI to be ready
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI, SPI_I2S_FLAG_BSY));
 
        switch(n){
            case 0:
//              TM_SPI_DMA_Transmit(SPI5, NULL, lcd_sdram2 + IMG_WIDTH*IMG_HEIGHT*BYTES_PER_PX/3/4, strlen(lcd_sdram2));
//              TM_SPI_DMA_Init(lcd_sdram2 + IMG_WIDTH*IMG_HEIGHT*BYTES_PER_PX/3/4);;
//              SPI_DMA_init(lcd_sdram2 + IMG_WIDTH*IMG_HEIGHT*BYTES_PER_PX/3/4);
                DMA_Cmd(DMA2_Stream4, ENABLE);
                n++;
                break;
            case 1:
                TM_SPI_DMA_Init(SPI5);
 //             TM_SPI_DMA_Transmit(SPI5, NULL, lcd_sdram2 + IMG_WIDTH*IMG_HEIGHT*BYTES_PER_PX/3/4, strlen(lcd_sdram2));
 //             TM_SPI_DMA_Init(lcd_sdram2 + 2*IMG_WIDTH*IMG_HEIGHT*BYTES_PER_PX/3/4);
 //             SPI_DMA_init(lcd_sdram2 + 2*IMG_WIDTH*IMG_HEIGHT*BYTES_PER_PX/3/4);
                DMA_Cmd(DMA2_Stream4, ENABLE);
                n++;
                break;
            case 2:
                GPIO_SetBits(ILI9341_CS_PORT, ILI9341_CS_PIN); // ILI9341_CS_SET;
                DMA_Cmd(DMA2_Stream1, ENABLE);
                DCMI_Cmd(ENABLE);
                DCMI_CaptureCmd(ENABLE);
                n=0;
                break;
        }
 
        // Clear IRQ flag
        DMA_ClearITPendingBit(DMA2_Stream4,DMA_IT_TCIF4);
	}
}
