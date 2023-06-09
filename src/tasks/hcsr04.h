/*
 * hcsr04.h
 *
 *  Created on: 25 мар. 2023 г.
 *      Author: user
 */

#ifndef HCSR04_H_
#define HCSR04_H_

#include "rangefinder.h"
#include "filters.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct hc_sr04 {
public:
	static void start();
	static void task_cb(void*);
	static float get_data_safe();
	static inline SemaphoreHandle_t interruptSemaphore;
	static inline SemaphoreHandle_t dataSemaphore;
	static inline float last_distance = 0;
	static inline ExponentialFilter<float> filter;

};

#endif /* HCSR04_H_ */
