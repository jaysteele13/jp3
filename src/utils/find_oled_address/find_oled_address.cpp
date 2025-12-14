#include <Wire.h>
#include "find_oled_address.h"

void FindOLEDAddress::find() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial);
  delay(1000);   
  Wire.begin(/*SDA=*/21, /*SCL=*/22);    
  
  Serial.print("scan test");
  for(byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      delay(1000);
      Serial.print("Found I2C device at 0x");
      Serial.println(address, HEX);
    }
  }
} 