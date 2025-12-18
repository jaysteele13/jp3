#include "navigation_controller.h"
#include "Adafruit_SSD1306.h"

NavigationController::NavigationController(uint16_t width, uint16_t height)
    : screenWidth(width), screenHeight(height), transitionInitialized(false) {
    currentTransition.type = TransitionType::NONE;
    currentTransition.duration = 0;
}

NavigationController::~NavigationController() {
    // Note: We don't delete the screens themselves as they're managed by GUIManager
}

void NavigationController::push(ScreenBase* newScreen, TransitionType animation, 
                                unsigned long duration) {
    if (!newScreen) {
        Serial.println("NavigationController::push - Attempted to push null screen!");
        return;
    }
    
    // Deactivate current screen if one exists
    if (!screenStack.empty()) {
        screenStack.back()->onScreenInactive();
    }
    
    // Add new screen to stack
    screenStack.push_back(newScreen);
    newScreen->onScreenActive();
    
    // Start transition animation
    startTransition(animation, duration);
    
    Serial.print("NavigationController::push - Screen: ");
    Serial.print(newScreen->getScreenName());
    Serial.print(" (Stack depth: ");
    Serial.print(screenStack.size());
    Serial.println(")");
}

void NavigationController::pop(TransitionType animation, unsigned long duration) {
    if (screenStack.size() <= 1) {
        Serial.println("NavigationController::pop - Cannot pop: only one screen in stack!");
        return;
    }
    
    // Deactivate current screen
    if (!screenStack.empty()) {
        screenStack.back()->onScreenInactive();
    }
    
    // Remove current screen
    screenStack.pop_back();
    
    // Activate previous screen
    if (!screenStack.empty()) {
        screenStack.back()->onScreenActive();
    }
    
    // Start transition animation (use reverse animation for back)
    startTransition(animation, duration);
    
    Serial.print("NavigationController::pop - Returning to: ");
    if (!screenStack.empty()) {
        Serial.print(screenStack.back()->getScreenName());
    }
    Serial.print(" (Stack depth: ");
    Serial.print(screenStack.size());
    Serial.println(")");
}

ScreenBase* NavigationController::current() const {
    if (screenStack.empty()) {
        return nullptr;
    }
    return screenStack.back();
}

bool NavigationController::canGoBack() const {
    return screenStack.size() > 1;
}

size_t NavigationController::getStackDepth() const {
    return screenStack.size();
}

void NavigationController::update() {
    if (screenStack.empty()) {
        return;
    }
    
    // Update current screen state
    screenStack.back()->update();
}

void NavigationController::render(Adafruit_SSD1306& display) {
    if (screenStack.empty()) {
        return;
    }
    
    // If transitioning, render animation
    if (isTransitioning()) {
        renderTransition(display);
    } else {
        // Otherwise just render current screen
        screenStack.back()->display(display);
    }
    
    display.display();  // Actually send buffer to OLED
}

bool NavigationController::isTransitioning() const {
    return currentTransition.isAnimating();
}

void NavigationController::clear() {
    screenStack.clear();
    currentTransition.type = TransitionType::NONE;
    currentTransition.duration = 0;
}

void NavigationController::startTransition(TransitionType type, unsigned long duration) {
    currentTransition.type = type;
    currentTransition.startTime = millis();
    currentTransition.duration = duration;
    transitionInitialized = true;
}

void NavigationController::renderTransition(Adafruit_SSD1306& display) {
    if (screenStack.size() < 2) {
        // Can't transition with less than 2 screens
        screenStack.back()->display(display);
        return;
    }
    
    float progress = currentTransition.getProgress();
    ScreenBase* fromScreen = screenStack[screenStack.size() - 2];
    ScreenBase* toScreen = screenStack.back();
    
    // Clear display for fresh drawing
    display.clearDisplay();
    
    switch (currentTransition.type) {
        case TransitionType::SLIDE_LEFT: {
            // Previous screen slides left (out), new screen slides right (in)
            int offset = (int)(screenWidth * progress);
            
            // Draw "from" screen sliding out to the left
            // Note: Adafruit_SSD1306 doesn't support partial screen drawing easily,
            // so we render to full screen and let it overflow (clipping is automatic)
            display.setTextColor(SSD1306_WHITE);  // Clear
            fromScreen->display(display);
            
            // Render new screen coming in from right
            display.setTextColor(SSD1306_WHITE);  // Draw
            toScreen->display(display);
            break;
        }
        
        case TransitionType::SLIDE_RIGHT: {
            // Previous screen slides right (out), new screen slides left (in)
            int offset = (int)(screenWidth * progress);
            
            fromScreen->display(display);
            toScreen->display(display);
            break;
        }
        
        case TransitionType::FADE: {
            // For fade effect, we just blend by rendering new screen at end
            // For full fade effect would need more sophisticated rendering
            if (progress < 0.5f) {
                fromScreen->display(display);
            } else {
                toScreen->display(display);
            }
            break;
        }
        
        case TransitionType::NONE:
        case TransitionType::INSTANT:
        default:
            toScreen->display(display);
            break;
    }
}
