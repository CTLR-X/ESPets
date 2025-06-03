#include <Arduino.h>
#include <LovyanGFX.h>
#include "LGFX_ESP32S3_RGB_TFT_SPI_ST7701_GT911.h"
#include "Pet.hpp"

// Forward declarations
void drawStaticElements();
void drawPetStatus();

LGFX gfx;
Pet myPet("Buddy");

// Store previous values to detect changes
struct PetStatus {
    PetState state;
    uint8_t health;
    uint8_t hunger;
    uint8_t happiness;
    uint8_t energy;
    uint8_t cleanliness;
} previousStatus;

// Button definitions
struct Button {
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    const char* label;
    uint16_t color;
    uint16_t textColor;
};

// Define buttons
const Button buttons[] = {
    {10, 300, 100, 40, "Feed", TFT_RED, TFT_WHITE},
    {120, 300, 100, 40, "Play", TFT_GREEN, TFT_WHITE},
    {230, 300, 100, 40, "Sleep", TFT_BLUE, TFT_WHITE},
    {340, 300, 100, 40, "Clean", TFT_YELLOW, TFT_BLACK}
};

// Timing variables
unsigned long lastUpdateTime = 0;
unsigned long lastTouchTime = 0;
const unsigned long UPDATE_INTERVAL = 1000;  // 1 second
const unsigned long TOUCH_DEBOUNCE = 200;    // 200ms

// Function to draw a button
void drawButton(const Button& btn, bool pressed = false) {
    // Draw button background
    gfx.fillRoundRect(btn.x, btn.y, btn.width, btn.height, 8, pressed ? gfx.color565(100, 100, 100) : btn.color);
    
    // Draw button border
    gfx.drawRoundRect(btn.x, btn.y, btn.width, btn.height, 8, TFT_WHITE);
    
    // Draw button label
    gfx.setTextColor(btn.textColor);
    gfx.setTextSize(2);
    
    // Center the text
    int16_t textWidth = gfx.textWidth(btn.label);
    int16_t textX = btn.x + (btn.width - textWidth) / 2;
    int16_t textY = btn.y + (btn.height - 16) / 2;  // 16 is approximate text height
    
    gfx.setCursor(textX, textY);
    gfx.print(btn.label);
}

void drawStaticElements() {
    // Clear screen with a test pattern
    gfx.fillScreen(TFT_BLACK);
    
    // Draw a test pattern in the background
    for(int i = 0; i < gfx.width(); i += 20) {
        gfx.drawFastVLine(i, 0, gfx.height(), TFT_DARKGREY);
    }
    
    // Draw labels with background
    gfx.fillRect(0, 0, gfx.width(), 80, TFT_BLACK);
    gfx.setCursor(10, 10);
    gfx.setTextColor(TFT_WHITE);
    gfx.setTextSize(2);
    gfx.print("Name: ");
    gfx.print(myPet.getName());
    
    gfx.setCursor(10, 40);
    gfx.print("State: ");
    
    // Draw static parts of status bars with background
    int y = 100;
    int barWidth = 200;
    
    const char* labels[] = {"Health:", "Hunger:", "Happy:", "Energy:", "Clean:"};
    for(int i = 0; i < 5; i++) {
        gfx.fillRect(0, y + (i * 30) - 2, gfx.width(), 24, TFT_BLACK);  // Clear area with margin
        gfx.setCursor(10, y + (i * 30));
        gfx.print(labels[i]);
        gfx.fillRect(100, y + (i * 30), barWidth, 20, TFT_DARKGREY);
    }
    
    // Draw all buttons
    for(const auto& btn : buttons) {
        drawButton(btn);
    }
}

// Function to update a single status bar
void updateStatusBar(int index, uint8_t value, uint8_t& previous, uint16_t color) {
    // Always draw the bar, regardless of value change
    int16_t y = 100 + (index * 30);
    int16_t width = (200 * value) / 100;
    
    // Use a single fill operation for the background
    gfx.fillRect(100, y, 200, 20, TFT_DARKGREY);
    
    // Draw the new value if it's greater than 0
    if (width > 0) {
        gfx.fillRect(100, y, width, 20, color);
    }
    
    previous = value;
}

