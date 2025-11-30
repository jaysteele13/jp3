#!/bin/bash
# Compile script for ESP32
# Usage: ./compile.sh [sketch_directory]
# If no directory provided, uses current directory

DIR=${1:-"."}
if [ ! -d "$DIR" ]; then
    echo "Error: Directory '$DIR' does not exist."
    exit 1
fi

arduino-cli compile --fqbn esp32:esp32:esp32 "$DIR"