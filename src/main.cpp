#include <Arduino.h>
#include <LovyanGFX.h>
#include "LGFX_ESP32S3_RGB_TFT_SPI_ST7701_GT911.h" // Your specific display configuration
#include "Pet.hpp" // Your Pet class definition

// LVGL includes
#include <lvgl.h>

// --- LVGL Configuration ---
#define LV_HOR_RES_MAX 480
#define LV_VER_RES_MAX 480
#define LVGL_BUFFER_SIZE (LV_HOR_RES_MAX * LV_VER_RES_MAX / 4)

// LVGL v9.x uses lv_display_t and lv_color_t
static lv_display_t* display;
static lv_color_t buf[LVGL_BUFFER_SIZE];

// --- Global Objects ---
LGFX gfx;
Pet myPet("Buddy");

// --- LVGL UI Objects ---
lv_obj_t* screen;
lv_obj_t* nameLabel;
lv_obj_t* stateLabel;
lv_obj_t* healthBar;
lv_obj_t* hungerBar;
lv_obj_t* happinessBar;
lv_obj_t* energyBar;
lv_obj_t* cleanlinessBar;

// --- LVGL Callbacks (Updated for LVGL v9.x API) ---
void lv_disp_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    gfx.pushImage(area->x1, area->y1, w, h, (lgfx::rgb565_t*)px_map);
    lv_display_flush_ready(disp);
}

void lv_touchpad_read_cb(lv_indev_t* indev, lv_indev_data_t* data) {
    uint16_t touchX, touchY;
    if (gfx.getTouch(&touchX, &touchY)) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// --- LVGL Event Handlers for Buttons ---
void feed_btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        myPet.feed();
        Serial.println("Feed button clicked!");
    }
}

void play_btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        myPet.play();
        Serial.println("Play button clicked!");
    }
}

void sleep_btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        myPet.sleep();
        Serial.println("Sleep button clicked!");
    }
}

void clean_btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        myPet.clean();
        Serial.println("Clean button clicked!");
    }
}

