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

lv_obj_t *create_container(lv_obj_t *screen)
{
  auto *obj = lv_obj_create(screen);
  lv_obj_set_pos(obj, 0, 0);
  lv_obj_set_size(obj, 160, 128);
  lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  //lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN); // Vertical alignment
  lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_pad_all(obj, 2, 0);          // Add padding
  //lv_obj_set_style_border_width(obj, 2, 0);      // Optional: Add a border

  return obj;
}

lv_obj_t *g_txt = nullptr;

void create_screen_main(lv_obj_t *screen) 
{
  auto *cont = create_container(screen);
  auto *label = lv_label_create(cont);
  //lv_obj_set_pos(label, 0, 0);
  lv_label_set_text(label, "Freq:");
  //lv_obj_set_align(label, LV_ALIGN_LEFT_MID);
  lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *txt_area = lv_textarea_create(cont);
  //lv_obj_set_pos(txt_area, 35, 0);
  lv_obj_set_size(txt_area, 90, 10);
  lv_textarea_set_max_length(txt_area, 10);
  lv_textarea_set_text(txt_area, "0,000,000");
  lv_textarea_set_one_line(txt_area, true);
  lv_textarea_set_password_mode(txt_area, false);
  lv_obj_add_state(txt_area, LV_STATE_FOCUSED|LV_STATE_FOCUS_KEY|LV_STATE_PRESSED);
  lv_obj_set_style_text_color(txt_area, lv_color_black(),  LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_textarea_set_cursor_pos(txt_area, 4);
  //lv_textarea_set_text_selection(txt_area, true);
  //lv_label_set_text_selection_start(txt_area, 1);
  //lv_label_set_text_selection_end(txt_area, 3);
  //lv_obj_set_style_text_decor(txt_area, LV_TEXT_DECOR_UNDERLINE, LV_PART_CURSOR | LV_STATE_FOCUSED);
  // Optional: Adjust cursor width and color
  lv_obj_set_style_text_color(txt_area, lv_color_make(0xFF, 0, 0), LV_PART_CURSOR | LV_STATE_FOCUSED); // Cursor color
  lv_obj_set_style_text_color(txt_area, lv_color_make(0xFF, 0xFF, 0), LV_PART_KNOB | LV_STATE_FOCUSED); // Cursor color
  lv_obj_set_style_text_color(txt_area, lv_color_make(0x0, 0x0, 0xFF), LV_PART_SELECTED | LV_STATE_FOCUSED | LV_STATE_PRESSED);   // Pressed color
  lv_textarea_set_accepted_chars(txt_area, "0123456789,");
  g_txt = txt_area;
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 5);

  /*Make a gradient*/
  lv_style_set_bg_opa(&style, LV_OPA_COVER);
  static lv_grad_dsc_t grad;
  grad.dir = LV_GRAD_DIR_VER;
  grad.stops_count = 2;
  grad.stops[0].color = lv_palette_lighten(LV_PALETTE_GREY, 1);
  grad.stops[0].opa = LV_OPA_COVER;
  grad.stops[1].color = lv_palette_main(LV_PALETTE_BLUE);
  grad.stops[1].opa = LV_OPA_COVER;

  /*Shift the gradient to the bottom*/
  grad.stops[0].frac  = 128;
  grad.stops[1].frac  = 192;

  lv_style_set_bg_grad(&style, &grad);
  lv_obj_add_style(txt_area, &style, 0);
  lv_obj_set_style_text_decor(txt_area, LV_TEXT_DECOR_UNDERLINE, LV_PART_CURSOR | LV_STATE_FOCUSED);

  //lv_obj_set_style_text_opa(txt_area, LV_OPA_TRANSP, LV_PART_CURSOR | LV_STATE_FOCUSED); // Make cursor fully transparent

  //lv_obj_set_style_text_opa(txt_area, LV_OPA_100, LV_PART_CURSOR | LV_STATE_FOCUSED);       // Cursor opacity

  lv_obj_align_to(label, txt_area, LV_ALIGN_LEFT_MID, 5, 10);
  lv_obj_add_event_cb(txt_area, [](lv_event_t *e) {
      lv_event_code_t code = lv_event_get_code(e);
      lv_obj_t * textarea = (lv_obj_t *) lv_event_get_target(e);

      if (code == LV_EVENT_FOCUSED) {

      } else if (code == LV_EVENT_CLICKED) {
        
      }
  }, LV_EVENT_FOCUSED, NULL);
}

void create_freq_editor(lv_obj_t *parent)
{
   create_screen_main(parent);
}


volatile bool buttonPressed = false;  // To track button state
volatile unsigned long lastDebounceTime = 0;
void IRAM_ATTR handleButtonPress() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime > 500) {
    buttonPressed = true;
    lastDebounceTime = currentTime;
  }
}


volatile bool valueButtonPressed = false;  // To track button state
volatile unsigned long value_lastDebounceTime = 0;
void IRAM_ATTR valueChangeButtonPress() {
  unsigned long currentTime = millis();
  if (currentTime - value_lastDebounceTime > 500) {
    valueButtonPressed = true;
    value_lastDebounceTime = currentTime;
  }
}

#define CUSROR_MOVE_PIN 26
#define VALUE_CHANGE_PIN 25

void setup() 
{
  Serial.begin(115200);

  pinMode(CUSROR_MOVE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CUSROR_MOVE_PIN), handleButtonPress, RISING);
  pinMode(VALUE_CHANGE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(VALUE_CHANGE_PIN), valueChangeButtonPress, RISING);

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

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x00), LV_PART_MAIN);

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

  //create_editable_label(lv_scr_act());
  create_freq_editor(lv_scr_act());

  Serial.println("setup done");
}

lv_obj_t *last_focused_obj = nullptr;

void loop() 
{
  lv_task_handler();  // let the GUI do its work
  if (buttonPressed) {
    //TODO: get the focused obj instead
    int pos = lv_textarea_get_cursor_pos(g_txt);
    Serial.printf("cursor pos=%d\r\n", pos);
    if (pos >= 9) {
      lv_textarea_set_cursor_pos(g_txt, 0);
    } else {
      const char *text = lv_textarea_get_text(g_txt);
      if (pos < strlen(text) and text[pos+1] == ',') {
        lv_textarea_cursor_right(g_txt);
        lv_textarea_cursor_right(g_txt);
      } else {
        lv_textarea_cursor_right(g_txt);
      }
    }

    buttonPressed = false;
  }

  if (valueButtonPressed) {
    //TODO: get the focused obj instead.
    int pos = lv_textarea_get_cursor_pos(g_txt);
    const char *text = lv_textarea_get_text(g_txt);
    char buf[10] = {};
    Serial.println("value btn clicked");
    strcpy(buf, text);
    if (pos >= 0 and pos <= strlen(text)) {
      int x = text[pos] - '0';
      Serial.printf("x=%d\r\n", x);
      x++;
      if (x > 9) {
        x = 0;
      }

     buf[pos] = x + '0';

     lv_textarea_set_text(g_txt, buf);
     lv_textarea_set_cursor_pos(g_txt, pos);
    }
    valueButtonPressed = false;
  }
}