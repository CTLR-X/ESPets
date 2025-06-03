#include <Arduino.h>
#include <LovyanGFX.h>
#include "LGFX_ESP32S3_RGB_TFT_SPI_ST7701_GT911.h"
#include "Pet.hpp"

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

// Function to draw static UI elements
void drawStaticElements() {
    // Draw labels
    gfx.setCursor(10, 10);
    gfx.setTextColor(TFT_WHITE);
    gfx.setTextSize(2);
    gfx.print("Name: ");
    gfx.print(myPet.getName());
    
    gfx.setCursor(10, 40);
    gfx.print("State: ");
    
    // Draw static parts of status bars with initial values
    int y = 100;
    int barWidth = 200;
    
    const char* labels[] = {"Health:", "Hunger:", "Happy:", "Energy:", "Clean:"};
    uint16_t colors[] = {TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_GREEN, TFT_BLUE};
    uint8_t values[] = {
        myPet.getHealth(),
        myPet.getHunger(),
        myPet.getHappiness(),
        myPet.getEnergy(),
        myPet.getCleanliness()
    };
    
    for(int i = 0; i < 5; i++) {
        gfx.setCursor(10, y + (i * 30));
        gfx.print(labels[i]);
        
        // Draw background
        gfx.fillRect(100, y + (i * 30), barWidth, 20, TFT_DARKGREY);
        
        // Draw initial value
        int width = (barWidth * values[i]) / 100;
        if (width > 0) {
            gfx.fillRect(100, y + (i * 30), width, 20, colors[i]);
        }
    }
    
    // Draw all buttons
    for(const auto& btn : buttons) {
        drawButton(btn);
    }
}

// Function to update dynamic UI elements
void drawPetStatus() {
    // Only update state text if it changed
    if (previousStatus.state != myPet.getState()) {
        gfx.fillRect(100, 40, 200, 20, TFT_BLACK);  // Clear previous state
        gfx.setCursor(100, 40);
        gfx.setTextColor(TFT_WHITE);
        gfx.print((int)myPet.getState());
        previousStatus.state = myPet.getState();
    }
    
    // Update status bars only if values changed
    int y = 100;
    int barWidth = 200;
    
    // Helper function to update a single bar
    auto updateBar = [&](uint8_t current, uint8_t& previous, int yPos, uint16_t color) {
        if (current != previous) {
            // Calculate new width
            int newWidth = (barWidth * current) / 100;
            int oldWidth = (barWidth * previous) / 100;
            
            // If new value is smaller, clear the excess
            if (newWidth < oldWidth) {
                gfx.fillRect(100 + newWidth, yPos, oldWidth - newWidth, 20, TFT_DARKGREY);
            }
            // If new value is larger, fill the new area
            if (newWidth > oldWidth) {
                gfx.fillRect(100 + oldWidth, yPos, newWidth - oldWidth, 20, color);
            }
            previous = current;
        }
    };
    
    // Update each bar
    updateBar(myPet.getHealth(), previousStatus.health, y, TFT_RED);
    updateBar(myPet.getHunger(), previousStatus.hunger, y + 30, TFT_ORANGE);
    updateBar(myPet.getHappiness(), previousStatus.happiness, y + 60, TFT_YELLOW);
    updateBar(myPet.getEnergy(), previousStatus.energy, y + 90, TFT_GREEN);
    updateBar(myPet.getCleanliness(), previousStatus.cleanliness, y + 120, TFT_BLUE);
}

// Function to check if a point is inside a button
bool isPointInButton(int16_t x, int16_t y, const Button& btn) {
    return x >= btn.x && x < btn.x + btn.width &&
           y >= btn.y && y < btn.y + btn.height;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");
    
    gfx.begin();
    gfx.setRotation(0);
    gfx.setBrightness(128);
    
    // Initial display setup
    gfx.setTextSize(2);
    gfx.setTextColor(TFT_WHITE);
    
    // Clear screen
    gfx.fillScreen(TFT_BLACK);
    
    // Initialize previous status
    previousStatus.state = myPet.getState();
    previousStatus.health = myPet.getHealth();
    previousStatus.hunger = myPet.getHunger();
    previousStatus.happiness = myPet.getHappiness();
    previousStatus.energy = myPet.getEnergy();
    previousStatus.cleanliness = myPet.getCleanliness();
    
    // Draw initial static elements
    drawStaticElements();
    
    // Force initial display of all values
    drawPetStatus();
    
    Serial.println("Setup complete");
}

void loop() {
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