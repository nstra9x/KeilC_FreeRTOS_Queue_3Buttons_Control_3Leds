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

typedef struct
{
	char *ledName;
	unsigned int ledTask;
	QueueHandle_t ledQueue;
	unsigned int ledPin;
} ledStruct;

enum task{
	TASK1 = 0,
	TASK2,
	TASK3
};

char Error = 0;
Data_t data;
portBASE_TYPE xStatus;

//static void TaskLed(ledStruct *pvParameter);
static void TaskLed(void *pvParameter);
void vButton1Task(void *pvParameter);
void vButton2Task(void *pvParameter);
void vButton3Task(void *pvParameter);

QueueHandle_t xLed1Queue;
QueueHandle_t xLed2Queue;
QueueHandle_t xLed3Queue;

ledStruct Led1;
ledStruct Led2;
ledStruct Led3;

int main(void)
{
	LedInit();
	ButtonInit();
	UartInit();
	
	xLed1Queue = xQueueCreate(1, sizeof(Data_t));
	xLed2Queue = xQueueCreate(1, sizeof(Data_t));
	xLed3Queue = xQueueCreate(1, sizeof(Data_t));
	
	Led1.ledPin = GPIO_Pin_13;
	Led1.ledQueue = xLed1Queue;
	Led1.ledTask = TASK1;
	Led1.ledName = "led1";
	
	Led2.ledPin = GPIO_Pin_14;
	Led2.ledQueue = xLed2Queue;
	Led2.ledTask = TASK2;
	Led2.ledName = "led2";
	
	Led3.ledPin = GPIO_Pin_15;
	Led3.ledQueue = xLed3Queue;
	Led3.ledTask = TASK3;
	Led3.ledName = "led3";
	
//	xTaskCreate(TaskLed,(const char *) "Task LED 1", configMINIMAL_STACK_SIZE, NULL, 2, (xTaskHandle *) NULL);
	xTaskCreate(TaskLed,(const char *) "Task LED 1", configMINIMAL_STACK_SIZE, (void *) &Led1, 2, (xTaskHandle *) NULL);
	xTaskCreate(TaskLed,(const char *) "Task LED 2", configMINIMAL_STACK_SIZE, (void *) &Led2, 2, (xTaskHandle *) NULL);
	xTaskCreate(TaskLed,(const char *) "Task LED 3", configMINIMAL_STACK_SIZE, (void *) &Led3, 2, (xTaskHandle *) NULL);

	xTaskCreate(vButton1Task,(const char *) "Button 1", 200, NULL, 1, (xTaskHandle *) NULL);
	xTaskCreate(vButton2Task,(const char *) "Button 2", 200, NULL, 1, (xTaskHandle *) NULL);
	xTaskCreate(vButton3Task,(const char *) "Button 3", 200, NULL, 1, (xTaskHandle *) NULL);
	vTaskStartScheduler();
	for(;;);
}

static void TaskLed(void *Led)
{
	ledStruct *Led1;
	Led1 = (ledStruct *) Led;
	for(;;)
	{		
		xStatus = xQueueReceive(Led1->ledQueue , &data, 100);

		if(xStatus == pdPASS)
		{
			if(data.taskSource == Led1->ledTask)
			{
				GPIO_WriteBit(GPIOC, Led1->ledPin, data.buttonValue);
				UartSend(Led1->ledName);
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

