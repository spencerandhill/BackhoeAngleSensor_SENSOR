// Sliders
static lv_obj_t * sliderX;
static lv_obj_t * sliderY;

static lv_obj_t * sliderX_label;
static lv_obj_t * sliderY_label;

void createSensorSliders()
{
// X-Slider    
    /* Create a slider at the right of the display */
    sliderX = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_width(sliderX, LV_DPI * 2);
    lv_obj_align(sliderX, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_slider_set_range(sliderX, -128, 128);
    lv_obj_set_size(sliderX, 200, 50);
    lv_obj_set_pos(sliderX, 50, 200);
    lv_slider_set_value(sliderX, 0, LV_ANIM_OFF);
    
    /* Create a label below the slider */
    sliderX_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(sliderX_label, "0");
    lv_obj_set_auto_realign(sliderX_label, true);
    lv_obj_align(sliderX_label, sliderX, LV_ALIGN_OUT_BOTTOM_LEFT, -5, 10);

// Y-Slider    
    /* Create a slider at the right of the display */
    sliderY = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_width(sliderY, LV_DPI * 2);
    lv_obj_align(sliderY, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_slider_set_range(sliderY, -128, 128);
    lv_obj_set_size(sliderY, 50, 200);
    lv_obj_set_pos(sliderY, 250, 20);
    lv_slider_set_value(sliderY, 0, LV_ANIM_OFF);

    /* Create a label below the slider */
    sliderY_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(sliderY_label, "0");
    lv_obj_set_auto_realign(sliderY_label, true);
    lv_obj_align(sliderY_label, sliderY, LV_ALIGN_OUT_TOP_MID, 0, 0);
}

void updateSliderXY(float xValue, float yValue) 
{
    lv_slider_set_value(sliderX, (int16_t) xValue, LV_ANIM_OFF);
    lv_label_set_text(sliderX_label, String(xValue, 1).c_str());

    lv_slider_set_value(sliderY, (int16_t) yValue, LV_ANIM_OFF);
    lv_label_set_text(sliderY_label, String(yValue, 1).c_str());
}