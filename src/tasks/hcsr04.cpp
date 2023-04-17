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

#define RANGEFINDER_TASK_PRIO     4
#define RANGEFINDER_STK_SIZE      configMINIMAL_STACK_SIZE
static TaskHandle_t RangefinderTask_Handler;

struct RangefinderProps {
	using EchoPin = Pin<'A', 8>;
	using TrigPin = Pin<'G', 4, 'H'>;
	using RangefinderTim = STM32::TIM::Timer<STM32::TIM::TIM_1>;
	using EchoPinExti = STM32::ExtInt<EchoPin, STM32::Edge::Both>;
	static constexpr unsigned sourceClock { 160000000 }; //!< timer source clock
	static constexpr unsigned trigTime { 10 }; //!< trigger impulse len (milliseconds)
	static constexpr unsigned chNum { 1 };    //!< timer channel
};

Rangefinder<RangefinderProps> rangefinder;

void hc_sr04::start() {

	hc_sr04::interruptSemaphore = xSemaphoreCreateBinary();
	hc_sr04::dataSemaphore = xSemaphoreCreateMutex();
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
		if ( xSemaphoreTake( hc_sr04::interruptSemaphore, 200 ) == pdPASS) {
			first_measure = rangefinder.first_state_measure();

			if ( xSemaphoreTake( hc_sr04::interruptSemaphore, 200 ) == pdPASS) {
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

void sExti::line5Handler() {
	static BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	RangefinderProps::EchoPinExti::Interrupt::clear();
	xSemaphoreGiveFromISR(hc_sr04::interruptSemaphore,
			&xHigherPriorityTaskWoken);

//	RangefinderProps::RangefinderTim::Ch1::Interrupt::Clear();
	if (xHigherPriorityTaskWoken) {
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
