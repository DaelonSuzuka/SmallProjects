#include "BluetoothSerial.h"
#include <Arduino.h>

BluetoothSerial BTserial;

void setup() {
    Serial.begin(9600);
    BTserial.begin("H-100 Serial Bridge");
}

void loop() {
    if (BTserial.available()) {
        Serial.printf("%c", BTserial.read());
    }

    if (Serial.available()) {
        BTserial.printf("%c", Serial.read());
    }
    delay(20);
}