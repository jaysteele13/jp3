#include "navigation_state.h"

ScreenType NavigationState::getScreenLevel(ScreenBase* screen) {
    if (!screen) {
        return ScreenType::UNKNOWN;
    }
    
    // No RTTI needed - use the screen's type identifier
    return screen->getScreenType();
}

ScreenType NavigationState::getNextLevel(ScreenType current) {
    switch (current) {
        case ScreenType::SECTION:
            return ScreenType::CATEGORY;
        case ScreenType::CATEGORY:
            return ScreenType::FOLDER;
        case ScreenType::FOLDER:
            return ScreenType::SONG;
        case ScreenType::SONG:
            return ScreenType::SONG;  // Can't go deeper than song
        default:
            return ScreenType::UNKNOWN;
    }
}

ScreenType NavigationState::getPreviousLevel(ScreenType current) {
    switch (current) {
        case ScreenType::SECTION:
            return ScreenType::SECTION;  // Can't go back from root
        case ScreenType::CATEGORY:
            return ScreenType::SECTION;
        case ScreenType::FOLDER:
            return ScreenType::CATEGORY;
        case ScreenType::SONG:
            return ScreenType::FOLDER;
        default:
            return ScreenType::UNKNOWN;
    }
}

bool NavigationState::isValidTransition(ScreenType from, ScreenType to) {
    ScreenType nextExpected = getNextLevel(from);
    ScreenType prevExpected = getPreviousLevel(from);
    
    return (to == nextExpected || to == prevExpected || to == from);
}

const char* NavigationState::screenTypeToString(ScreenType type) {
    switch (type) {
        case ScreenType::CATEGORY:
            return "Category";
        case ScreenType::SECTION:
            return "Section";
        case ScreenType::FOLDER:
            return "Folder";
        case ScreenType::SONG:
            return "Song";
        case ScreenType::UNKNOWN:
            return "Unknown";
        default:
            return "Unrecognized";
    }
}

