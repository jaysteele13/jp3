#include "button_manager.h"

ButtonManager::ButtonManager() {
    upButton = {26, ButtonState::RELEASED, ButtonState::RELEASED, 0, 0, false};
    downButton = {25, ButtonState::RELEASED, ButtonState::RELEASED, 0, 0, false};
}

void ButtonManager::begin() {
    pinMode(upButton.pin, INPUT_PULLUP);
    pinMode(downButton.pin, INPUT_PULLUP);
}

void ButtonManager::update() {
    updateButton(upButton);
    updateButton(downButton);
}

void ButtonManager::updateButton(Button &button) {
    bool currentRawState = !digitalRead(button.pin);
    unsigned long currentTime = millis();
    
    if (currentRawState != button.lastRawState) {
        button.lastDebounceTime = currentTime;
    }
    
    if ((currentTime - button.lastDebounceTime) > DEBOUNCE_DELAY) {
        if (currentRawState && button.state == ButtonState::RELEASED) {
            button.state = ButtonState::PRESSED;
            button.pressStartTime = currentTime;
        } else if (currentRawState && button.state == ButtonState::PRESSED) {
            if ((currentTime - button.pressStartTime) > HOLD_THRESHOLD) {
                button.state = ButtonState::HELD;
            }
        } else if (!currentRawState) {
            button.state = ButtonState::RELEASED;
        }
    }
    
    button.lastRawState = currentRawState;
}

bool ButtonManager::isUpPressed() {
    return upButton.state == ButtonState::PRESSED;
}

bool ButtonManager::isDownPressed() {
    return downButton.state == ButtonState::PRESSED;
}

bool ButtonManager::isUpHeld() {
    return upButton.state == ButtonState::HELD;
}

bool ButtonManager::isDownHeld() {
    return downButton.state == ButtonState::HELD;
}

ButtonState ButtonManager::getUpButtonState() {
    return upButton.state;
}

ButtonState ButtonManager::getDownButtonState() {
    return downButton.state;
}

