#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "Arduino.h"

enum class ButtonState {
    RELEASED,
    PRESSED,
    HELD
};

class ButtonManager {
    public:
        ButtonManager();
        void begin();
        void update();
        
        bool isUpPressed();
        bool isDownPressed();
        bool isUpHeld();
        bool isDownHeld();
        ButtonState getUpButtonState();
        ButtonState getDownButtonState();

        struct Button {
            int pin;
            ButtonState state;
            ButtonState lastState;
            unsigned long lastDebounceTime;
            unsigned long pressStartTime;
            bool lastRawState;
        };
        
    private:
        static const int DEBOUNCE_DELAY = 50;
        static const int HOLD_THRESHOLD = 1000;
        void updateButton(Button &button);
        Button upButton;
        Button downButton;
   
        
    };

#endif