#include <Arduino.h>
#include <BleGamepad.h>

BleGamepad bleGamepad;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    bleGamepad.begin();
}

void loop() {
    if (bleGamepad.isConnected()) {
        Serial.println("Press buttons 1 and 32. Move all axes to max. Set DPAD "
                       "to down right.");
        bleGamepad.press(BUTTON_1);
        bleGamepad.press(BUTTON_32);
        bleGamepad.setAxes(32767, 32767, 32767, 32767, 255, 255,
                           DPAD_DOWN_RIGHT);
        delay(500);

        Serial.println(
            "Release button 1. Move all axes to min. Set DPAD to centred.");
        bleGamepad.release(BUTTON_1);
        bleGamepad.setAxes(-32767, -32767, -32767, -32767, 0, 0, DPAD_CENTERED);
        delay(500);
    }
}