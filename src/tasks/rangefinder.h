/*
 * rangefinder.h
 *
 *  Created on: 25 мар. 2023 г.
 *      Author: user
 */

#ifndef RANGEFINDER_H_
#define RANGEFINDER_H_

#include "pin.h"
#include "stm32_tim.h"
#include "stm32_extint.h"
#include "FreeRTOS.h"
#include "task.h"

struct DefaultRangefinderProps {
	using EchoPin = Pin<'E', 9>;
	using TrigPin = Pin<'A', 1>;
	using RangefinderTim = STM32::TIM::Timer<STM32::TIM::TIM_1>;
	using TimCh = STM32::TIM::Timer<STM32::TIM::TIM_1>::CCModule<1>;
	static constexpr unsigned sourceClock { 10 };    //!< timer source clock
	static constexpr unsigned trigTime { 10 }; //!< trigger impulse len (milliseconds)
	static constexpr unsigned chNum { 1 };    //!< timer channel
};

template<typename Props>
class Rangefinder {
public:
	Rangefinder() = default;

	void init() {

		Tim::EnableClocks();
		//base timer init
		Tim::TIMx->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
		Tim::TIMx->CR1 |= TIM_COUNTERMODE_UP;

		Tim::TIMx->CR1 &= ~TIM_CR1_CKD;
		Tim::TIMx->CR1 |= TIM_CLOCKDIVISION_DIV1;
		Tim::TIMx->CR1 |= TIM_AUTORELOAD_PRELOAD_DISABLE;

		Tim::TIMx->PSC = (sourceClock / 1000000) - 1;
		Tim::TIMx->ARR = UINT16_MAX - 1;

		Tim::TIMx->RCR = 0;
		Tim::TIMx->EGR = TIM_EGR_UG;

		//master config
		Tim::TIMx->CR2 &= ~TIM_CR2_MMS;
		Tim::TIMx->CR2 |= TIM_TRGO_RESET;

		TimCh::CCMRx &= ~(TimCh::CCMR_CCS);
		TimCh::CCMRx |= TimCh::CCMR_CCS_IC1;
		Tim::TIMx->CCER &= ~(TimCh::CCER_CCxNP | TimCh::CCER_CCxP);
		Tim::TIMx->CCER |= TimCh::CCER_CCxP;

		TimCh::Interrupt::Enable();
		TimCh::Enable();
		
		Tim::Enable();
	}

	void start_measure() {
		TrigPin::On();
		vTaskDelay(pdMS_TO_TICKS(trigTime));
		TrigPin::Off();
		Tim::TIMx->CNT = 0;
		Tim::TIMx->CCER &= ~(TimCh::CCER_CCxNP | TimCh::CCER_CCxP);
		Tim::TIMx->CCER |= TimCh::CCER_CCxNP;
	}

	uint32_t first_state_measure() {

		uint32_t result = 0;
		result = Tim::TIMx->CNT;
		Tim::TIMx->CCER &= ~(TimCh::CCER_CCxNP | TimCh::CCER_CCxP);
		Tim::TIMx->CCER |= TimCh::CCER_CCxP;
		return result;
	}

	uint32_t second_state_measure() {
		uint32_t result = 0;
		result = Tim::TIMx->CNT;
		return result;
	}

public:
	using EchoPin = typename Props::EchoPin;
	using TrigPin = typename Props::TrigPin;
	using Tim = typename Props::RangefinderTim;
	using TimCh = typename Props::TimCh;
	static constexpr auto trigTime { Props::trigTime };
	static constexpr auto sourceClock { Props::sourceClock };
};

#endif /* RANGEFINDER_H_ */
