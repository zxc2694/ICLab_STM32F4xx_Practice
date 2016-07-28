#include "stm32f4xx.h"

#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_delay.h"
#include "string.h"

void init_USART()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP
	};
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStruct = {
		.USART_BaudRate = 9600,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No
	};
	USART_Init(USART3, &USART_InitStruct);
	USART_Cmd(USART3, ENABLE);
}

void send_data(char *string)
{
	while(*string != '\0') {
		USART_SendData(USART3, (uint16_t)(*string));
		string++;
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}

int main(void) 
{
	char write, read;
	uint8_t i;

	SystemInit();
	init_USART();
	TM_DELAY_Init(); //Initialize delay
	TM_DISCO_LedInit(); //Initialize leds

	if (TM_SDRAM_Init()) { //Initialize SDRAM
		TM_DISCO_LedOn(LED_GREEN);
	} 
	else {
		TM_DISCO_LedOn(LED_RED);
	}
	Delayms(20);
	
	write = 5;

	TM_SDRAM_Write16(0x3214, write); // Write 16bit value to SDRAM at location 0x3214
	
	read = TM_SDRAM_Read16(0x3214); // Read from location 0x3214 
	
	
	// if (write == read) { /* Check if read data is the same as written data */
	// 	TM_DISCO_LedToggle(LED_GREEN | LED_RED);
	// 	Delayms(50);
	// 	TM_DISCO_LedToggle(LED_GREEN | LED_RED);
	// 	Delayms(50);
	// 	TM_DISCO_LedToggle(LED_GREEN | LED_RED);
	// 	Delayms(50);
	// 	TM_DISCO_LedToggle(LED_GREEN | LED_RED);
	// 	Delayms(50);
	// }	
	while(1) {
		send_data(read);
		send_data("\n\r");	
	}
}
