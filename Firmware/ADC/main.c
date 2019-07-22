#include "stm32f4xx.h"
#include <stddef.h>

unsigned int My_ADC_Value;

void init_ADC(){ // PA3

    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 啟用 ADC3, DMA2, GPIO 的 RCC 時鐘 ****************************************/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

    /* DMA2流0通道2配置 **************************************/
    DMA_InitStructure.DMA_Channel = DMA_Channel_2;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)0x4001224C;
    DMA_InitStructure.DMA_Memory0BaseAddr =(unsigned int) &My_ADC_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);

    /* 配置 PA3 的 ADC 輸入 ******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADC 常見的初始化 **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC3 初始化****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC3, &ADC_InitStructure);

    /* ADC3定期通道3配置 *************************************/
    ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_3Cycles);

    /* 啟用DMA請求後，最後轉移（單ADC模式）*/
    ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

    /* 啟用ADC3 的 DMA */
    ADC_DMACmd(ADC3, ENABLE);

    /* 啟用 ADC3 */
    ADC_Cmd(ADC3, ENABLE);

    /* 開始軟體轉換 */
    ADC_SoftwareStartConv(ADC3);

}

void init_USART() // PB10, PB11
{
	// RCC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// GPIO
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

	// USART
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

void delay(uint32_t count)
{
	while(count--);
}

int main(void)
{
    init_ADC();
	init_USART();
    while (1)
    {
        if(My_ADC_Value > 150) // for sound-detect module
        {
        	send_data_int(My_ADC_Value);
        	delay(200000L);
        }
    }
}