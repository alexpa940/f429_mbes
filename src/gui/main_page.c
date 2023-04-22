#include "main_page.h"

/**********************
 *       WIDGETS
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

lv_ui main_ui;

void ui_init_style(lv_style_t * style)
{
	if (style->prop_cnt > 1)
		lv_style_reset(style);
	else
		lv_style_init(style);
}

__attribute__((unused)) static void ta_screen_event_cb(lv_event_t *e)
{

	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = lv_event_get_user_data(e);
	if (code == LV_EVENT_FOCUSED || code == LV_EVENT_CLICKED)
	{
		lv_keyboard_set_textarea(kb, ta);
		lv_obj_move_foreground(kb);
		lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
	if (code == LV_EVENT_CANCEL || code == LV_EVENT_DEFOCUSED)
	{
		lv_keyboard_set_textarea(kb, NULL);
		lv_obj_move_background(kb);
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}

void setup_scr_screen(lv_ui *ui){


	//Write codes analog_clock_1
	ui->analog_clock_1 = lv_obj_create(NULL);

	//Create keyboard on analog_clock_1
	lv_obj_set_scrollbar_mode(ui->analog_clock_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for analog_clock_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->analog_clock_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->analog_clock_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->analog_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->analog_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->analog_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->analog_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->analog_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for analog_clock_1. Part: LV_PART_TICKS, State: LV_STATE_DEFAULT
	lv_obj_set_style_text_color(ui->analog_clock_1, lv_color_make(0xff, 0x00, 0x00), LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes analog_clock_1_tabview_1
	ui->analog_clock_1_tabview_1 = lv_tabview_create(ui->analog_clock_1, LV_DIR_TOP, 50);
	lv_obj_set_scrollbar_mode(ui->analog_clock_1_tabview_1, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_pos(ui->analog_clock_1_tabview_1, 0, 0);
	lv_obj_set_size(ui->analog_clock_1_tabview_1, 240, 319);

	//Uptime
	ui->analog_clock_1_tabview_1_Uptime = lv_tabview_add_tab(ui->analog_clock_1_tabview_1,"Uptime");

	//Distance
	ui->analog_clock_1_tabview_1_Distance = lv_tabview_add_tab(ui->analog_clock_1_tabview_1,"Distance");

	//IMU
	ui->analog_clock_1_tabview_1_IMU = lv_tabview_add_tab(ui->analog_clock_1_tabview_1,"IMU");
	lv_obj_t * analog_clock_1_tabview_1_IMU_label = lv_label_create(ui->analog_clock_1_tabview_1_IMU);
	lv_label_set_text(analog_clock_1_tabview_1_IMU_label, "con3");


	//Set style for analog_clock_1_tabview_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->analog_clock_1_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->analog_clock_1_tabview_1, lv_color_make(0x2F, 0xDA, 0xAE), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->analog_clock_1_tabview_1, lv_color_make(0xea, 0xef, 0xf3), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->analog_clock_1_tabview_1, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->analog_clock_1_tabview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->analog_clock_1_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->analog_clock_1_tabview_1, lv_color_make(0x2F, 0xDA, 0xAE), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->analog_clock_1_tabview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->analog_clock_1_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->analog_clock_1_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->analog_clock_1_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->analog_clock_1_tabview_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->analog_clock_1_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->analog_clock_1_tabview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->analog_clock_1_tabview_1, lv_color_make(0x4d, 0x4d, 0x4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->analog_clock_1_tabview_1, &lv_font_montserrat_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->analog_clock_1_tabview_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->analog_clock_1_tabview_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_analog_clock_1_tabview_1_extra_btnm_main_default
	static lv_style_t style_analog_clock_1_tabview_1_extra_btnm_main_default;
	ui_init_style(&style_analog_clock_1_tabview_1_extra_btnm_main_default);
	lv_style_set_radius(&style_analog_clock_1_tabview_1_extra_btnm_main_default, 0);
	lv_style_set_bg_color(&style_analog_clock_1_tabview_1_extra_btnm_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_analog_clock_1_tabview_1_extra_btnm_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_analog_clock_1_tabview_1_extra_btnm_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_analog_clock_1_tabview_1_extra_btnm_main_default, 255);
	lv_style_set_border_color(&style_analog_clock_1_tabview_1_extra_btnm_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_analog_clock_1_tabview_1_extra_btnm_main_default, 0);
	lv_style_set_border_opa(&style_analog_clock_1_tabview_1_extra_btnm_main_default, 100);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->analog_clock_1_tabview_1), &style_analog_clock_1_tabview_1_extra_btnm_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_analog_clock_1_tabview_1_extra_btnm_items_default
	static lv_style_t style_analog_clock_1_tabview_1_extra_btnm_items_default;
	ui_init_style(&style_analog_clock_1_tabview_1_extra_btnm_items_default);
	lv_style_set_text_color(&style_analog_clock_1_tabview_1_extra_btnm_items_default, lv_color_make(0x4d, 0x4d, 0x4d));
	lv_style_set_text_font(&style_analog_clock_1_tabview_1_extra_btnm_items_default, &lv_font_montserrat_12);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->analog_clock_1_tabview_1), &style_analog_clock_1_tabview_1_extra_btnm_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_analog_clock_1_tabview_1_extra_btnm_items_checked
	static lv_style_t style_analog_clock_1_tabview_1_extra_btnm_items_checked;
	ui_init_style(&style_analog_clock_1_tabview_1_extra_btnm_items_checked);
	lv_style_set_radius(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, 0);
	lv_style_set_bg_color(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, 60);
	lv_style_set_border_color(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, 4);
	lv_style_set_border_opa(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, 255);
	lv_style_set_border_side(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, LV_BORDER_SIDE_BOTTOM);
	lv_style_set_text_color(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_text_font(&style_analog_clock_1_tabview_1_extra_btnm_items_checked, &lv_font_montserrat_12);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->analog_clock_1_tabview_1), &style_analog_clock_1_tabview_1_extra_btnm_items_checked, LV_PART_ITEMS|LV_STATE_CHECKED);

	//Write codes analog_clock_1_analog_clock_1
	ui->analog_clock_1_analog_clock_1 = lv_analogclock_create(ui->analog_clock_1_tabview_1_Uptime);
	lv_obj_set_pos(ui->analog_clock_1_analog_clock_1, 6, 11);
	lv_obj_set_size(ui->analog_clock_1_analog_clock_1, 200, 200);
	lv_analogclock_hide_digits(ui->analog_clock_1_analog_clock_1, false);
	lv_analogclock_set_major_ticks(ui->analog_clock_1_analog_clock_1, 2, 10, lv_color_make(0x55, 0x55, 0x55), 10);
	lv_analogclock_set_ticks(ui->analog_clock_1_analog_clock_1, 2, 5, lv_color_make(0x33, 0x33, 0x33));
	lv_analogclock_set_hour_needle_line(ui->analog_clock_1_analog_clock_1, 2, lv_color_make(0x00, 0xff, 0x00), -40);
	lv_analogclock_set_min_needle_line(ui->analog_clock_1_analog_clock_1, 2, lv_color_make(0xE1, 0xFF, 0x00), -30);
	lv_analogclock_set_sec_needle_line(ui->analog_clock_1_analog_clock_1, 2, lv_color_make(0x66, 0x00, 0xFF), -10);

	//Set style for analog_clock_1_analog_clock_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->analog_clock_1_analog_clock_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->analog_clock_1_analog_clock_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->analog_clock_1_analog_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->analog_clock_1_analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->analog_clock_1_analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->analog_clock_1_analog_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->analog_clock_1_analog_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->analog_clock_1_analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->analog_clock_1_analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->analog_clock_1_analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->analog_clock_1_analog_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->analog_clock_1_analog_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->analog_clock_1_analog_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for analog_clock_1_analog_clock_1. Part: LV_PART_TICKS, State: LV_STATE_DEFAULT
	lv_obj_set_style_text_color(ui->analog_clock_1_analog_clock_1, lv_color_make(0xff, 0x00, 0x00), LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->analog_clock_1_analog_clock_1, &lv_font_montserrat_12, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes analog_clock_1_chart_1
	lv_obj_t * analog_clock_1_tabview_1_Distance_label = lv_label_create(ui->analog_clock_1_tabview_1_Distance);
	lv_label_set_text(analog_clock_1_tabview_1_Distance_label, "Santimerty`");
	ui->analog_clock_1_chart_1 = lv_chart_create(ui->analog_clock_1_tabview_1_Distance);
	lv_obj_set_pos(ui->analog_clock_1_chart_1, 40, 33);
	lv_obj_set_size(ui->analog_clock_1_chart_1, 180, 155);
	lv_obj_set_scrollbar_mode(ui->analog_clock_1_chart_1, LV_SCROLLBAR_MODE_AUTO);

	//Set style for analog_clock_1_chart_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->analog_clock_1_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->analog_clock_1_chart_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->analog_clock_1_chart_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->analog_clock_1_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->analog_clock_1_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->analog_clock_1_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->analog_clock_1_chart_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->analog_clock_1_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->analog_clock_1_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->analog_clock_1_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->analog_clock_1_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->analog_clock_1_chart_1, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->analog_clock_1_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->analog_clock_1_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->analog_clock_1_chart_1, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->analog_clock_1_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->analog_clock_1_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_chart_set_type(ui->analog_clock_1_chart_1, LV_CHART_TYPE_LINE);
	lv_chart_set_range(ui->analog_clock_1_chart_1,LV_CHART_AXIS_PRIMARY_Y, 0, 300);
	lv_chart_set_div_line_count(ui->analog_clock_1_chart_1, 3, 10);
	lv_chart_set_point_count(ui->analog_clock_1_chart_1, 5);
	lv_chart_set_axis_tick(ui->analog_clock_1_chart_1, LV_CHART_AXIS_PRIMARY_Y, 5, 2, 10, 2, true, 40);
	ui->analog_clock_1_chart_1_0 = lv_chart_add_series(ui->analog_clock_1_chart_1, lv_color_make(0x00, 0x00, 0x00), LV_CHART_AXIS_PRIMARY_Y);
	lv_chart_set_next_value(ui->analog_clock_1_chart_1, ui->analog_clock_1_chart_1_0, 1);
	lv_chart_set_next_value(ui->analog_clock_1_chart_1, ui->analog_clock_1_chart_1_0, 20);
	lv_chart_set_next_value(ui->analog_clock_1_chart_1, ui->analog_clock_1_chart_1_0, 30);
	lv_chart_set_next_value(ui->analog_clock_1_chart_1, ui->analog_clock_1_chart_1_0, 40);
	lv_chart_set_next_value(ui->analog_clock_1_chart_1, ui->analog_clock_1_chart_1_0, 5);
}

void setup_main_page_gui(){

	setup_scr_screen(&main_ui);
	lv_scr_load(main_ui.analog_clock_1);
	return;
}
