#include "ft6x36.h"

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

lv_obj_t * mainLabel;
lv_obj_t * buttonLabel;

#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{
    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

bool touch_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    bool res = ft6x36_read(drv, data);
    return res;
}

void initDisplay()
{
    Serial.println("Display Begin");

    // LVGL Init
    lv_init();

    // Touch Init
    ft6x06_init(FT6236_I2C_SLAVE_ADDR);

#if USE_LV_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.begin(); /* TFT init */
    tft.setRotation(2); /* Portrait orientation */

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 480;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the touch input device driver*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_driver_read;
    lv_indev_drv_register(&indev_drv);

    createDisplayContent();
}

void createDisplayContent()
{
    
    /* Create simple label */
    mainLabel = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(mainLabel, "Weidemann Display Start (LVGL V7.0.X)");
    lv_obj_align(mainLabel, NULL, LV_ALIGN_CENTER, 0, 0);

lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button to the current screen*/
lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
lv_obj_set_size(btn, 200, 50);                          /*Set its size*/
lv_obj_set_event_cb(btn, btn_event_cb);                 /*Assign a callback to the button*/

buttonLabel = lv_label_create(btn, NULL);          /*Add a label to the button*/
lv_label_set_text(buttonLabel, "Button");                     /*Set the labels text*/

}

void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_PRESSED) {
        lv_label_set_text(mainLabel, "Clicked");
        Serial.println("Clicked");
    } else if(event == LV_EVENT_RELEASED) {
        lv_label_set_text(mainLabel, "Released");
        Serial.println("Released");
    }

}

void loopDisplay()
{
    lv_task_handler(); /* let the GUI do its work */
}