// --- UI Creation Function ---
void create_ui() {
    screen = lv_obj_create(NULL);
    lv_scr_load(screen);

    // Set background color
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);

    // Pet Name Label
    nameLabel = lv_label_create(screen);
    lv_label_set_text_fmt(nameLabel, "Name: %s", myPet.getName());
    lv_obj_set_style_text_color(nameLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(nameLabel, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_pos(nameLabel, 10, 10);

    // Pet State Label
    stateLabel = lv_label_create(screen);
    lv_label_set_text_fmt(stateLabel, "State: %d", (int)myPet.getState());
    lv_obj_set_style_text_color(stateLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(stateLabel, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_pos(stateLabel, 10, 50);

    // Status Bars
    lv_obj_t* labels[] = {
        lv_label_create(screen), lv_label_create(screen), lv_label_create(screen),
        lv_label_create(screen), lv_label_create(screen)
    };
    
    const char* bar_labels[] = {"Health:", "Hunger:", "Happy:", "Energy:", "Clean:"};
    lv_color_t bar_colors[] = {
        lv_color_hex(0xFF0000),
        lv_color_hex(0xFFA500),
        lv_color_hex(0xFFFF00),
        lv_color_hex(0x00FF00),
        lv_color_hex(0x0000FF)
    };

    int y_start_bars = 100;
    int bar_height = 20;
    int bar_width = 250;
    int spacing = 30;

    for (int i = 0; i < 5; ++i) {
        lv_label_set_text(labels[i], bar_labels[i]);
        lv_obj_set_style_text_color(labels[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_font(labels[i], &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_set_pos(labels[i], 10, y_start_bars + i * spacing);

        lv_obj_t* current_bar = lv_bar_create(screen);
        lv_obj_set_size(current_bar, bar_width, bar_height);
        lv_obj_set_pos(current_bar, 100, y_start_bars + i * spacing);
        lv_bar_set_range(current_bar, 0, 100);

        lv_obj_set_style_bg_color(current_bar, bar_colors[i], LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(current_bar, lv_color_hex(0x404040), LV_PART_MAIN);
        lv_obj_set_style_radius(current_bar, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_PART_INDICATOR);

        if (i == 0) healthBar = current_bar;
        else if (i == 1) hungerBar = current_bar;
        else if (i == 2) happinessBar = current_bar;
        else if (i == 3) energyBar = current_bar;
        else if (i == 4) cleanlinessBar = current_bar;
    }

    // Buttons
    lv_obj_t* btns[] = {
        lv_btn_create(screen), lv_btn_create(screen), lv_btn_create(screen), lv_btn_create(screen)
    };
    lv_obj_t* btn_labels[] = {
        lv_label_create(btns[0]), lv_label_create(btns[1]), lv_label_create(btns[2]), lv_label_create(btns[3])
    };
    const char* btn_texts[] = {"Feed", "Play", "Sleep", "Clean"};
    lv_event_cb_t btn_cbs[] = {feed_btn_event_cb, play_btn_event_cb, sleep_btn_event_cb, clean_btn_event_cb};
    lv_color_t btn_colors[] = {
        lv_color_hex(0xFF0000),
        lv_color_hex(0x00FF00),
        lv_color_hex(0x0000FF),
        lv_color_hex(0xFFFF00)
    };

    int btn_width = 100;
    int btn_height = 40;
    int btn_start_x = 10;
    int btn_y = 300;
    int btn_spacing = 110;

    for (int i = 0; i < 4; ++i) {
        lv_obj_set_size(btns[i], btn_width, btn_height);
        lv_obj_set_pos(btns[i], btn_start_x + i * btn_spacing, btn_y);
        
        lv_obj_set_style_bg_color(btns[i], btn_colors[i], LV_PART_MAIN);
        lv_obj_set_style_radius(btns[i], 8, LV_PART_MAIN);

        lv_label_set_text(btn_labels[i], btn_texts[i]);
        lv_obj_set_style_text_color(btn_labels[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_font(btn_labels[i], &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_center(btn_labels[i]);

        lv_obj_add_event_cb(btns[i], btn_cbs[i], LV_EVENT_CLICKED, NULL);
    }
}

void update_ui_pet_status() {
    lv_label_set_text_fmt(stateLabel, "State: %d", (int)myPet.getState());

    lv_bar_set_value(healthBar, myPet.getHealth(), LV_ANIM_OFF);
    lv_bar_set_value(hungerBar, myPet.getHunger(), LV_ANIM_OFF);
    lv_bar_set_value(happinessBar, myPet.getHappiness(), LV_ANIM_OFF);
    lv_bar_set_value(energyBar, myPet.getEnergy(), LV_ANIM_OFF);
    lv_bar_set_value(cleanlinessBar, myPet.getCleanliness(), LV_ANIM_OFF);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESPets with LVGL...");

    gfx.begin();
    gfx.setRotation(0);
    gfx.setBrightness(128);
    gfx.fillScreen(TFT_BLACK);

    lv_init();

    // Initialize LVGL display (v9.x style)
    display = lv_display_create(LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_display_set_flush_cb(display, lv_disp_flush_cb);
    lv_display_set_buffers(display, buf, NULL, LVGL_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Register input device driver (v9.x style)
    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, lv_touchpad_read_cb);

    create_ui();
    update_ui_pet_status();

    Serial.println("Setup complete. UI created.");
}

unsigned long lastPetUpdateTime = 0;
const unsigned long PET_UPDATE_INTERVAL = 1000;

void loop() {
    unsigned long currentTime = millis();

    if (currentTime - lastPetUpdateTime >= PET_UPDATE_INTERVAL) {
        myPet.update();
        update_ui_pet_status();
        lastPetUpdateTime = currentTime;

        Serial.printf("State: %d, Health: %d, Hunger: %d, Happy: %d, Energy: %d, Clean: %d\n",
                     (int)myPet.getState(), myPet.getHealth(), myPet.getHunger(),
                     myPet.getHappiness(), myPet.getEnergy(), myPet.getCleanliness());
    }

    lv_timer_handler();
    delay(5);
}
