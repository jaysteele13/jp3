#ifndef SCREEN_BASE_H
#define SCREEN_BASE_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"

// Forward declaration
class ButtonManager;

/**
 * Screen type enum for type identification without RTTI.
 * Each screen class implements getScreenType() to return its type.
 * This avoids the memory overhead of dynamic_cast/RTTI on embedded systems.
 */
enum class ScreenType {
    CATEGORY,
    SECTION,
    FOLDER,
    SONG,
    UNKNOWN
};

/**
 * Abstract base class for all screens in the navigation system.
 * Each screen handles its own rendering, input, and state updates.
 */
class ScreenBase {
public:
    virtual ~ScreenBase() = default;
    
    /**
     * Render the screen to the display.
     * Called during each update cycle.
     */
    virtual void display(Adafruit_SSD1306& display) = 0;
    
    /**
     * Handle user input for this screen.
     * Should be quick and non-blocking.
     */
    virtual void handleInput(ButtonManager& buttons) = 0;
    
    /**
     * Update any non-rendering state (timers, scrolling text, etc).
     * Called during each update cycle.
     */
    virtual void update() {}
    
    /**
     * Called when this screen becomes active.
     * Useful for initialization/state reset.
     */
    virtual void onScreenActive() {}
    
    /**
     * Called when this screen is no longer active.
     * Useful for cleanup.
     */
    virtual void onScreenInactive() {}
    
    /**
     * Get the type of this screen for navigation purposes.
     * Returns the screen type without requiring RTTI.
     * Each derived class must implement this.
     */
    virtual ScreenType getScreenType() const = 0;
    
    /**
     * Get screen identifier for debugging.
     */
    virtual String getScreenName() const = 0;
};

#endif
