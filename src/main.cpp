#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

#define TASK1_TASK_PRIO     4
#define TASK1_STK_SIZE      2*configMINIMAL_STACK_SIZE

static TaskHandle_t mainTask_Handler;

extern void
setup(void);
extern void
loop(void);

void main_loop(void*) {
	setup();

	for (;;) {
		loop();
	}

	return;
}

int main(void) {

	xTaskCreate((TaskFunction_t) main_loop, (const char*) "MainLoop",
			(uint16_t) TASK1_STK_SIZE, (void*) NULL,
			(UBaseType_t) TASK1_TASK_PRIO, (TaskHandle_t*) &mainTask_Handler);

	vTaskStartScheduler();

	while (1) {
	}
}

