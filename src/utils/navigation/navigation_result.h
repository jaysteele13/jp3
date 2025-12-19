#ifndef NAVIGATION_RESULT_H
#define NAVIGATION_RESULT_H

/**
 * Result codes for navigation operations.
 * Allows callers to know if navigation succeeded or why it failed.
 */
enum class NavResult {
    SUCCESS,                    // Navigation completed successfully
    NULL_SCREEN,               // Attempted to navigate to null screen
    ALREADY_AT_ROOT,           // Attempted to pop from root screen
    STACK_OVERFLOW,            // Stack exceeded maximum depth
    INVALID_TRANSITION,        // Transition parameters were invalid
    FAILED                     // Generic failure
};

/**
 * Convert NavResult to human-readable string for debugging
 */
inline const char* navResultToString(NavResult result) {
    switch (result) {
        case NavResult::SUCCESS:
            return "SUCCESS";
        case NavResult::NULL_SCREEN:
            return "NULL_SCREEN";
        case NavResult::ALREADY_AT_ROOT:
            return "ALREADY_AT_ROOT";
        case NavResult::STACK_OVERFLOW:
            return "STACK_OVERFLOW";
        case NavResult::INVALID_TRANSITION:
            return "INVALID_TRANSITION";
        case NavResult::FAILED:
            return "FAILED";
        default:
            return "UNKNOWN";
    }
}

#endif
