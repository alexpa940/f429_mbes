/*
 * startup_stm32f429.h
 *
 *  Created on: 23 мар. 2023 г.
 *      Author: user
 */

#ifndef STARTUP_STM32F429_H_
#define STARTUP_STM32F429_H_

#include <stdint.h>

/********************************************************************************
 * Define
 ********************************************************************************/

#define WEAK                __attribute__ ((weak))
#define INLINE inline       __attribute__((always_inline))

/********************************************************************************
 * Section for linker script
 ********************************************************************************/

extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

extern char __StackTop;

typedef void (* pvf)();

/********************************************************************************
 * Function prototype
 ********************************************************************************/

extern int main (void);         // The entry point for the application

extern "C" void SetSysClock(void);
extern "C" void SystemCoreClockUpdate(void);
extern "C" void xPortSysTickHandler( void );
extern "C" void xPortPendSVHandler( void );
extern "C" void vPortSVCHandler( void );
extern "C" void __libc_init_array(void); //c++ call constructors

/********************************************************************************
 * Class
 ********************************************************************************/

class sNMI {
public:
    static void handler (void);
};

class sHardFault {
public:
    static void handler (void);
};

class sMemManage {
public:
    static void handler (void);
};

class sBusFault {
public:
    static void handler (void);
};

class sUsageFault {
public:
    static void handler (void);
};

class sSVC {
public:
    static void handler (void);
};

class sDebugMon {
public:
    static void handler (void);
};

class sPendSV {
public:
    static void handler (void);
};

class sSysTick {
public:
    static void handler (void);
};

class sWWDG {
public:
    static void handler (void);
};

class sPVD {
public:
    static void handler (void);
};

class sTampStamp {
public:
    static void handler (void);
};

class sRtc {
public:
    static void weakeUp (void);
    static void alarmHandler (void);
};

class sFlash {
public:
    static void handler (void);
};

class sRcc {
public:
    static void handler (void);
};

class sExti {
public:
    static void line0Handler (void);
    static void line1Handler (void);
    static void line2Handler (void);
    static void line3Handler (void);
    static void line4Handler (void);
    static void line5Handler (void);
    static void line10Handler (void);
};

class sDma {
public:
    static void channel1Handler (void);
    static void channel2Handler (void);
    static void channel3Handler (void);
    static void channel4Handler (void);
    static void channel5Handler (void);
    static void channel6Handler (void);
    static void channel7Handler (void);
};

class sAdc {
public:
    static void handler (void);
};

class sCan {
public:
    static void txHandler (void);
    static void rx0Handler (void);
    static void rx1Handler (void);
    static void sceHandler (void);
};

class sTim1 {
public:
    static void breakeTim9Handler (void);
    static void upTim10Handler (void);
    static void triggerTim11Handler (void);
    static void ccTim1Handler (void);
};

class sTim2 {
public:
    static void handler (void);
};

class sTim3 {
public:
    static void handler (void);
};

class sTim4 {
public:
    static void handler (void);
};

class sI2C1 {
public:
    static void eventHandler (void);
    static void errorHandler (void);
};

class sI2C2 {
public:
    static void eventHandler (void);
    static void errorHandler (void);
};

class sSpi1 {
public:
    static void handler (void);
};

class sSpi2 {
public:
    static void handler (void);
};

class sUsart1 {
public:
    static void handler (void);
};

class sUsart2 {
public:
    static void handler (void);
};

class sUsart3 {
public:
    static void handler (void);
};

#endif /* STARTUP_STM32F429_H_ */
