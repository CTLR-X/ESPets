# ESPets Project Roadmap

This roadmap outlines the key phases, milestones, and tasks for developing your ESPets desktop virtual pet device. It's designed to be iterative, allowing you to get a basic version working quickly and then build upon it.

## Phase 1: Core Hardware & Display Setup

**Goal:** Get the fundamental hardware components (ESP32-S3 board, display, touch) initialized and working.

### Milestone 1.1: Development Environment Ready

* \[ \] Install PlatformIO (recommended) or set up Arduino IDE for ESP32-S3.

* \[ \] Install necessary libraries: TFT_eSPI, XPT2046_Touchscreen (or similar touch driver), LittleFS.

### Milestone 1.2: Display Functionality

* \[ \] Configure TFT_eSPI User_Setup.h for the Guition ESP32-S3-4848S040 (parallel interface, correct pins, display controller).

* \[ \] Write a basic sketch to draw text and simple shapes (rectangles, circles) on the entire screen.

* \[ \] Test screen clearing and full-screen refreshes.

### Milestone 1.3: Touch Input Integration

* \[ \] Configure and initialize the XPT2046 touch controller.

* \[ \] Implement touch calibration routine (if needed for accurate input).

* \[ \] Write a sketch to read and print touch coordinates to the serial monitor.

* \[ \] Implement a simple visual feedback for touch (e.g., draw a dot where touched).

## Phase 2: Basic Pet Logic & Graphics

**Goal:** Implement the core pet states, basic animations, and integrate touch for simple interactions.

### Milestone 2.1: Pet Data Structure

* \[ \] Define a Pet class or struct with essential attributes (e.g., hunger, happiness, health, age, current_state).

* \[ \] Implement methods to update these attributes over time.

### Milestone 2.2: Sprite Management

* \[ \] Create initial pixel art sprites for your pet (e.g., idle, happy, sad, eating).

* \[ \] Convert sprites to a suitable format (e.g., RGB565 raw bitmaps, or optimized PNGs for LittleFS).

* \[ \] Implement functions to load and draw individual sprites from flash memory onto the display.

### Milestone 2.3: Basic Animations

* \[ \] Implement a simple frame-by-frame animation system for the pet (e.g., an idle breathing animation).

* \[ \] Integrate animation playback into the main loop.

### Milestone 2.4: First Touch Interactions

* \[ \] Define touchable areas on the screen (e.g., pet's body, food bowl icon).

* \[ \] Implement logic to detect touches within these areas.

* \[ \] Link touch events to basic pet actions (e.g., touching the pet increases happiness, touching a food icon reduces hunger).

## Phase 3: Core Game Loop & Persistence

**Goal:** Establish the main game loop, ensure pet needs decay over time, and implement saving/loading of pet state.

### Milestone 3.1: Time-Based Pet Needs

* \[ \] Implement a robust time management system (using `millis()` or an RTC if desired) to track elapsed time.

* \[ \] Periodically update pet attributes (e.g., hunger increases every 5 minutes, happiness decreases over time).

* \[ \] Trigger state changes based on attribute thresholds (e.g., hunger > X changes pet state to hungry).

### Milestone 3.2: Game State Persistence

* \[ \] Implement functions to save the pet's current state (attributes, current time) to LittleFS.

* \[ \] Implement functions to load the pet's state from LittleFS on startup.

* \[ \] Ensure data is saved periodically (e.g., every few minutes) and gracefully on power loss (if possible, or via a "save" touch button).

### Milestone 3.3: Basic User Interface (UI)

* \[ \] Design and draw simple UI elements for displaying pet stats (e.g., hunger bar, happiness meter).

* \[ \] Create touchable icons for basic actions (e.g., "Feed," "Play," "Clean").

* \[ \] Implement simple menu navigation using touch.

## Phase 4: Enhanced Interaction & Features

**Goal:** Add more depth to interactions, sound, and refine the overall experience.

### Milestone 4.1: Sound Integration

* \[ \] Connect and test the I2C audio DAC on the board for sound output. This will require soldering to move a resistor to access the DAC.

* \[ \] Create or source simple sound effects (e.g., eating sound, happy sound, alert sound).

* \[ \] Trigger sounds based on pet actions and UI interactions.

### Milestone 4.2: Advanced Animations & Backgrounds

* \[ \] Create more complex animation sequences for various pet states (e.g., sleeping, playing mini-game, evolving).

* \[ \] Implement dynamic backgrounds that change with time of day or pet's mood.

* \[ \] Optimize partial screen updates for smoother transitions and less flicker.

### Milestone 4.3: Mini-Game (Optional but Recommended)

* \[ \] Design and implement a simple touch-based mini-game (e.g., a reaction game, a simple puzzle).

* \[ \] Integrate the mini-game into the pet's happiness/discipline stats.

### Milestone 4.4: Enclosure Design

* \[ \] Design a 3D printable enclosure that perfectly fits the Guition board and complements a desktop setup.

* \[ \] Print and assemble the first prototype enclosure.

## Phase 5: Polish & Future Enhancements

**Goal:** Refine the user experience, optimize performance, and explore advanced functionalities.

### Milestone 5.1: Performance Optimization

* \[ \] Profile code to identify bottlenecks and optimize drawing routines.

* \[ \] Ensure efficient use of ESP32-S3's SIMD instructions if applicable for specific graphics operations.

### Milestone 5.2: Advanced Pet AI/Behavior

* \[ \] Implement more nuanced pet reactions and behaviors based on multiple factors.

* \[ \] Add a "discipline" or "training" mechanic.

* \[ \] Implement pet evolution stages based on age and care.

### Milestone 5.3: Connectivity Features (Optional)

* \[ \] Integrate Wi-Fi for time synchronization (NTP).

* \[ \] (Advanced) Explore basic online features (e.g., sharing pet stats, simple interactions with other ESPets devices).

### Milestone 5.4: Final Polish & Documentation

* \[ \] Thoroughly test all features and interactions.

* \[ \] Refine UI/UX for a polished feel.

* \[ \] Complete comprehensive documentation in the `docs/` folder (wiring, software architecture, usage guide).
