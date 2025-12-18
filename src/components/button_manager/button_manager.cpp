#include "button_manager.h"

ButtonManager::ButtonManager() {
    upPin = 26;
    downPin = 25;
    selectPin = 27;
    backPin = 32;
    lastUpState = HIGH;
    lastDownState = HIGH;
    lastSelectState = HIGH;
    lastBackState = HIGH;
    stableUpState = HIGH;
    stableDownState = HIGH;
    stableSelectState = HIGH;
    stableBackState = HIGH;
    lastUpDebounceTime = 0;
    lastDownDebounceTime = 0;
    lastSelectDebounceTime = 0;
    lastBackDebounceTime = 0;
}

void ButtonManager::begin() {
    Serial.println("Initializing button manager...");
    pinMode(upPin, INPUT_PULLUP);
    pinMode(downPin, INPUT_PULLUP);
    pinMode(selectPin, INPUT_PULLUP);
    pinMode(backPin, INPUT_PULLUP);
    Serial.println("Button pins set to INPUT_PULLUP");
    Serial.println("=== Button Configuration ===");
    Serial.println("GPIO26 (Up), GPIO25 (Down), GPIO27 (Select), GPIO32 (Back)");
    Serial.print("Up pin (26) initial state: ");
    Serial.println(digitalRead(upPin));
    Serial.print("Down pin (25) initial state: ");
    Serial.println(digitalRead(downPin));
    Serial.print("Select pin (27) initial state: ");
    Serial.println(digitalRead(selectPin));
    Serial.print("Back pin (32) initial state: ");
    Serial.println(digitalRead(backPin));
}

bool ButtonManager::checkDownPressed() {
    bool currentState = digitalRead(downPin);
    
    // If state changed, reset debounce timer
    if (currentState != lastDownState) {
        lastDownDebounceTime = millis();
    }
    
    // Check if enough time has passed for debouncing
    if ((millis() - lastDownDebounceTime) > DEBOUNCE_DELAY) {
        // Only update stable state if it's different
        if (currentState != stableDownState) {
            stableDownState = currentState;
            
            // Return true only on HIGH to LOW transition (button pressed)
            if (stableDownState == LOW) {
                return true;
            }
        }
    }
    
    // Update last state for next iteration
    lastDownState = currentState;
    return false;
};

bool ButtonManager::checkUpPressed() {
    bool currentState = digitalRead(upPin);
    
    // If state changed, reset debounce timer
    if (currentState != lastUpState) {
        lastUpDebounceTime = millis();
    }
    
    // Check if enough time has passed for debouncing
    if ((millis() - lastUpDebounceTime) > DEBOUNCE_DELAY) {
        // Only update stable state if it's different
        if (currentState != stableUpState) {
            stableUpState = currentState;
            
            // Return true only on HIGH to LOW transition (button pressed)
            if (stableUpState == LOW) {
                return true;
            }
        }
    }
    
    // Update last state for next iteration
    lastUpState = currentState;
    return false;
}

bool ButtonManager::checkSelectPressed() {
    bool currentState = digitalRead(selectPin);
    
    // If state changed, reset debounce timer
    if (currentState != lastSelectState) {
        lastSelectDebounceTime = millis();
    }
    
    // Check if enough time has passed for debouncing
    if ((millis() - lastSelectDebounceTime) > DEBOUNCE_DELAY) {
        // Only update stable state if it's different
        if (currentState != stableSelectState) {
            stableSelectState = currentState;
            
            // Return true only on HIGH to LOW transition (button pressed)
            if (stableSelectState == LOW) {
                return true;
            }
        }
    }
    
    // Update last state for next iteration
    lastSelectState = currentState;
    return false;
}

bool ButtonManager::checkBackPressed() {
    bool currentState = digitalRead(backPin);
    
    // If state changed, reset debounce timer
    if (currentState != lastBackState) {
        lastBackDebounceTime = millis();
    }
    
    // Check if enough time has passed for debouncing
    if ((millis() - lastBackDebounceTime) > DEBOUNCE_DELAY) {
        // Only update stable state if it's different
        if (currentState != stableBackState) {
            stableBackState = currentState;
            
            // Return true only on HIGH to LOW transition (button pressed)
            if (stableBackState == LOW) {
                return true;
            }
        }
    }
    
    // Update last state for next iteration
    lastBackState = currentState;
    return false;
}