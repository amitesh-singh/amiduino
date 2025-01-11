#pragma METER_WIDGET

#include "lvgl.h"

// Forward declaration of the custom widget's type
typedef struct {
    lv_obj_t *obj;         // Base LVGL object
    int32_t value;        // Custom property: meter value
} lv_custom_meter_t;

// Custom widget class
extern const lv_obj_class_t lv_custom_meter_class;
lv_obj_t *lv_custom_meter_create(lv_obj_t *parent);

void lv_custom_meter_set_value(lv_obj_t *obj, int32_t value);
int32_t lv_custom_meter_get_value(lv_obj_t *obj);
