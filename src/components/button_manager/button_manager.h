#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "Arduino.h"

class ButtonManager {
private:
    static const int DEBOUNCE_DELAY = 50;
    int upPin;
    int downPin;
    bool lastUpState;
    bool lastDownState;
    unsigned long lastDebounceTime;
    
public:
    ButtonManager();
    void begin();
    bool checkDownPressed();
    bool checkUpPressed();
};

#endif