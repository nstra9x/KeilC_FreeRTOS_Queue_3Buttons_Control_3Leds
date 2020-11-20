#include "my_task.h"

void ButtonUpdate(buttonStruct *Button, uint16_t button_pin, QueueHandle_t ledQueue, unsigned int buttonTask)
{
	Button->button_pin = button_pin;
	Button->ledQueue = ledQueue;
	Button->buttonTask = buttonTask;
}

void LedUpdate(ledStruct *Led, char *ledName, unsigned int ledTask, QueueHandle_t ledQueue, unsigned int ledPin)
{
	Led->ledPin = ledPin;
	Led->ledQueue = ledQueue;
	Led->ledTask = ledTask;
	Led->ledName = ledName;
}

void LedTask(void *Led)
{
	ledStruct *sLed;
	sLed = (ledStruct *) Led;
	portBASE_TYPE xStatus;
	Data_t data;
	for(;;)
	{		
		xStatus = xQueueReceive(sLed->ledQueue , &data, 100);

		if(xStatus == pdPASS)
		{
			if(data.taskSource == sLed->ledTask)
			{
				GPIO_WriteBit(GPIOC, sLed->ledPin, data.buttonValue);
				UartSend(sLed->ledName);
			}
		}
	}
}

void ButtonTask(void *Button)
{
	buttonStruct *sButton;
	sButton = (buttonStruct *) Button;
	
	unsigned int curVal;
	portBASE_TYPE xStatus;
	Data_t data;
	curVal = BUTTON(sButton->button_pin);
	data.taskSource = sButton->buttonTask;
	data.buttonValue = curVal;
	xStatus = xQueueSend(sButton->ledQueue, &data, 100);
	
	for(;;)
	{
		if(CheckButton(sButton->button_pin))
		{
			curVal ^= 0x01;
			data.buttonValue = curVal;
			xStatus = xQueueSend(sButton->ledQueue, &data, 100);
		}
		taskYIELD();
	}
}