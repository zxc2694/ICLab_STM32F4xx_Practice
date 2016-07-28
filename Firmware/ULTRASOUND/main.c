#include <stm32f4xx.h>

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

void read_ultrasound()
{
	unsigned int lenHigh = 0; //高位
	unsigned int lenLow = 0; //低位
	unsigned int d = 0; //距離
	
	Serial1.flush(); //清空串列接收緩衝區
	Serial1.write(0x55); //發送0x55，觸發SRF06 開始測距

	delay(1000000L);		

	if(Serial1.available() >= 2) //當串列接收緩衝區中資料大於2 位元組
	{
		lenHigh = Serial1.read(); //距離的高位元組
		lenLow = Serial1.read(); //距離的低位元組
		d =(lenHigh*256 + lenLow)*0.1; //計算距離值
		Serial.print(d);
		Serial.println("cm");
}
Control_PWM(d); //控制LED的亮度變化與蜂鳴器聲音長度變化
End(); //停止led亮,停止蜂鳴器叫
delay(500); // 等待500ms

}

int main()
{
	init_RCC();
	init_GPIO();
	init_USART();

	while(1) {
		send_data("Hello World!\n\r");
		delay(1000000L);		
	}

	return 0;
}
