/*
 * startup_stm32f429.cpp
 *
 *  Created on: 23 мар. 2023 г.
 *      Author: user
 */

#include "startup_stm32f429.h"
#include "stm32f4xx.h"

/********************************************************************************
 * Default reset
 ********************************************************************************/

static INLINE void InitializeData() {

	uint32_t *pSrc;
	uint32_t *pDest;

	// Load initialized data from FLASH to RAM
	pSrc = &__etext;
	pDest = &__data_start__;

	while (pDest < &__data_end__)
		*pDest++ = *pSrc++;

	// Clear uninitialized data
	pDest = &__bss_start__;

	while (pDest < &__bss_end__)
		*pDest++ = 0;

#if __FPU_USED
	__asm(
			"  LDR.W R0, =0xE000ED88\n"
			"  LDR R1, [R0]\n"
			"  ORR R1, R1, #(0xF << 20)\n"
			"  STR R1, [R0]"
	);
#endif
}

extern "C" void ResetHandler() {
	// SetSysClock();
	SystemInit();
	InitializeData();
	SystemCoreClockUpdate();
	__libc_init_array();
	main();
}

extern "C" __attribute__((weak)) void _init(void) {
}

__attribute__ ((used,section(".isr_vector"))) void
(*const g_pfnVectors[])(void) =
{

	/************* Core exceptions ******************/

	pvf(&__StackTop),
	ResetHandler,
	sNMI::handler,
	sHardFault::handler,
	sMemManage::handler,
	sBusFault::handler,
	sUsageFault::handler,
	0,0,0,0,
	vPortSVCHandler, //sSVC::handler,
	sDebugMon::handler,
	0,
	xPortPendSVHandler,//sPendSV::handler,
	xPortSysTickHandler,//sSysTick::handler,

	//external interrupts
	sWWDG::handler,
	sPendSV::handler,
	sTampStamp::handler,
	sRtc::weakeUp,
	sFlash::handler,
	sRcc::handler,
	sExti::line0Handler,
	sExti::line1Handler,
	sExti::line2Handler,
	sExti::line3Handler,
	sExti::line4Handler,
	sDma::channel1Handler,
	sDma::channel2Handler,
	sDma::channel3Handler,
	sDma::channel4Handler,
	sDma::channel5Handler,
	sDma::channel6Handler,
	sDma::channel7Handler,
	sAdc::handler,
	sCan::txHandler,
	sCan::rx0Handler,
	sCan::rx1Handler,
	sCan::sceHandler,
	sExti::line5Handler,
	sTim1::breakeTim9Handler,
	sTim1::upTim10Handler,
	sTim1::triggerTim11Handler,
	sTim1::ccTim1Handler,
	sTim2::handler,
	sTim3::handler,
	sTim4::handler,
	sI2C1::eventHandler,
	sI2C1::errorHandler,
	sI2C2::eventHandler,
	sI2C2::errorHandler,
	sSpi1::handler,
	sSpi2::handler,
	sUsart1::handler,
	sUsart2::handler,
	sUsart3::handler,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,

	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,
	LTDC_IRQHandler
};

/********************************************************************************
 * Default interrupt
 ********************************************************************************/

WEAK void sNMI::handler() {
	while (1) {
	}
}
WEAK void sHardFault::handler() {
	while (1) {
	}
}
WEAK void sMemManage::handler() {
	while (1) {
	}
}
WEAK void sBusFault::handler() {
	while (1) {
	}
}
WEAK void sUsageFault::handler() {
	while (1) {
	}
}
WEAK void sSVC::handler() {
	while (1) {
	}
}
WEAK void sDebugMon::handler() {
	while (1) {
	}
}
WEAK void sPendSV::handler() {
	while (1) {
	}
}
WEAK void sSysTick::handler() {
	while (1) {
	}
}

WEAK void sWWDG::handler() {
	while (1) {
	}
}
WEAK void sPVD::handler() {
	while (1) {
	}
}
WEAK void sTampStamp::handler() {
	while (1) {
	}
}
WEAK void sRtc::weakeUp() {
	while (1) {
	}
}
WEAK void sFlash::handler() {
	while (1) {
	}
}
WEAK void sRcc::handler() {
	while (1) {
	}
}
WEAK void sExti::line0Handler() {
	while (1) {
	}
}
WEAK void sExti::line1Handler() {
	while (1) {
	}
}
WEAK void sExti::line2Handler() {
	while (1) {
	}
}
WEAK void sExti::line3Handler() {
	while (1) {
	}
}
WEAK void sExti::line4Handler() {
	while (1) {
	}
}
WEAK void sDma::channel1Handler() {
	while (1) {
	}
}
WEAK void sDma::channel2Handler() {
	while (1) {
	}
}
WEAK void sDma::channel3Handler() {
	while (1) {
	}
}
WEAK void sDma::channel4Handler() {
	while (1) {
	}
}
WEAK void sDma::channel5Handler() {
	while (1) {
	}
}
WEAK void sDma::channel6Handler() {
	while (1) {
	}
}
WEAK void sDma::channel7Handler() {
	while (1) {
	}
}
WEAK void sAdc::handler() {
	while (1) {
	}
}
WEAK void sCan::txHandler() {
	while (1) {
	}
}
WEAK void sCan::rx0Handler() {
	while (1) {
	}
}
WEAK void sCan::rx1Handler() {
	while (1) {
	}
}
WEAK void sCan::sceHandler() {
	while (1) {
	}
}
WEAK void sExti::line5Handler() {
	while (1) {
	}
}

WEAK void sExti::line10Handler() {
	while (1) {
	}
}


WEAK void sTim1::breakeTim9Handler() {
	while (1) {
	}
}
WEAK void sTim1::upTim10Handler() {
	while (1) {
	}
}
WEAK void sTim1::triggerTim11Handler() {
	while (1) {
	}
}
WEAK void sTim1::ccTim1Handler() {
	while (1) {
	}
}
WEAK void sTim2::handler() {
	while (1) {
	}
}
WEAK void sTim3::handler() {
	while (1) {
	}
}
WEAK void sTim4::handler() {
	while (1) {
	}
}
WEAK void sI2C1::eventHandler() {
	while (1) {
	}
}
WEAK void sI2C1::errorHandler() {
	while (1) {
	}
}
WEAK void sI2C2::eventHandler() {
	while (1) {
	}
}
WEAK void sI2C2::errorHandler() {
	while (1) {
	}
}

WEAK void sSpi1::handler() {
	while (1) {
	}
}
WEAK void sSpi2::handler() {
	while (1) {
	}
}
WEAK void sUsart1::handler() {
	while (1) {
	}
}
WEAK void sUsart2::handler() {
	while (1) {
	}
}
WEAK void sUsart3::handler() {
	while (1) {
	}
}

