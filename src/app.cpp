/*
 * app.cpp
 *
 *  Created on: 23 мар. 2023 г.
 *      Author: user
 */
#include "board_props.hpp"

SingleButton<SingleButtonProps> UBUTTON;

typedef UartDbg<SampleUartDbgProps> uart;

void setup() {
//	HAL_Init();
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

	Wire1.begin(I2C1);

    uint8_t error, address;
    int nDevices;

	
    nDevices = 0;
	for(address = 8; address < 127; address++ ){
		Wire1.beginTransmission(address);
		error = Wire1.endTransmission();

		if (error == 0){
			printf("I2C device found at address 0x");
			if (address<16)
				printf("0");
			printf("%02X", address);
			printf(" !\r\n");

			nDevices++;
		}
		else if (error==4) {
			printf("Unknow error at address 0x");
			if (address<16)
				printf("0");
			printf("%02X\r\n", address);
		}
	}
};

void loop() {
	static uint32_t led1 = xTaskGetTickCount();
	static uint32_t led2 = xTaskGetTickCount();
	static uint32_t button_pull = xTaskGetTickCount();
	static uint32_t time_pull = xTaskGetTickCount();
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
		printf("Высота: %.2f (м)\r\n", distance);
		printf("Free heap size = %u\r\n", xPortGetFreeHeapSize());
		printf("\r\n");
		time_pull = xTaskGetTickCount();
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
