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


static constexpr const uint8_t button_w = 50;
static constexpr const uint8_t button_h = 35;

/*

This example shows how to navigate to different widgets using single button and click the focused 
widget on clicking action button
two switch buttons are required

        { LCD  }
[navigate_btn]  [action button]

*/
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

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = (lv_obj_t *) lv_event_get_target(e);
    if (code == LV_EVENT_FOCUSED) {
      Serial.println("btn1 focused");
    } else if(code == LV_EVENT_CLICKED) {
       Serial.println("btn1 clicked");
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "CH1:%d", cnt);
    }
}

/** Data structure passed to an input driver to fill*/
//typedef struct {
//    lv_point_t point; /**< For LV_INDEV_TYPE_POINTER the currently pressed point*/
//    uint32_t key;     /**< For LV_INDEV_TYPE_KEYPAD the currently pressed key*/
//    uint32_t btn_id;  /**< For LV_INDEV_TYPE_BUTTON the currently pressed button*/
//    int16_t enc_diff; /**< For LV_INDEV_TYPE_ENCODER number of steps since the previous read*/

 //   lv_indev_state_t state; /**< LV_INDEV_STATE_RELEASED or LV_INDEV_STATE_PRESSED*/
 //   bool continue_reading;  /**< If set to true, the read callback is invoked again, unless the device is in event-driven mode*/
//} lv_indev_data_t;
volatile bool buttonPressed = false;  // To track button state
volatile unsigned long lastDebounceTime = 0;

void tactile_dev_btn_cb(lv_indev_t * indev, lv_indev_data_t * data)
{
 if (buttonPressed) {
  data->key = LV_KEY_NEXT;
  data->state = LV_INDEV_STATE_PR;
  buttonPressed = false;
 } else {
  data->state = LV_INDEV_STATE_REL;
 }
}


void IRAM_ATTR handleButtonPress() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime > 500) {
    buttonPressed = true;
    lastDebounceTime = currentTime;
  }
}


volatile bool buttonActionPressed = false;  // To track button state
volatile unsigned long lastDebounceTimeAction = 0;

void IRAM_ATTR handleActionButtonPress() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTimeAction > 500) {
    buttonActionPressed = true;
    lastDebounceTimeAction = currentTime;
  }
}
static const lv_coord_t ecg_sample[] = {
2, 2, 0, -15, -39, -63, -71, -68, -67, -69, -84, -95, -104, -107, -108, -107, -107, -107, -107, -114, -118, -117,
    -112, -100, -89, -83, -71, -64, -58, -58, -62, -62, -58, -51, -46, -39, -27, -10, 4, 7, 1, -3, 0, 14, 24, 30, 25, 19,
    13, 7, 12, 15, 18, 21, 13, 6, 9, 8, 17, 19, 13, 11, 11, 11, 23, 30, 37, 34, 25, 14, 15, 19, 28, 31, 26, 23, 25, 31,
    39, 37, 37, 34, 30, 32, 22, 29, 31, 33, 37, 23, 13, 7, 2, 4, -2, 2, 11, 22, 33, 19, -1, -27, -55, -67, -72, -71, -63,
    -49, -18, 35, 113, 230, 369, 525, 651, 722, 730, 667, 563, 454, 357 };

#define NAVIGATE_BUTTON_PIN 26
#define ACTION_BUTTON_PIN 25
lv_group_t *input_group;

