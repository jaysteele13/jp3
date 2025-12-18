#include "navigation_controller.h"
#include "Adafruit_SSD1306.h"

NavigationController::NavigationController(uint16_t width, uint16_t height)
    : screenWidth(width), screenHeight(height), transitionInitialized(false) {
    currentTransition.type = TransitionType::NONE;
    currentTransition.duration = 0;
}

NavigationController::~NavigationController() {
    // NOTE: We don't delete screens - we don't own them
    // Caller is responsible for screen lifecycle
}

NavResult NavigationController::push(ScreenBase* newScreen, TransitionType animation, 
                                     unsigned long duration) {
    if (!newScreen) {
        Serial.println("ERROR: NavigationController::push - Null screen!");
        return NavResult::NULL_SCREEN;
    }
    
    if (screenStack.size() >= MAX_STACK_DEPTH) {
        Serial.println("ERROR: NavigationController::push - Stack overflow!");
        return NavResult::STACK_OVERFLOW;
    }
    
    if (!screenStack.empty()) {
        screenStack.back()->onScreenInactive();
    }
    
    screenStack.push_back(newScreen);
    newScreen->onScreenActive();
    startTransition(animation, duration);
    
    Serial.print("NAV: Push - ");
    Serial.print(newScreen->getScreenName());
    Serial.print(" (Depth:");
    Serial.print(screenStack.size());
    Serial.println(")");
    
    return NavResult::SUCCESS;
}

NavResult NavigationController::pop(TransitionType animation, unsigned long duration) {
    if (screenStack.size() <= 1) {
        Serial.println("ERROR: NavigationController::pop - At root!");
        return NavResult::ALREADY_AT_ROOT;
    }
    
    if (!screenStack.empty()) {
        screenStack.back()->onScreenInactive();
    }
    
    screenStack.pop_back();
    
    if (!screenStack.empty()) {
        screenStack.back()->onScreenActive();
    }
    
    startTransition(animation, duration);
    
    Serial.print("NAV: Pop - Back to ");
    if (!screenStack.empty()) {
        Serial.print(screenStack.back()->getScreenName());
    }
    Serial.print(" (Depth:");
    Serial.print(screenStack.size());
    Serial.println(")");
    
    return NavResult::SUCCESS;
}

ScreenBase* NavigationController::current() const {
    return screenStack.empty() ? nullptr : screenStack.back();
}

bool NavigationController::canGoBack() const {
    return screenStack.size() > 1;
}

size_t NavigationController::getStackDepth() const {
    return screenStack.size();
}

void NavigationController::update() {
    if (!screenStack.empty()) {
        screenStack.back()->update();
    }
}

void NavigationController::render(Adafruit_SSD1306& display) {
    if (screenStack.empty()) {
        return;
    }
    
    if (isTransitioning()) {
        renderTransition(display);
    } else {
        screenStack.back()->display(display);
    }
    
    display.display();
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
        screenStack.back()->display(display);
        return;
    }
    
    display.clearDisplay();
    
    // Only INSTANT animation works reliably on small OLED
    // SLIDE and FADE require capabilities this display doesn't have
    screenStack.back()->display(display);
}

