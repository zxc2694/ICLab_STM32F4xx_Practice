#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>

#define Use_stm32f407_board 0
#define Use_stm32f429_board 1

void delay(uint32_t count)
{
	while(count--);
}

void init_GPIO()
{

#if  Use_stm32f407_board 	
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType =GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init(GPIOD, &GPIO_InitStruct);
#endif

#if  Use_stm32f429_board
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType =GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_Init(GPIOG, &GPIO_InitStruct);
#endif

}

int main()
{
	init_GPIO();

	int digitStatus = 1;

	while(1) {

#if  Use_stm32f407_board 
		GPIO_WriteBit(GPIOD, GPIO_Pin_12, digitStatus);
		delay(1000000L);
		GPIO_WriteBit(GPIOD, GPIO_Pin_13, digitStatus);	
		delay(1000000L);
		GPIO_WriteBit(GPIOD, GPIO_Pin_14, digitStatus);
		delay(1000000L);
		GPIO_WriteBit(GPIOD, GPIO_Pin_15, digitStatus);
		delay(1000000L);
#endif

#if  Use_stm32f429_board
		GPIO_WriteBit(GPIOG, GPIO_Pin_13, digitStatus);	
		delay(1000000L);
		GPIO_WriteBit(GPIOG, GPIO_Pin_14, digitStatus);
		delay(1000000L);
#endif		
		digitStatus = (digitStatus + 1) % 2;
	}

	return 0;
}
