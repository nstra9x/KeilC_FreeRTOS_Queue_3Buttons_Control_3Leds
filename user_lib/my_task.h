#ifndef _BUTTON_TASK_H_
#define _BUTTON_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "init_config.h"

typedef struct
{
	uint16_t button_pin;
	unsigned int buttonTask;
	QueueHandle_t ledQueue;
} buttonStruct;

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

extern QueueHandle_t xLed1Queue;
extern QueueHandle_t xLed2Queue;
extern QueueHandle_t xLed3Queue;

void LedUpdate(ledStruct *Led, char *ledName, unsigned int ledTask, QueueHandle_t ledQueue, unsigned int ledPin);
void ButtonUpdate(buttonStruct *Button, uint16_t button_pin, QueueHandle_t ledQueue, unsigned int buttonTask);

void LedTask(void *pvParameter);
void ButtonTask(void *pvParameter);

#endif
