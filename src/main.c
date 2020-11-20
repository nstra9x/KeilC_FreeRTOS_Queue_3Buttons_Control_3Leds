#include "stm32f10x.h"     // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "init_config.h"

typedef struct
{
	unsigned int buttonValue;
	unsigned int taskSource;
} Data_t;

enum task{
	TASK1 = 0,
	TASK2,
	TASK3
};

char Error = 0;
Data_t data;
portBASE_TYPE xStatus;

static void Task1Led(void *pvParameter);
static void Task2Led(void *pvParameter);
static void Task3Led(void *pvParameter);
void vButton1Task(void *pvParameter);
void vButton2Task(void *pvParameter);
void vButton3Task(void *pvParameter);

QueueHandle_t xLed1Queue;
QueueHandle_t xLed2Queue;
QueueHandle_t xLed3Queue;

int main(void)
{
	LedInit();
	ButtonInit();
	UartInit();
	xLed1Queue = xQueueCreate(1, sizeof(Data_t));
	xLed2Queue = xQueueCreate(1, sizeof(Data_t));
	xLed3Queue = xQueueCreate(1, sizeof(Data_t));
	
	xTaskCreate(Task1Led,(const char *) "Task LED 1", configMINIMAL_STACK_SIZE, NULL, 2, (xTaskHandle *) NULL);
	xTaskCreate(Task2Led,(const char *) "Task LED 2", configMINIMAL_STACK_SIZE, NULL, 2, (xTaskHandle *) NULL);
	xTaskCreate(Task3Led,(const char *) "Task LED 3", configMINIMAL_STACK_SIZE, NULL, 2, (xTaskHandle *) NULL);
	xTaskCreate(vButton1Task,(const char *) "Button 1", configMINIMAL_STACK_SIZE, NULL, 1, (xTaskHandle *) NULL);
	xTaskCreate(vButton2Task,(const char *) "Button 2", configMINIMAL_STACK_SIZE, NULL, 1, (xTaskHandle *) NULL);
	xTaskCreate(vButton3Task,(const char *) "Button 3", configMINIMAL_STACK_SIZE, NULL, 1, (xTaskHandle *) NULL);
	vTaskStartScheduler();
	for(;;);
}

static void Task1Led(void *pvParameter)
{
	for(;;)
	{		
		xStatus = xQueueReceive(xLed1Queue, &data, 100);

		if(xStatus == pdPASS)
		{
			if(data.taskSource == TASK1)
			{
				GPIO_WriteBit(GPIOC, GPIO_Pin_13, data.buttonValue);
				UartSend("led1", 4);
			}
		}
	}
}

static void Task2Led(void *pvParameter)
{
	for(;;)
	{		
		xStatus = xQueueReceive(xLed2Queue, &data, 100);

		if(xStatus == pdPASS)
		{
			if(data.taskSource == TASK2)
			{
				GPIO_WriteBit(GPIOC, GPIO_Pin_14, data.buttonValue);
				UartSend("led2", 4);
			}
		}
	}
}

static void Task3Led(void *pvParameter)
{
	for(;;)
	{		
		xStatus = xQueueReceive(xLed3Queue, &data, 100);

		if(xStatus == pdPASS)
		{
			if(data.taskSource == TASK3)
			{
				GPIO_WriteBit(GPIOC, GPIO_Pin_15, data.buttonValue);
				UartSend("led3", 4);
			}
		}
	}
}

static void vButton1Task(void *pvParameter)
{
	unsigned int curVal;
	Data_t data;
	curVal = BUTTON1;
	data.taskSource = TASK1;
	data.buttonValue = curVal;
	xStatus = xQueueSend(xLed1Queue, &data, 100);
	
	for(;;)
	{
		if(Button1())
		{
			curVal ^= 0x01;
			data.buttonValue = curVal;
			xStatus = xQueueSend(xLed1Queue, &data, 100);
		}
		taskYIELD();
	}
}

static void vButton2Task(void *pvParameter)
{
	unsigned int preVal, curVal;
	Data_t data;
	data.taskSource = TASK2;
	preVal = BUTTON2;
	data.buttonValue = preVal;
	xStatus = xQueueSend(xLed2Queue, &data, 100);
	
	for(;;)
	{
		curVal = BUTTON2;
		if(curVal != preVal)
		{
			data.buttonValue = curVal;
			preVal = curVal;
			xStatus = xQueueSend(xLed2Queue, &data, 100);
			if(xStatus != pdPASS)
			{
				Error = 1;
			}
		}
		taskYIELD();
	}
}

static void vButton3Task(void *pvParameter)
{
	unsigned int preVal, curVal;
	Data_t data;
	data.taskSource = TASK3;
	preVal = BUTTON3;
	data.buttonValue = preVal;
	xStatus = xQueueSend(xLed3Queue, &data, 100);
	
	for(;;)
	{
		curVal = BUTTON3;
		if(curVal != preVal)
		{
			data.buttonValue = curVal;
			preVal = curVal;
			xStatus = xQueueSend(xLed3Queue, &data, 100);
			if(xStatus != pdPASS)
			{
				Error = 1;
			}
		}
		taskYIELD();
	}
}

