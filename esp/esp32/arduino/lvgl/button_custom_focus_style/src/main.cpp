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

/*

This example shows how to navigate to different widgets using single button

        { LCD  }
[navigate_btn]

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
    Serial.println("btn1 focused");
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
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

#define NAVIGATE_BUTTON_PIN 26
void setup() 
{
  pinMode(NAVIGATE_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(NAVIGATE_BUTTON_PIN), handleButtonPress, RISING);

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
  static lv_style_t style_focused;
  lv_style_init(&style_focused);
  lv_style_set_bg_color(&style_focused, lv_palette_main(LV_PALETTE_GREEN));
  lv_style_set_border_color(&style_focused, lv_palette_darken(LV_PALETTE_GREEN, 3));
  lv_style_set_border_width(&style_focused, 4);
  //lv_style_set_radius(&style_focused, 10);

  lv_obj_t *btn = lv_button_create( lv_screen_active() );
  lv_obj_set_pos(btn, 5, 5);
  lv_obj_set_size(btn, 35, 30);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_FOCUSED, NULL);
  // set the button style when it's focused
  lv_obj_add_style(btn, &style_focused, LV_STATE_FOCUSED);

  lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
  lv_label_set_text(label, "CH1");                     /*Set the labels text*/
  lv_obj_center(label);

  lv_obj_t *btn2 = lv_button_create( lv_screen_active() );
  lv_obj_set_pos(btn2, 5, 40);
  lv_obj_set_size(btn2, 35, 30);
  lv_obj_add_event_cb(btn2, 
    [](lv_event_t * e) {
      Serial.println("btn2 focused");
    }, 
    LV_EVENT_FOCUSED, NULL);
  // set the button style when it's focused
  lv_obj_add_style(btn2, &style_focused, LV_STATE_FOCUSED);

  label = lv_label_create(btn2);          /*Add a label to the button*/
  lv_label_set_text(label, "CH2");                     /*Set the labels text*/
  lv_obj_center(label);

  lv_group_t *input_group;
  input_group = lv_group_create();
  lv_group_set_default(input_group);
  
  lv_group_add_obj(input_group, btn);
  lv_group_add_obj(input_group, btn2);

   /*Initialize the (dummy) input device driver*/
  lv_indev_t * button_indev_input = lv_indev_create();
  lv_indev_set_type(button_indev_input, LV_INDEV_TYPE_KEYPAD); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(button_indev_input, tactile_dev_btn_cb);
  lv_indev_set_group(button_indev_input, input_group);
  

  //lv_obj_t *meter = lv_custom_meter_create(lv_screen_active());
  //lv_obj_align(meter, LV_ALIGN_CENTER, 0, 0);

    // Set the value of the meter
  //lv_custom_meter_set_value(meter, 75);

  Serial.println("setup done");
}

void loop() 
{
  lv_task_handler();  // let the GUI do its work
  delay(10);
}