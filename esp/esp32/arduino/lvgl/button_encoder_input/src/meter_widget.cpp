#include <lvgl_private.h>

#include "meter_widget.h"

// Forward declaration of private functions
static void lv_custom_meter_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void lv_custom_meter_event_handler(lv_event_t *e);

// Widget class definition
const lv_obj_class_t lv_custom_meter_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = lv_custom_meter_constructor,
    .destructor_cb = nullptr,
    .event_cb = nullptr,
    .user_data = nullptr,
    .name = "meter",
    .width_def = 100,
    .height_def = 100,
    .editable = 0,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .instance_size = sizeof(lv_custom_meter_t),
    .theme_inheritable = 1,
};

// Constructor function
static void lv_custom_meter_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj) {
    lv_custom_meter_t *meter = (lv_custom_meter_t *)obj;
    meter->value = 0; // Initialize custom property
}

// Create function (public API)
lv_obj_t *lv_custom_meter_create(lv_obj_t *parent) {

    lv_obj_t *obj = lv_obj_class_create_obj(&lv_custom_meter_class, parent);
    lv_obj_class_init_obj(obj);

    return obj;
}

void lv_custom_meter_set_value(lv_obj_t *obj, int32_t value) {
    lv_custom_meter_t *meter = (lv_custom_meter_t *)obj;
    meter->value = LV_CLAMP(0, value, 100); // Clamp value between 0 and 100
    lv_obj_invalidate(obj);                // Redraw the widget
}

int32_t lv_custom_meter_get_value(lv_obj_t *obj) {
    lv_custom_meter_t *meter = (lv_custom_meter_t *)obj;
    return meter->value;
}
