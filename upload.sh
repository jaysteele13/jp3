#!/bin/bash
# Upload script for ESP32
# Usage: ./upload.sh [sketch_directory]
# If no directory provided, uses current directory

DIR=${1:-"."}
if [ ! -d "$DIR" ]; then
    echo "Error: Directory '$DIR' does not exist."
    exit 1
fi

arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 --board-options FlashMode=dio "$DIR"