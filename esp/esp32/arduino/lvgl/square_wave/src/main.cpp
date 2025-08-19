#include <Arduino.h>
//refer to this https://github.com/meshtastic/device-ui
// nice input driver examples

//go to .pio/lib/tft spi folder
// and define the pins inside User_Select.h header file

//lv_conf.h has to be copied parallel to lvgl folder location.
//./pio/libdeps/esp32dev/lv_conf.h
//#include <lv_conf.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(128, 160);
// LV_COLOR_DEPTH - 16
// LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (128 * 160/10 * (16/8))
uint32_t draw_buf[DRAW_BUF_SIZE/4];

static uint32_t my_tick(void)
{
  return millis();
}

void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors( ( uint16_t * )px_map, w * h, true );

  tft.endWrite();

  lv_display_flush_ready(disp);

  Serial.println("flush cb is called");
}

static void resolution_changed_event_cb(lv_event_t * e)
{
    lv_display_t * disp = (lv_display_t *)lv_event_get_target(e);
    //lv_tft_espi_t * dsc = (lv_tft_espi_t *)lv_display_get_driver_data(disp);
    int32_t hor_res = lv_display_get_horizontal_resolution(disp);
    int32_t ver_res = lv_display_get_vertical_resolution(disp);
    lv_display_rotation_t rot = lv_display_get_rotation(disp);

    /* handle rotation */
    switch(rot) {
        case LV_DISPLAY_ROTATION_0:
            tft.setRotation(0);   /* Portrait orientation */
            break;
        case LV_DISPLAY_ROTATION_90:
            tft.setRotation(1);   /* Landscape orientation */
            break;
        case LV_DISPLAY_ROTATION_180:
            tft.setRotation(2);   /* Portrait orientation, flipped */
            break;
        case LV_DISPLAY_ROTATION_270:
            tft.setRotation(3);   /* Landscape orientation, flipped */
            break;
    }
}



static const lv_coord_t ecg_sample[] = {
2, 2, 0, -15, -39, -63, -71, -68, -67, -69, -84, -95, -104, -107, -108, -107, -107, -107, -107, -114, -118, -117,
    -112, -100, -89, -83, -71, -64, -58, -58, -62, -62, -58, -51, -46, -39, -27, -10, 4, 7, 1, -3, 0, 14, 24, 30, 25, 19,
    13, 7, 12, 15, 18, 21, 13, 6, 9, 8, 17, 19, 13, 11, 11, 11, 23, 30, 37, 34, 25, 14, 15, 19, 28, 31, 26, 23, 25, 31,
    39, 37, 37, 34, 30, 32, 22, 29, 31, 33, 37, 23, 13, 7, 2, 4, -2, 2, 11, 22, 33, 19, -1, -27, -55, -67, -72, -71, -63,
    -49, -18, 35, 113, 230, 369, 525, 651, 722, 730, 667, 563, 454, 357 };

void setup() 
{
  Serial.begin(115200);
  lv_init();
  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(my_tick);

  lv_display_t *disp;
  disp = lv_display_create(128, 160);
  tft.begin(INITR_BLACKTAB);

  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_add_event_cb(disp, resolution_changed_event_cb, LV_EVENT_RESOLUTION_CHANGED, NULL);
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);
  // my st7735 blacktab does not have touch screen
  
  lv_display_set_default(disp);

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000), LV_PART_MAIN);

  lv_obj_t *chart = lv_chart_create(lv_screen_active());
  lv_obj_set_size(chart, 30, 50);
  lv_obj_align(chart, LV_ALIGN_RIGHT_MID, -10, -10);
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -200, 1000);
  
  /*Do not display points on the data*/
  lv_obj_set_style_size(chart, 0, 100, LV_PART_INDICATOR);
  lv_chart_series_t * ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
  uint32_t pcnt = sizeof(ecg_sample) / sizeof(ecg_sample[0]);
  lv_chart_set_point_count(chart, pcnt);
  lv_chart_set_ext_y_array(chart, ser, (lv_coord_t *)ecg_sample);

  // draw a square wave
  static lv_point_precise_t points[] = {{-5, 10}, {0, 10}, {0, 5}, {5,5}, {5, 10}, {10, 10}, {10, 5}, {15, 5}, {15, 10}, {20, 10}}; // Array to store wave points
  
  /*Create style*/
  static lv_style_t style_line;
  lv_style_init(&style_line);
  lv_style_set_line_width(&style_line, 1);
  lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
  lv_style_set_line_rounded(&style_line, true);

  lv_obj_t * line1;
  line1 = lv_line_create(lv_scr_act());
  //lv_obj_set_size(line1, 20, 20);
  lv_obj_set_pos(line1, 140, 0);
  lv_line_set_points(line1, points, sizeof(points)/sizeof(lv_point_precise_t));
  lv_obj_add_style(line1, &style_line, 0);
  //lv_obj_center(line1);

  Serial.println("setup done");
}

lv_obj_t *last_focused_obj = nullptr;

void loop() 
{
  lv_task_handler();  // let the GUI do its work
  delay(10);
}