# jp3
This is the beginning of my MP3 player project.

This will include me planning, developing, workshopping ideas, soldering, designing PCB and 3D printed parts.

I will create a master prompt and initial direcotry structure to adhere to better coding standards. The aim of this practice is to maintain code readability. Unit testing will be considered.

This may also include asynchronous threading.

## How to Compile and Load Sketches

The chip I am using is an esp32 dev board from Ali Express. The FQBN in esp32/esspressif is `esp32:esp32:esp32`.
To get this working on my computer I needed to ensure drivers for `CP2102` were properly configured.

As I am not using the Arduino IDE. I must leverage the `Arduino-CLI` tool. I have scripts in order to **compile** and **upload** my code to the ESP32.

#### For Uploading
`FlashMode` must be '**dio**' otherwise the frequency won't match up and uploading will fail:

```bash
--board-options FlashMode=dio
```
