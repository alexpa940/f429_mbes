#pragma once

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include "pin.h"
#include "button.h"
#include "hcsr04.h"

#include "rtc.h"

#include "stm32_uart_dbg.h"

#include "cstring"
#include "cstdio"

#include "Wire.h"

static const char *compile_dt = __DATE__ " " __TIME__;

using LD3 = Pin<'G', 13, 'H'>;
using LD4 = Pin<'G', 14, 'H'>;
using UBUTTON_PIN = Pin<'A', 0, 'H'>;
using Tim = STM32::TIM::Timer<STM32::TIM::TIM_1>;

struct SingleButtonProps {
	using ButtonPin = Pin<'A', 0, 'H'>;                      //!< pin
	static constexpr unsigned poolInterval { 10 }; //!< pool interval (milliseconds)
	static constexpr unsigned debounceMs { 30 }; //!< debounce time (milliseconds)
	static constexpr unsigned longPressMs { 900 };//!< long press time (milliseconds)
	static constexpr unsigned doubleClickMs { 500 };//!< double click time (milliseconds)
};


