#ifndef NAVIGATION_STATE_H
#define NAVIGATION_STATE_H

#include "Arduino.h"
#include "screen_base.h"

/**
 * Encapsulates the navigation flow logic.
 * Replaces depth-based hard-coded checks with a proper state machine.
 * 
 * Uses screen type identification (no RTTI required).
 * Defines the hierarchy: Category -> Section -> Folder -> Song
 */
class NavigationState {
public:
    /**
     * Determine the navigation level from a screen pointer.
     * Uses the screen's getScreenType() method (no RTTI needed).
     */
    static ScreenType getScreenLevel(ScreenBase* screen);
    
    /**
     * Get the next logical level in the hierarchy.
     * Used for forward navigation.
     */
    static ScreenType getNextLevel(ScreenType current);
    
    /**
     * Get the previous logical level in the hierarchy.
     * Used for back navigation.
     */
    static ScreenType getPreviousLevel(ScreenType current);
    
    /**
     * Convert ScreenType to human-readable string.
     */
    static const char* screenTypeToString(ScreenType type);
};

#endif

