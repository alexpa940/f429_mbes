#include "wlvgl.h"
#include "lvgl/lvgl.h"

#define GUI_TASK_PRIO     4
#define GUI_STK_SIZE      1024
static TaskHandle_t guitask_handler;

void wlvgl::start()
{
    wlvgl::gui_semaphore = xSemaphoreCreateMutex();
    xTaskCreate((TaskFunction_t) wlvgl::task_cb, (const char*) "GUI",
        (uint16_t) GUI_STK_SIZE, (void*) NULL,
        (UBaseType_t) GUI_TASK_PRIO,
        (TaskHandle_t*) &guitask_handler);
}

void wlvgl::task_cb(void *)
{
    for(;;)
    {
        wlvgl::takeMutex();
        lv_task_handler();
        wlvgl::giveMutex();
        vTaskDelay( pdMS_TO_TICKS(30) );
    }

}
