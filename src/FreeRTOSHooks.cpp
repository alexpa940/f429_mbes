/*
 * FreeRTOSHooks.cpp
 *
 *  Created on: 23 мар. 2023 г.
 *      Author: user
 */

#include "startup_stm32f429.h"
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

extern "C" {
void vApplicationIdleHook(void) {
	//resetWatchdog();
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
		StackType_t **ppxTimerTaskStackBuffer,
		uint32_t *pulTimerTaskStackSize) {

	static StaticTask_t xTimerTaskTCB;
	static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
	(void) xTask;
	(void) pcTaskName;

	// We dont have a good way to handle a stack overflow at this point in time
	NVIC_SystemReset();
}

void vApplicationMallocFailedHook(void) {
	NVIC_SystemReset();
}
}

