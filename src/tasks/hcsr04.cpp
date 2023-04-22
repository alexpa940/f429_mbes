/*
 * hcsr04.cpp
 *
 *  Created on: 25 мар. 2023 г.
 *      Author: user
 */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "hcsr04.h"
#include "startup_stm32f429.h"

#define RANGEFINDER_TASK_PRIO     6
#define RANGEFINDER_STK_SIZE      configMINIMAL_STACK_SIZE
static TaskHandle_t RangefinderTask_Handler;

struct RangefinderProps {
	using EchoPin = Pin<'A', 5>;
	using TrigPin = Pin<'C', 3, 'H'>;
	using RangefinderTim = STM32::TIM::Timer<STM32::TIM::TIM_2>;
	using TimCh = STM32::TIM::Timer<STM32::TIM::TIM_2>::CCModule<1>;
	static constexpr unsigned sourceClock { 84000000 }; //!< timer source clock
	static constexpr unsigned trigTime { 20 }; //!< trigger impulse len (milliseconds)
	static constexpr unsigned chNum { 1 };    //!< timer channel
};

Rangefinder<RangefinderProps> rangefinder;

void hc_sr04::start() {

	hc_sr04::interruptSemaphore = xSemaphoreCreateBinary();
	hc_sr04::dataSemaphore = xSemaphoreCreateMutex();
	NVIC_SetPriority(TIM2_IRQn, 5);
	NVIC_EnableIRQ(TIM2_IRQn);

	RangefinderProps::TrigPin::Mode(OUTPUT);
	RangefinderProps::TrigPin::Off();
	RangefinderProps::EchoPin::Mode(ALT_INPUT_PULLDOWN);
	RangefinderProps::EchoPin::Alternate(ALT_FUNC_TIM2);

	rangefinder.init();

	xTaskCreate((TaskFunction_t) hc_sr04::task_cb, (const char*) "HC-SR04",
			(uint16_t) RANGEFINDER_STK_SIZE, (void*) NULL,
			(UBaseType_t) RANGEFINDER_TASK_PRIO,
			(TaskHandle_t*) &RangefinderTask_Handler);
}

float hc_sr04::get_data_safe() {
	float res = 0;
	if ( xSemaphoreTake( hc_sr04::dataSemaphore, portMAX_DELAY ) == pdPASS) {
		res = hc_sr04::last_distance;

		xSemaphoreGive(hc_sr04::dataSemaphore);
	}
	return res;
}

void hc_sr04::task_cb(void*) {
	static uint32_t first_measure = 0;
	static uint32_t second_measure = 0;
	while (1) {
		rangefinder.start_measure();
		if ( xSemaphoreTake( hc_sr04::interruptSemaphore, 2000 ) == pdPASS) {
			first_measure = rangefinder.first_state_measure();

			if ( xSemaphoreTake( hc_sr04::interruptSemaphore, 2000 ) == pdPASS) {
				second_measure = rangefinder.second_state_measure();

				if ( xSemaphoreTake(hc_sr04::dataSemaphore,
						portMAX_DELAY) == pdPASS) {
					filter.Put((second_measure - first_measure) * .034 / 2);
					hc_sr04::last_distance = filter.Get();

					xSemaphoreGive(hc_sr04::dataSemaphore);
				}
			} else {
				rangefinder.second_state_measure();
			}
		} else {
			rangefinder.second_state_measure();
		}

		vTaskDelay(250);
	}
}


void sTim2::handler()
{
	static BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	RangefinderProps::TimCh::Interrupt::Clear();
	xSemaphoreGiveFromISR(hc_sr04::interruptSemaphore,
			&xHigherPriorityTaskWoken);

	if (xHigherPriorityTaskWoken) {
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

