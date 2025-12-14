#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

class GUIManager {
private:
    Adafruit_SSD1306* display;
    
public:
    GUIManager();
    bool begin();
    void clear();
    void update();
    void setText(int x, int y, const char* text);
    void showSplashScreen();
};

#endif