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

static void TaskLed(void *pvParameter);

QueueHandle_t xLedQueue;

int main(void)
{
	LedInit();
	ButtonInit();
	UartInit();
	xLedQueue = xQueueCreate(3, sizeof(Data_t));
	
	xTaskCreate(TaskLed,(const char *) "Task LED 1", configMINIMAL_STACK_SIZE, NULL, 2, (xTaskHandle *) NULL);
	xTaskCreate(vButton1Task,(const char *) "Button 1", configMINIMAL_STACK_SIZE, NULL, 1, (xTaskHandle *) NULL);
	xTaskCreate(vButton2Task,(const char *) "Button 2", configMINIMAL_STACK_SIZE, NULL, 1, (xTaskHandle *) NULL);
	xTaskCreate(vButton3Task,(const char *) "Button 3", configMINIMAL_STACK_SIZE, NULL, 1, (xTaskHandle *) NULL);
	vTaskStartScheduler();
	for(;;);
}

static void TaskLed(void *pvParameter)
{
	Data_t data;
	portBASE_TYPE xStatus;
	for(;;)
	{		
		xStatus = xQueueReceive(xLedQueue, &data, 100);

		if(xStatus == pdPASS)
		{
			if(data.taskSource == TASK1)
			{
				GPIO_WriteBit(GPIOC, GPIO_Pin_13, data.buttonValue);
				UartSend("led1", 4);
			}
			
			else if (data.taskSource == TASK2)
			{
				GPIO_WriteBit(GPIOC, GPIO_Pin_14, data.buttonValue);
				UartSend("led2", 4);
			}
			
			else
			{
				GPIO_WriteBit(GPIOC, GPIO_Pin_15, data.buttonValue);
				UartSend("led3", 4);
			}
		}
	}
}

static void vButton1Task(void *pvParameter)
{
	portBASE_TYPE xStatus;
	unsigned int curVal;
	Data_t data;
	curVal = BUTTON1;
	data.taskSource = TASK1;
	data.buttonValue = curVal;
	xStatus = xQueueSend(xLedQueue, &data, 100);
	
	for(;;)
	{
		if(Button1())
		{
			curVal ^= 0x01;
			data.buttonValue = curVal;
			xStatus = xQueueSend(xLedQueue, &data, 100);
		}
		taskYIELD();
	}
}

static void vButton2Task(void *pvParameter)
{
	portBASE_TYPE xStatus;
	unsigned int preVal, curVal;
	Data_t data;
	data.taskSource = TASK2;
	preVal = BUTTON2;
	data.buttonValue = preVal;
	xStatus = xQueueSend(xLedQueue, &data, 100);
	
	for(;;)
	{
		curVal = BUTTON2;
		if(curVal != preVal)
		{
			data.buttonValue = curVal;
			preVal = curVal;
			xStatus = xQueueSend(xLedQueue, &data, 100);
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
	portBASE_TYPE xStatus;
	unsigned int preVal, curVal;
	Data_t data;
	data.taskSource = TASK3;
	preVal = BUTTON3;
	data.buttonValue = preVal;
	xStatus = xQueueSend(xLedQueue, &data, 100);
	
	for(;;)
	{
		curVal = BUTTON3;
		if(curVal != preVal)
		{
			data.buttonValue = curVal;
			preVal = curVal;
			xStatus = xQueueSend(xLedQueue, &data, 100);
			if(xStatus != pdPASS)
			{
				Error = 1;
			}
		}
		taskYIELD();
	}
}

