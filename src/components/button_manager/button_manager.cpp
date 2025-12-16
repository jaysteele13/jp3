#include "button_manager.h"

ButtonManager::ButtonManager() {
    upPin = 26;
    downPin = 25;
    lastUpState = HIGH;
    lastDownState = HIGH;
    lastDebounceTime = 0;
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
    // Read multiple times to ensure stable reading
    bool reading1 = digitalRead(downPin);
    delay(5);
    bool reading2 = digitalRead(downPin);
    delay(5);
    bool reading3 = digitalRead(downPin);
    
    // Consider it pressed if at least 2 out of 3 readings are LOW
    bool currentState = (reading1 + reading2 + reading3) <= 1;
    
    if (currentState == LOW && lastDownState == HIGH) {
        lastDownState = currentState;
        return true;
    }
    
    lastDownState = currentState;
    return false;
}


bool ButtonManager::checkUpPressed() {
    bool currentState = digitalRead(upPin);
    
    if (currentState == LOW && lastUpState == HIGH) {
        lastUpState = currentState;
        return true;
    }
    
    lastUpState = currentState;
    return false;
}