// Function to update dynamic UI elements
void drawPetStatus() {
    // Update state text with a single operation
    gfx.fillRect(100, 40, 200, 20, TFT_BLACK);
    gfx.setCursor(100, 40);
    gfx.setTextColor(TFT_WHITE);
    gfx.print((int)myPet.getState());
    
    // Update each status bar
    updateStatusBar(0, myPet.getHealth(), previousStatus.health, TFT_RED);
    updateStatusBar(1, myPet.getHunger(), previousStatus.hunger, TFT_ORANGE);
    updateStatusBar(2, myPet.getHappiness(), previousStatus.happiness, TFT_YELLOW);
    updateStatusBar(3, myPet.getEnergy(), previousStatus.energy, TFT_GREEN);
    updateStatusBar(4, myPet.getCleanliness(), previousStatus.cleanliness, TFT_BLUE);
}

// Function to check if a point is inside a button
bool isPointInButton(int16_t x, int16_t y, const Button& btn) {
    return x >= btn.x && x < btn.x + btn.width &&
           y >= btn.y && y < btn.y + btn.height;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting...");
    
    gfx.begin();
    gfx.setRotation(0);
    gfx.setBrightness(128);
    
    // Test display
    gfx.fillScreen(TFT_RED);
    delay(500);
    gfx.fillScreen(TFT_GREEN);
    delay(500);
    gfx.fillScreen(TFT_BLUE);
    delay(500);
    gfx.fillScreen(TFT_BLACK);
    
    // Initial display setup
    gfx.setTextSize(2);
    gfx.setTextColor(TFT_WHITE);
    
    // Clear screen with a single operation
    gfx.fillScreen(TFT_BLACK);
    
    // Draw static elements
    drawStaticElements();
    
    // Initialize previous status with current values
    previousStatus.state = myPet.getState();
    previousStatus.health = myPet.getHealth();
    previousStatus.hunger = myPet.getHunger();
    previousStatus.happiness = myPet.getHappiness();
    previousStatus.energy = myPet.getEnergy();
    previousStatus.cleanliness = myPet.getCleanliness();
    
    // Force initial display of all values
    drawPetStatus();
    
    Serial.println("Setup complete");
}

void loop()
{
    unsigned long currentTime = millis();
    
    // Update pet state and display at regular intervals
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        myPet.update();
        drawPetStatus();
        lastUpdateTime = currentTime;
        
        // Print status to Serial for debugging
        Serial.printf("State: %d, Health: %d, Hunger: %d, Happy: %d, Energy: %d, Clean: %d\n",
                     (int)myPet.getState(), myPet.getHealth(), myPet.getHunger(),
                     myPet.getHappiness(), myPet.getEnergy(), myPet.getCleanliness());
    }
    
    // Check for touch input with debounce
    if (currentTime - lastTouchTime >= TOUCH_DEBOUNCE) {
        uint16_t x, y;
        if (gfx.getTouch(&x, &y)) {
            Serial.printf("Touch at: %d, %d\n", x, y);
            lastTouchTime = currentTime;
            
            // Check which button was pressed
            for(int i = 0; i < 4; i++) {
                if (isPointInButton(x, y, buttons[i])) {
                    Serial.printf("Button %d pressed\n", i);
                    
                    // Draw pressed button
                    drawButton(buttons[i], true);
                    
                    // Perform action based on button index
                    switch(i) {
                        case 0: 
                            myPet.feed(); 
                            Serial.println("Feed action");
                            break;
                        case 1: 
                            myPet.play(); 
                            Serial.println("Play action");
                            break;
                        case 2: 
                            myPet.sleep(); 
                            Serial.println("Sleep action");
                            break;
                        case 3: 
                            myPet.clean(); 
                            Serial.println("Clean action");
                            break;
                    }
                    
                    // Redraw button in normal state
                    drawButton(buttons[i], false);
                    break;
                }
            }
        }
    }
}