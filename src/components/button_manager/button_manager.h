#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "Arduino.h"

class ButtonManager {
private:
    static const int DEBOUNCE_DELAY = 10;  // 10ms for responsive yet reliable input
    int upPin;
    int downPin;
    int selectPin;
    int backPin;
    bool lastUpState;
    bool lastDownState;
    bool lastSelectState;
    bool lastBackState;
    unsigned long lastUpDebounceTime;
    unsigned long lastDownDebounceTime;
    unsigned long lastSelectDebounceTime;
    unsigned long lastBackDebounceTime;
    
    // Stable states after debouncing
    bool stableUpState;
    bool stableDownState;
    bool stableSelectState;
    bool stableBackState;
    
public:
    ButtonManager();
    void begin();
    bool checkDownPressed();
    bool checkUpPressed();
    bool checkSelectPressed();
    bool checkBackPressed();
};

#endif