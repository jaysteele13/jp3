#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "Arduino.h"

class ButtonManager {
private:
    static const int DEBOUNCE_DELAY = 10;  // 10ms for responsive yet reliable input
    int upPin;
    int downPin;
    bool lastUpState;
    bool lastDownState;
    unsigned long lastUpDebounceTime;
    unsigned long lastDownDebounceTime;
    
    // Stable states after debouncing
    bool stableUpState;
    bool stableDownState;
    
public:
    ButtonManager();
    void begin();
    bool checkDownPressed();
    bool checkUpPressed();
};

#endif