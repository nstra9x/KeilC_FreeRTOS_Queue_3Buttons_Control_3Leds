#include "init_config.h"

void LedInit(void)
{
	GPIO_InitTypeDef gpioInit ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &gpioInit);
}

void ButtonInit(void)
{
	GPIO_InitTypeDef gpioInit ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_IPU;
	gpioInit.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &gpioInit);
}

void UartInit(void)
{
	USART_InitTypeDef uartInit;
	GPIO_InitTypeDef gpioInit ;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
		/*========= Pin B10 is TX =========*/
	gpioInit.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_10;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInit);
	
	/*========= Pin B11 is RX =========*/
	gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInit.GPIO_Pin = GPIO_Pin_11;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInit);
	
	/*========= UART Configuration =========*/
	uartInit.USART_BaudRate = 9600;
	uartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	uartInit.USART_Parity = USART_Parity_No;
	uartInit.USART_StopBits = USART_StopBits_1;
	uartInit.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &uartInit);
	
	USART_Cmd(USART3, ENABLE);
}

void UartSend(char buff[])
{
	uint8_t index = 0;
	while(buff[index] != '\0')
	{
		USART_SendData(USART3, buff[index]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
		{}
		index++;
	}
	USART_SendData(USART3,13);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
	{}	
}

int Button1(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		delay_ms();
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
			{}
			return 1;
		}
	}
	return 0;
}

int Button2(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		delay_ms();
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
			{}
			return 1;
		}
	}
	return 0;
}

int Button3(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
	{
		delay_ms();
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
			{}
			return 1;
		}
	}
	return 0;
}

void delay_ms() 	
{
	uint32_t nTime = 0xffff;
	while(nTime--)
	{}
}
