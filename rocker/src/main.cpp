#include <Arduino.h>
#include "judi.hpp"

/* ************************************************************************** */

#define OUTPUT_PIN 3
#define INPUT_PIN A6

/* ************************************************************************** */

JUDI judi("Rocker", "12345");

String device_info = "{'update':{'device_info':{'product_name':'Rocker','serial_number':'12345'}}}";

void check_comms(char currentChar) {
    if (judi.update(currentChar)) {
        if (judi["request"] == "device_info") {
            // serializeJson(judi.device_info, Serial);

            Serial.print(device_info);
        }
        judi.reset();
    }
}

/* -------------------------------------------------------------------------- */

int value = 0;

void check_buttons(void) {
    // static unsigned long lastCheckTime = 0;
    // if (millis() - lastCheckTime < 20) {
    //     return;
    // }
    // lastCheckTime = millis();
    
    value = ((value * 7) + analogRead(INPUT_PIN)) / 8;
}

void report_value(void) {
    static unsigned long lastCheckTime = 0;
    if (millis() - lastCheckTime < 1000) {
        return;
    }
    lastCheckTime = millis();

    Serial.println(value);
}

/* ************************************************************************** */

void setup() {
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, HIGH);

    pinMode(INPUT_PIN, INPUT);

    // fix string quotes
    device_info.replace("'", "\"");

    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        check_comms(Serial.read());
    }

    check_buttons();
    report_value();
}