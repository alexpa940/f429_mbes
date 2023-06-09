/*
 * app.cpp
 *
 *  Created on: 23 мар. 2023 г.
 *      Author: user
 */
#include "board_props.hpp"

#include "hal_stm_lvgl/stm32f429i_discovery.h"

#include "lvgl/lvgl.h"
#include "lv_analogclock.h"

#include "hal_stm_lvgl/tft/tft.h"
#include "hal_stm_lvgl/touchpad/touchpad.h"

#include "wlvgl.h"

#include "main_page.h"

#include "imu.h"

SingleButton<SingleButtonProps> UBUTTON;

typedef UartDbg<SampleUartDbgProps> uart;
struct imu_data_s imu_data;

void setup() {
	HAL_Init();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	LD3::Mode(OUTPUT);
	LD4::Mode(OUTPUT);
	UBUTTON_PIN::Mode(INPUT);

	RtcModuleLSI();

	hc_sr04::start();

	uart();

	printf("Project compiled AT %s\r\n", compile_dt);

	lv_init();

	tft_init();
	touchpad_init();

	//lv_demo_widgets();
	imu::start();

	wlvgl::start();
	wlvgl::takeMutex();
	setup_main_page_gui();
	wlvgl::giveMutex();
};

extern "C" void compass_rotate(int16_t angle);
extern "C" void set_gpos(float r, float p, float y);

void loop() {
	static uint32_t led1 = xTaskGetTickCount();
	static uint32_t led2 = xTaskGetTickCount();
	static uint32_t button_pull = xTaskGetTickCount();
	static uint32_t time_pull = xTaskGetTickCount();
	static uint32_t distance_pull = xTaskGetTickCount();
	static uint32_t imu_pull = xTaskGetTickCount();
	static uint8_t led1_period = 1;
	static uint8_t led2_period = 1;
	static struct tm _time;

	if (xTaskGetTickCount() - button_pull > 10) {
		UBUTTON.pool();
		button_pull = xTaskGetTickCount();
	}

	if (xTaskGetTickCount() - led1 > led1_period) {
		LD3::Cpl();
		led1 = xTaskGetTickCount();
	}

	if (xTaskGetTickCount() - led2 > led2_period) {
		LD4::Cpl();
		led2 = xTaskGetTickCount();
	}
	if (UBUTTON.hasEvents()) {
		switch (UBUTTON.getEvent()) {
		case SingleButton<SingleButtonProps>::Event::Click:
			led1_period += 64;
			break;
		case SingleButton<SingleButtonProps>::Event::DoubleClick:
			led2_period += 64;
			break;
		default:
			break;
		}
	}

	if (xTaskGetTickCount() - time_pull > 1500) {
		uint32_t res = RtcModuleLSI::ReadTime();
		TimeUtil::localtime(res, &_time);
		float distance = hc_sr04::get_data_safe() / 100;
		printf("Время %02d:%02d:%02d (hh:mm:ss)\r\n", _time.tm_hour,
				_time.tm_min, _time.tm_sec);
		printf("Углы ориентации: %0.2f/%0.2f/%0.2f (град/град/град)\r\n", imu_data.yaw, imu_data.pitch, imu_data.roll);
		printf("Высота: %.2f (м)\r\n", distance);
		printf("Направление движения: %d (град)\r\n", imu_data.heading);
		//printf("Free heap size = %u\r\n", xPortGetFreeHeapSize());
		printf("\r\n");

		wlvgl::takeMutex();
		lv_analogclock_set_time(main_ui.analog_clock_1_analog_clock_1, _time.tm_hour, _time.tm_min, _time.tm_sec);
		wlvgl::giveMutex();
		time_pull = xTaskGetTickCount();
	}

	if (xTaskGetTickCount() - distance_pull > 300) {
		float distance = hc_sr04::get_data_safe() / 100;
		wlvgl::takeMutex();
		lv_chart_set_next_value(main_ui.analog_clock_1_chart_1, main_ui.analog_clock_1_chart_1_0, distance*100);
		wlvgl::giveMutex();
		distance_pull = xTaskGetTickCount();
	}

	if (xTaskGetTickCount() - imu_pull > 200) {
		imu::getDataSafe(&imu_data);
		wlvgl::takeMutex();
		compass_rotate(imu_data.heading);
		set_gpos(imu_data.roll, imu_data.pitch, imu_data.yaw);
		wlvgl::giveMutex();
		imu_pull = xTaskGetTickCount();
	}
}

extern "C" __attribute__((used)) int _write(int fd, char *buf, int size) {
	(void) fd;

	int i;
	vTaskSuspendAll();
	for (i = 0; i < size; i++) {
		uart::WriteData(*buf++);
		while ((uart::Status() & USART_FLAG_TC) != USART_FLAG_TC)
			;
	}
	xTaskResumeAll();
	return size;
}
