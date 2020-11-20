#ifndef _INIT_CONFIG_H_
#define _INIT_CONFIG_H_

#include "stm32f10x.h"  
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "FreeRTOS.h"
#include "queue.h"

#define LED1_PIN GPIO_Pin_13
#define LED2_PIN GPIO_Pin_14
#define LED3_PIN GPIO_Pin_15

#define BUTTON1_PIN GPIO_Pin_1
#define BUTTON2_PIN GPIO_Pin_2
#define BUTTON3_PIN GPIO_Pin_3

#define BUTTON(button_pin) GPIO_ReadInputDataBit(GPIOA, button_pin)

typedef struct
{
	char *ledName;
	unsigned int ledTask;
	QueueHandle_t ledQueue;
	unsigned int ledPin;
} ledStruct;

int CheckButton(uint16_t ButtonPin);
void LedInit(void);
void ButtonInit(void);
void UartInit(void);
void UartSend(char buff[]);
void delay_ms(void);

#endif
