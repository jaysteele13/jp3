## Here is the place I try out the gear.

### ESP32

`**30/11/2025 - Blink Works**`
I made an environent to compile and upload code. I get the second LED to blink to show that the ESP32 compiled the code correctly. The on memory is low. We will see how much of a challenge this will pose in the future to utilise all components.

    plan for next time:
    - Power esp32 via power rails
    - Create environment and branches for OLED connectivity
    - Create small GUI system. Plan ahead for button usage. Song Names, Albums, animations, as well as battery life


### PCM5102A


### SD Card Reader



### OLED Screen

Find Drivers and Config for OLED Screen: SSD1306 I2C IIC SPI  SERIAL 128X64.

- Has GND, VDD, SCK, SDA. GND and VDD must be ground and 3.3v input. The other two are analoge. I need to use SDA and SCL for data and clock. These are pins: 
    - GPIO21 (SDA)
    - GPIO22 (SCL)

Now connected to ESP32 to prototype.

Now the plan is to download this driver and available library and display something on the screen. We will then working on getting text, then scrolling text. Then animations...

When Connected to Mb102. The voltage is less than either 5v or 3.3v So this can be used to power both alone. I need to buck boost, minimize voltage to be solid 3.3v. For now prototype with micro usb plaug.



### Buttons and Switches


