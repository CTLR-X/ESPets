#pragma once

#include <Arduino.h>

// Pet states
enum class PetState {
    IDLE,
    HAPPY,
    SAD,
    HUNGRY,
    SLEEPING,
    PLAYING,
    EATING,
    MOVING
};

class Pet {
private:
    // Basic attributes
    String name;
    uint8_t age;  // in days
    uint8_t health;  // 0-100
    uint8_t hunger;  // 0-100
    uint8_t happiness;  // 0-100
    uint8_t energy;  // 0-100
    uint8_t cleanliness;  // 0-100
    
    // State management
    PetState currentState;
    unsigned long lastUpdateTime;
    unsigned long lastStateChangeTime;
    
    // Constants for decay rates (in milliseconds)
    static const unsigned long HUNGER_DECAY_INTERVAL = 300000;  // 5 minutes
    static const unsigned long HAPPINESS_DECAY_INTERVAL = 600000;  // 10 minutes
    static const unsigned long ENERGY_DECAY_INTERVAL = 900000;  // 15 minutes
    static const unsigned long CLEANLINESS_DECAY_INTERVAL = 1200000;  // 20 minutes
    
    // Thresholds for state changes
    static const uint8_t HUNGRY_THRESHOLD = 70;
    static const uint8_t SAD_THRESHOLD = 30;
    static const uint8_t TIRED_THRESHOLD = 30;
    static const uint8_t DIRTY_THRESHOLD = 30;

public:
    // Constructor
    Pet(const String& petName = "Pet") : 
        name(petName),
        age(0),
        health(100),
        hunger(0),
        happiness(100),
        energy(100),
        cleanliness(100),
        currentState(PetState::IDLE),
        lastUpdateTime(millis()),
        lastStateChangeTime(millis())
    {}

    // Getters
    String getName() const { return name; }
    uint8_t getAge() const { return age; }
    uint8_t getHealth() const { return health; }
    uint8_t getHunger() const { return hunger; }
    uint8_t getHappiness() const { return happiness; }
    uint8_t getEnergy() const { return energy; }
    uint8_t getCleanliness() const { return cleanliness; }
    PetState getState() const { return currentState; }

    // Setters with bounds checking
    void setHealth(uint8_t value) { health = constrain(value, 0, 100); }
    void setHunger(uint8_t value) { hunger = constrain(value, 0, 100); }
    void setHappiness(uint8_t value) { happiness = constrain(value, 0, 100); }
    void setEnergy(uint8_t value) { energy = constrain(value, 0, 100); }
    void setCleanliness(uint8_t value) { cleanliness = constrain(value, 0, 100); }

    // Action methods
    void feed() {
        hunger = max(0, hunger - 30);  // Reduce hunger by 30
        happiness = min(100, happiness + 10);  // Increase happiness
        setState(PetState::EATING);
    }

    void play() {
        if (energy >= 20) {  // Can only play if there's enough energy
            energy = max(0, energy - 20);
            happiness = min(100, happiness + 30);
            hunger = min(100, hunger + 10);
            setState(PetState::PLAYING);
        }
    }

    void sleep() {
        energy = min(100, energy + 50);
        setState(PetState::SLEEPING);
    }

    void clean() {
        cleanliness = 100;
        happiness = min(100, happiness + 20);
    }

    // Update method to be called in the main loop
    void update() {
        unsigned long currentTime = millis();
        
        // Update age (increment every 24 hours)
        if (currentTime - lastUpdateTime >= 86400000) {  // 24 hours in milliseconds
            age++;
            lastUpdateTime = currentTime;
        }

        // Decay attributes over time
        if (currentTime - lastUpdateTime >= HUNGER_DECAY_INTERVAL) {
            hunger = min(100, hunger + 5);
        }
        if (currentTime - lastUpdateTime >= HAPPINESS_DECAY_INTERVAL) {
            happiness = max(0, happiness - 5);
        }
        if (currentTime - lastUpdateTime >= ENERGY_DECAY_INTERVAL) {
            energy = max(0, energy - 5);
        }
        if (currentTime - lastUpdateTime >= CLEANLINESS_DECAY_INTERVAL) {
            cleanliness = max(0, cleanliness - 5);
        }

        // Update health based on other attributes
        updateHealth();
        
        // Update state based on attributes
        updateState();
    }

private:
    void updateHealth() {
        // Health is affected by other attributes
        int healthChange = 0;
        
        if (hunger > 80) healthChange -= 10;
        if (happiness < 20) healthChange -= 5;
        if (cleanliness < 20) healthChange -= 5;
        if (energy < 20) healthChange -= 5;
        
        health = constrain(health + healthChange, 0, 100);
    }

    void updateState() {
        PetState newState = PetState::IDLE;
        
        // Determine new state based on attributes
        if (hunger > HUNGRY_THRESHOLD) {
            newState = PetState::HUNGRY;
        } else if (happiness < SAD_THRESHOLD) {
            newState = PetState::SAD;
        } else if (energy < TIRED_THRESHOLD) {
            newState = PetState::SLEEPING;
        } else if (cleanliness < DIRTY_THRESHOLD) {
            newState = PetState::SAD;
        } else if (happiness > 80) {
            newState = PetState::HAPPY;
        }
        
        setState(newState);
    }

    void setState(PetState newState) {
        if (newState != currentState) {
            currentState = newState;
            lastStateChangeTime = millis();
        }
    }
}; 