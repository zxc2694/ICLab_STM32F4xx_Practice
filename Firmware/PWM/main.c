#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>

void delay(uint32_t count)
{
        while(count--);
}

void init_RCC()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}

void init_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_Speed = GPIO_Speed_100MHz,
		.GPIO_OType =GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_NOPULL
	};

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM3);

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void init_TIM()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {
		.TIM_Period = 1680 - 1,
		.TIM_Prescaler = 500 - 1,
		.TIM_CounterMode = TIM_CounterMode_Up
	};
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	TIM_OCInitTypeDef TIM_OCInitStruct = {
		.TIM_OutputState = TIM_OutputState_Enable,
		.TIM_OCMode = TIM_OCMode_PWM1,
		.TIM_Pulse = 0
	};

	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
    TIM_OC2Init(TIM3, &TIM_OCInitStruct);
    TIM_OC3Init(TIM3, &TIM_OCInitStruct);

	TIM_Cmd(TIM3, ENABLE);
}

void init_USART()
{
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

#define PWM_INC 1
#define PWM_DEC 0

int main()
{
	init_RCC();
	init_GPIO();
	init_TIM();
	init_USART();
	
	int PWM_Status = PWM_INC, PWM_CCR = 0;

	while(1) {
		/* If the PWM status is set to be increased */
		if(PWM_Status == PWM_INC) {
			PWM_CCR += 10;
			/* Check if the PWM duty cycle achieve to 100% */
			if(PWM_CCR == 1680) PWM_Status = PWM_DEC;
		/* If the PWM status is set to be decreased */
		} else {
			PWM_CCR -= 10;
			/* Check if the PWM duty cycle achieve to 0% */
			if(PWM_CCR == 0) PWM_Status = PWM_INC;
		}

		TIM3->CCR1 = PWM_CCR;
		TIM3->CCR2 = PWM_CCR;
		TIM3->CCR3 = PWM_CCR;

		delay(10000L);
	}

	return 0;
}
