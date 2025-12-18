#ifndef NAVIGATION_CONTROLLER_H
#define NAVIGATION_CONTROLLER_H

#include "Arduino.h"
#include "screen_base.h"
#include "screen_transition.h"
#include <vector>

/**
 * Manages screen navigation with a stack-based system.
 * Supports forward/back navigation with smooth animations.
 * 
 * Usage:
 *   navigator.push(newScreen, TransitionType::SLIDE_LEFT);
 *   navigator.pop(TransitionType::SLIDE_RIGHT);
 *   navigator.update();  // Call in main loop
 *   navigator.render(display);  // Call to render current state
 */
class NavigationController {
private:
    std::vector<ScreenBase*> screenStack;
    ScreenTransition currentTransition;
    bool transitionInitialized;
    
    // Display dimensions for animations
    uint16_t screenWidth;
    uint16_t screenHeight;
    
    /**
     * Internal helper to start a new transition.
     */
    void startTransition(TransitionType type, unsigned long duration);
    
    /**
     * Render transition animation between two screens.
     */
    void renderTransition(Adafruit_SSD1306& display);
    
public:
    NavigationController(uint16_t width = 128, uint16_t height = 64);
    ~NavigationController();
    
    /**
     * Push a new screen onto the stack.
     * The new screen becomes active and will be rendered.
     * Animation duration defaults to 300ms.
     */
    void push(ScreenBase* newScreen, TransitionType animation = TransitionType::SLIDE_LEFT, 
              unsigned long duration = 300);
    
    /**
     * Pop the current screen off the stack, returning to previous screen.
     * Automatically reverses the animation direction (SLIDE_LEFT becomes SLIDE_RIGHT).
     * Animation duration defaults to 300ms.
     */
    void pop(TransitionType animation = TransitionType::SLIDE_RIGHT, 
             unsigned long duration = 300);
    
    /**
     * Get the currently active screen without removing it.
     */
    ScreenBase* current() const;
    
    /**
     * Check if we can go back (stack has more than 1 screen).
     */
    bool canGoBack() const;
    
    /**
     * Get the number of screens in the stack.
     */
    size_t getStackDepth() const;
    
    /**
     * Update all screen states (non-rendering updates).
     * Should be called once per main loop iteration.
     */
    void update();
    
    /**
     * Render the current state to the display.
     * Handles both static screens and animations.
     * Should be called once per main loop iteration.
     */
    void render(Adafruit_SSD1306& display);
    
    /**
     * Check if any animation is currently playing.
     */
    bool isTransitioning() const;
    
    /**
     * Force clear all screens from stack.
     * Useful for resetting to a known state.
     */
    void clear();
};

#endif
