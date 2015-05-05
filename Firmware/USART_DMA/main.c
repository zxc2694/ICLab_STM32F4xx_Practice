
#include "stm32f4xx.h"
/* Include my libraries here */
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_usart_dma.h"

#include "string.h"

void delay(uint32_t count)
{
	while(count--);
}

char USART_Buffer[100] = "\n\rHello via USART3 with TX DMA\n\r";

int main(void) {

	SystemInit();
	TM_USART_Init(USART3, TM_USART_PinsPack_1, 9600); 						// TX= PB10 (White line); RX= PB11 (Green line)
	TM_USART_DMA_Init(USART3); 												// Init TX DMA for USART2 
	TM_USART_Puts(USART3, "\n\rHello via USART3 without DMA\n\r"); 			// Say string without DMA  
	TM_USART_DMA_Send(USART3, (uint8_t *)USART_Buffer, strlen(USART_Buffer));//Send data with DMA
	
	/* You can do other stuff here instead of waiting for DMA to end */

	while (TM_USART_DMA_Sending(USART3));									//Wait till DMA works
	
	while (1) { 															//If any string arrived over USART
		if (TM_USART_Gets(USART3, USART_Buffer, sizeof(USART_Buffer))) {
		
			TM_USART_DMA_Send(USART3, (uint8_t *)USART_Buffer, strlen(USART_Buffer));//Send it back over DMA

			/* You can do other stuff here instead of waiting for DMA to end */
			
			while (TM_USART_DMA_Sending(USART3)); //Wait till DMA works
		}
	}
}

