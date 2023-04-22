#ifndef PAGE_1_H
#define PAGE_1_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 *      INCLUDES
 **********************/

#include "lvgl/lvgl.h"

/**********************
 *       WIDGETS
 **********************/

typedef struct
{
	lv_obj_t *analog_clock_1;
	bool analog_clock_1_del;
	lv_obj_t *analog_clock_1_tabview_1;
	lv_obj_t *analog_clock_1_tabview_1_Uptime;
	lv_obj_t *analog_clock_1_analog_clock_1;
	lv_obj_t *analog_clock_1_tabview_1_Distance;
	lv_obj_t *analog_clock_1_chart_1;
    lv_chart_series_t * analog_clock_1_chart_1_0;
	lv_obj_t *analog_clock_1_tabview_1_IMU;
}lv_ui;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
extern lv_ui main_ui;
void setup_main_page_gui();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*PAGE_1_H*/