#include <Arduino.h>
#include <LovyanGFX.h>
#include "LGFX_ESP32S3_RGB_TFT_SPI_ST7701_GT911.h"

LGFX gfx;

void setup()
{
    gfx.begin();
    gfx.setRotation(0);  // Set rotation if needed
    gfx.setBrightness(128);  // Set brightness (0-255)

    // Fill screen with black
    gfx.fillScreen(TFT_BLACK);
    
    // Draw some shapes
    gfx.fillRect(10, 10, 100, 100, TFT_RED);  // Red square
    gfx.fillCircle(200, 200, 50, TFT_GREEN);  // Green circle
    gfx.fillTriangle(300, 100, 350, 200, 400, 100, TFT_BLUE);  // Blue triangle
    
    // Draw some text
    gfx.setTextSize(2);
    gfx.setTextColor(TFT_WHITE);
    gfx.setCursor(50, 300);
    gfx.println("Hello, LovyanGFX!");
    
    // Draw a gradient
    for(int i = 0; i < gfx.width(); i++) {
        gfx.drawFastVLine(i, 350, 50, gfx.color565(i * 255 / gfx.width(), 0, 255 - (i * 255 / gfx.width())));
    }
}

void loop()
{
    // Read touch input
    uint16_t x, y;
    if (gfx.getTouch(&x, &y)) {
        // Draw a small circle where touched
        gfx.fillCircle(x, y, 5, TFT_YELLOW);
        delay(1);  // Small delay to prevent too many touch points
    }
}