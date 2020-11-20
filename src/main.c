//#include "init_config.h"
#include "my_task.h"

QueueHandle_t xLed1Queue;
QueueHandle_t xLed2Queue;
QueueHandle_t xLed3Queue;

ledStruct Led1;
ledStruct Led2;
ledStruct Led3;

buttonStruct Button1;
buttonStruct Button2;
buttonStruct Button3;

int main(void)
{
	LedInit();
	ButtonInit();
	UartInit();
	
	xLed1Queue = xQueueCreate(1, sizeof(Data_t));
	xLed2Queue = xQueueCreate(1, sizeof(Data_t));
	xLed3Queue = xQueueCreate(1, sizeof(Data_t));
	
	LedUpdate(&Led1, "led1", TASK1, xLed1Queue, LED1_PIN);
	LedUpdate(&Led2, "led2", TASK2, xLed2Queue, LED2_PIN);
	LedUpdate(&Led3, "led3", TASK3, xLed3Queue, LED3_PIN);
	
	ButtonUpdate(&Button1, BUTTON1_PIN, xLed1Queue, TASK1);
	ButtonUpdate(&Button2, BUTTON2_PIN, xLed2Queue, TASK2);
	ButtonUpdate(&Button3, BUTTON3_PIN, xLed3Queue, TASK3);
	
	xTaskCreate(LedTask,(const char *) "Task LED 1", configMINIMAL_STACK_SIZE, (void *) &Led1, 2, (xTaskHandle *) NULL);
	xTaskCreate(LedTask,(const char *) "Task LED 2", configMINIMAL_STACK_SIZE, (void *) &Led2, 2, (xTaskHandle *) NULL);
	xTaskCreate(LedTask,(const char *) "Task LED 3", configMINIMAL_STACK_SIZE, (void *) &Led3, 2, (xTaskHandle *) NULL);

	xTaskCreate(ButtonTask,(const char *) "Button 1", 200, (void *) &Button1, 1, (xTaskHandle *) NULL);
	xTaskCreate(ButtonTask,(const char *) "Button 2", 200, (void *) &Button2, 1, (xTaskHandle *) NULL);
	xTaskCreate(ButtonTask,(const char *) "Button 3", 200, (void *) &Button3, 1, (xTaskHandle *) NULL);
	vTaskStartScheduler();
	for(;;);
}