void setup() 
{
  pinMode(NAVIGATE_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(NAVIGATE_BUTTON_PIN), handleButtonPress, RISING);

  pinMode(ACTION_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ACTION_BUTTON_PIN), handleActionButtonPress, RISING);
  Serial.begin(115200);
  lv_init();

  //tft.fillScreen(TFT_BLACK);
  //tft.drawString("I love <3 Aarav", 30, 30, 2);
  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(my_tick);

  lv_display_t *disp;
  // the below code also works and it uses the engine in lvgl;
  // check lv_tft_espi.cpp
  // you have to enable following macro in lv_conf.h
  // LV_USE_TFT_ESPI
  //disp = lv_tft_espi_create(128, 160, draw_buf, sizeof(draw_buf));
  //lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

  disp = lv_display_create(128, 160);
  tft.begin(INITR_BLACKTAB);

  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_add_event_cb(disp, resolution_changed_event_cb, LV_EVENT_RESOLUTION_CHANGED, NULL);
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);
  // my st7735 blacktab does not have touch screen
  
  lv_display_set_default(disp);

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000), LV_PART_MAIN);

  //style for focused state
  static lv_style_t style_focused, style_clicked;
  lv_style_init(&style_focused);
  lv_style_set_bg_color(&style_focused, lv_palette_main(LV_PALETTE_GREEN));
  lv_style_set_border_color(&style_focused, lv_palette_darken(LV_PALETTE_GREEN, 3));
  lv_style_set_border_width(&style_focused, 4);
  //lv_style_set_radius(&style_focused, 10);

  lv_style_init(&style_clicked);
  lv_style_set_bg_color(&style_clicked, lv_palette_main(LV_PALETTE_DEEP_PURPLE));
  lv_style_set_border_color(&style_clicked, lv_palette_darken(LV_PALETTE_DEEP_PURPLE, 3));
  lv_style_set_border_width(&style_clicked, 4);

  lv_obj_t *btn = lv_button_create( lv_screen_active() );
  lv_obj_set_pos(btn, 10, 10);
  lv_obj_set_size(btn, button_w, button_h);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
  // set the button style when it's focused
  lv_obj_add_style(btn, &style_focused, LV_STATE_FOCUSED);
  lv_obj_add_style(btn, &style_clicked, LV_STATE_PRESSED);

  lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
  lv_label_set_text(label, "CH1");                     /*Set the labels text*/
  lv_obj_center(label);

  
  input_group = lv_group_create();
  lv_group_set_default(input_group);
  
  lv_group_add_obj(input_group, btn);

   /*Initialize the (dummy) input device driver*/
  lv_indev_t * button_indev_input = lv_indev_create();
  lv_indev_set_type(button_indev_input, LV_INDEV_TYPE_KEYPAD); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(button_indev_input, tactile_dev_btn_cb);
  lv_indev_set_group(button_indev_input, input_group);
  
  //make CH1 button focus initially
  lv_group_focus_obj(btn);
  //lv_group_get_focused()

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

  lv_obj_set_state(btn, LV_STATE_DISABLED, true);

  Serial.println("setup done");
}

lv_obj_t *last_focused_obj = nullptr;

void loop() 
{
  lv_task_handler();  // let the GUI do its work
  if (buttonActionPressed) {
    Serial.println("action button pressed");
    auto *focused_obj = lv_group_get_focused(input_group);
    
    //lv_obj_send_event(last_focused_obj, LV_EVENT_DEFOCUSED, NULL);
    //lv_obj_send_event(last_focused_obj, , NULL);
    //if (last_focused_obj)
    //  lv_obj_clear_state(last_focused_obj, LV_STATE_PRESSED);
    lv_obj_send_event(focused_obj, LV_EVENT_CLICKED, NULL);
    auto state = lv_obj_get_state(focused_obj);
    Serial.printf("state = %d\r\n", state);
    if (lv_obj_get_state(focused_obj) == 38 or lv_obj_get_state(focused_obj) == LV_EVENT_FOCUSED) {
      lv_obj_clear_state(last_focused_obj, LV_STATE_PRESSED);
      Serial.println("btn state is cleared");
    } else {
      Serial.println("button state is pressed");
      lv_obj_send_event(focused_obj, LV_EVENT_PRESSED, NULL);
    }
    last_focused_obj = focused_obj;
    buttonActionPressed = false;
  }
  //delay(10);
}