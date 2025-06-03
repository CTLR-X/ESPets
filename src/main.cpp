#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <Wire.h>
#include <TAMC_GT911.h>

// Display configuration
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 39 /* CS */,
    48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    11 /* R0 */, 12 /* R1 */, 13 /* R2 */, 14 /* R3 */, 0 /* R4 */,
    8 /* G0 */, 20 /* G1 */, 3 /* G2 */, 46 /* G3 */, 9 /* G4 */, 10 /* G5 */,
    4 /* B0 */, 5 /* B1 */, 6 /* B2 */, 7 /* B3 */, 15 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 1 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type9_init_operations, sizeof(st7701_type9_init_operations));

// GT911 Touch configuration
#define TOUCH_GT911
#define TOUCH_GT911_SCL 45
#define TOUCH_GT911_SDA 19
#define TOUCH_GT911_INT -1
#define TOUCH_GT911_RST -1
#define TOUCH_GT911_ROTATION 2  // 0: 0°, 1: 90°, 2: 180°, 3: 270°
#define TOUCH_MAP_X1 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_Y2 0

TAMC_GT911 ts = TAMC_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

int16_t last_x = -1;
int16_t last_y = -1;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 RGB Panel Test");

  // Initialize I2C for GT911
  Wire.begin(TOUCH_GT911_SDA, TOUCH_GT911_SCL);
  
  // Initialize GT911
  ts.begin();
  ts.setRotation(TOUCH_GT911_ROTATION);  // Set the rotation
  Serial.println("GT911 initialization completed");
  
  // Initialize display
  if (!gfx->begin()) {
    Serial.println("Display initialization failed!");
    while (1);
  }

  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);

  // Set display parameters
  gfx->fillScreen(BLACK);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2);
  
  // Draw a test pattern
  gfx->fillRect(50, 50, 100, 100, 0x001F);  // Should be red (using blue value)
  gfx->fillRect(200, 50, 100, 100, 0xF800); // Should be green (using red value)
  gfx->fillRect(350, 50, 100, 100, 0x07E0); // Should be blue (using green value)
}

void loop() {
  // Read touch data from GT911
  ts.read();
  
  if (ts.isTouched) {
    // Get touch coordinates
    uint16_t x = ts.points[0].x;
    uint16_t y = ts.points[0].y;
    
    Serial.print("Raw coordinates - X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.println(y);
    
    // Map coordinates based on rotation
    if (TOUCH_GT911_ROTATION == 0) {
      x = map(x, 0, 480, TOUCH_MAP_X1, TOUCH_MAP_X2);
      y = map(y, 0, 480, TOUCH_MAP_Y1, TOUCH_MAP_Y2);
      
      Serial.print("Mapped coordinates - X: ");
      Serial.print(x);
      Serial.print(" Y: ");
      Serial.println(y);
    }
    
    // Draw a line from the last touch point to the current one
    if (last_x != -1 && last_y != -1) {
      Serial.print("Drawing line from (");
      Serial.print(last_x);
      Serial.print(",");
      Serial.print(last_y);
      Serial.print(") to (");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.println(")");
      
      gfx->drawLine(last_x, last_y, x, y, WHITE);
    }
    
    // Update last touch point
    last_x = x;
    last_y = y;
  } else {
    // Reset last touch point when touch is released
    if (last_x != -1 || last_y != -1) {
      Serial.println("Touch released");
    }
    last_x = -1;
    last_y = -1;
  }
  
  delay(10); // Small delay to prevent overwhelming the touch sensor
}