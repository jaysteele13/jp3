#ifndef SCREEN_TRANSITION_H
#define SCREEN_TRANSITION_H

#include "Arduino.h"

/**
 * Types of transitions between screens.
 * Used to determine animation style.
 */
enum class TransitionType {
    NONE,           // No animation
    SLIDE_LEFT,     // New screen slides in from right
    SLIDE_RIGHT,    // New screen slides in from left (usually for back)
    FADE,           // Cross-fade between screens
    INSTANT         // Immediate change (same as NONE)
};

/**
 * Manages the state of a screen transition/animation.
 * Handles timing and progress calculation for smooth, frame-rate-independent animations.
 */
struct ScreenTransition {
    TransitionType type;
    unsigned long startTime;        // millis() when transition started
    unsigned long duration;         // How long the transition should take (ms)
    
    /**
     * Check if animation is still in progress.
     */
    bool isAnimating() const {
        if (type == TransitionType::NONE || type == TransitionType::INSTANT) {
            return false;
        }
        return (millis() - startTime) < duration;
    }
    
    /**
     * Get animation progress as a float from 0.0 to 1.0.
     * Frame-rate independent - safe to use for lerp calculations.
     */
    float getProgress() const {
        if (duration == 0) return 1.0f;
        float p = (millis() - startTime) / (float)duration;
        return constrain(p, 0.0f, 1.0f);
    }
    
    /**
     * Get the reverse animation type (for back navigation).
     */
    TransitionType getReverseType() const {
        switch (type) {
            case TransitionType::SLIDE_LEFT:
                return TransitionType::SLIDE_RIGHT;
            case TransitionType::SLIDE_RIGHT:
                return TransitionType::SLIDE_LEFT;
            case TransitionType::FADE:
            case TransitionType::NONE:
            case TransitionType::INSTANT:
            default:
                return type;
        }
    }
};

#endif
