#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct wlvgl
{
    static void start();
	static void task_cb(void*);
    static inline SemaphoreHandle_t gui_semaphore;

    static void takeMutex()
    {
        while( xSemaphoreTake( gui_semaphore, portMAX_DELAY ) != pdTRUE )
            ;
    }

    static void giveMutex ()
    {
        xSemaphoreGive( gui_semaphore );
    }
};

