#include "button_manager.h"

ButtonManager::ButtonManager() {
    upPin = 26;
    downPin = 25;
    lastUpState = HIGH;
    lastDownState = HIGH;
    stableUpState = HIGH;
    stableDownState = HIGH;
    lastUpDebounceTime = 0;
    lastDownDebounceTime = 0;
}

void ButtonManager::begin() {
    Serial.println("Initializing button manager...");
    pinMode(upPin, INPUT_PULLUP);
    pinMode(downPin, INPUT_PULLUP);
    Serial.println("Button pins set to INPUT_PULLUP");
    Serial.println("=== TEST: Direct GPIO to Ground ===");
    Serial.println("Try touching GPIO25 directly to GND with a jumper wire");
    Serial.print("Up pin (26) initial state: ");
    Serial.println(digitalRead(upPin));
    Serial.print("Down pin (25) initial state: ");
    Serial.println(digitalRead(downPin));
}

bool ButtonManager::checkDownPressed() {
    bool currentState = digitalRead(downPin);
    
    Serial.print("Down pin reading: ");
    Serial.println(currentState);
    
    // If state changed, reset debounce timer
    if (currentState != lastDownState) {
        lastDownDebounceTime = millis();
        Serial.println("State changed - reset timer");
    }
    
    // Check if enough time has passed for debouncing
    if ((millis() - lastDownDebounceTime) > DEBOUNCE_DELAY) {
        // Only update stable state if it's different
        if (currentState != stableDownState) {
            stableDownState = currentState;
            
            // Return true only on HIGH to LOW transition (button pressed)
            if (stableDownState == LOW) {
                Serial.println("Down button PRESSED - returning true");
                return true;
            }
        }
    }
    
    // Update last state for next iteration
    lastDownState = currentState;
    return false;
};

bool ButtonManager::checkUpPressed() {
    Serial.println("=== checkUpPressed() called ===");
    bool currentState = digitalRead(upPin);
    
    Serial.print("Up pin reading: ");
    Serial.println(currentState);
    
    // If state changed, reset debounce timer
    if (currentState != lastUpState) {
        lastUpDebounceTime = millis();
        Serial.println("Up state changed - reset timer");
    }
    
    // Check if enough time has passed for debouncing
    if ((millis() - lastUpDebounceTime) > DEBOUNCE_DELAY) {
        // Only update stable state if it's different
        if (currentState != stableUpState) {
            stableUpState = currentState;
            
            // Return true only on HIGH to LOW transition (button pressed)
            if (stableUpState == LOW) {
                Serial.println("Up button PRESSED - returning true");
                return true;
            }
        }
    }
    
    // Update last state for next iteration
    lastUpState = currentState;
    Serial.println("Up button NOT pressed - returning false");
    return false;
}