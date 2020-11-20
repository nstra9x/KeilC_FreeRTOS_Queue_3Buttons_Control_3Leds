#ifndef _INIT_CONFIG_H_
#define _INIT_CONFIG_H_

#include "stm32f10x.h"  
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stdio.h"


#define BUTTON1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1);
#define BUTTON2 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
#define BUTTON3 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);

int Button1(void);
int Button2(void);
int Button3(void);
void delay_ms(void);

void LedInit(void);
void ButtonInit(void);
void UartInit(void);
void UartSend(char buff[], uint8_t len);


#endif
