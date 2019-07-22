#include <stm32f4xx.h>
#include <stddef.h>

void delay(uint32_t count)
{
	while(count--);
}


void init_RCC()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}

void init_GPIO()
{
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

void send_data(char *string)
{
	while(*string != '\0') {
		USART_SendData(USART3, (uint16_t)(*string));
		string++;
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}

void int2str(int n, char *str){
	 char buf[10] = "";
	 int i = 0;
	 int len = 0;
	 int temp = n < 0 ? -n: n;
	 if(str == NULL){
	     return;
	 }

	 while(temp){
	 buf[i++] = (temp % 10) + '0';
	 temp = temp / 10;
	 }
	 len = n < 0 ? ++i: i;
	 str[i] = 0;
	 while(1){
	     i--;
	     if(buf[len-i-1] ==0){
	         break;
	     }
	 str[i] = buf[len-i-1];
	 }
	 if (i == 0){
	     str[i] = '-';
	 }
}

void send_data_int(int data)
{
	char p[10];
	int2str(data, p);
	send_data(p);
	send_data("\n\r");
}

int main()
{
	init_RCC();
	init_GPIO();
	init_USART();

	while(1) {
		send_data("Hello World!\n\r");
		send_data_int(123);
		delay(1000000L);		
	}

	return 0;
}
