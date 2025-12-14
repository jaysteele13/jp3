#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"

GUIManager gui;

void setup() {
  gui.begin();
  Song testSong("Test Song", 240, "Test Album", "Test Artist", "Test Playlist");
  gui.displaySong(testSong);
}

void loop() {
  delay(1000);
}




// THIS WORKS FOR TESTING THE OLED DISPLAY ADDRESS
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET -1
// #define SCREEN_ADDRESS 0x3C

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// void setup() {
//   Serial.begin(115200);
//   Wire.begin(21,22); // or whichever pins worked
//   if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
//     Serial.println("SSD1306 allocation failed");
//     for(;;);
//   }
//   display.clearDisplay();
//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(0,0);
//   display.println("Hello OLED");
//   display.display();
// }

// void loop() {}

