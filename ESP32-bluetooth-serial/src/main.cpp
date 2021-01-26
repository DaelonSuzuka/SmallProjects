#include "BluetoothSerial.h"
#include <HardwareSerial.h>
#include <Arduino.h>

BluetoothSerial BTserial;
HardwareSerial HWserial(1);

void setup() {
    Serial.begin(9600);
    HWserial.begin(9600, SERIAL_8N1, 26, 27);

    BTserial.begin("H-100 Serial Bridge");
}

void loop() {
    char c = 0;
    if (BTserial.available()) {
        c = BTserial.read();
        Serial.printf("BT: %c\r\n", c);
        HWserial.printf("%c", c);
    }

    if (HWserial.available()) {
        c = HWserial.read();
        Serial.printf("HW: %c\r\n", c);
        BTserial.printf("%c", c);
    }
    delay(20);
}