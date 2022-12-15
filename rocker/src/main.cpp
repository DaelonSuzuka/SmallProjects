#include "judi.hpp"
#include <Arduino.h>

/* ************************************************************************** */

#define OUTPUT0_PIN 8
#define OUTPUT1_PIN 9

#define INPUT0_PIN A1
#define INPUT1_PIN A2

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

#define NUMBER_OF_CHANNELS 2
#define UPDATE_THRESHOLD 10
int values[NUMBER_OF_CHANNELS] = {0, 0};
int lastUpdate[NUMBER_OF_CHANNELS] = {0, 0};

void check_inputs(void) {
    values[0] = ((values[0] * 7) + analogRead(INPUT0_PIN)) / 8;
    values[1] = ((values[1] * 7) + analogRead(INPUT1_PIN)) / 8;
}

String valueChangedMsg[NUMBER_OF_CHANNELS] = {
    "{'update':{'value_changed':{'channel':'0', 'value':",
    "{'update':{'value_changed':{'channel':'1', 'value':",
};

String msgTail = "\"}}}";

void send_update(void) {
    for (int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        if (abs(values[i] - lastUpdate[i]) > UPDATE_THRESHOLD) {
            Serial.print(valueChangedMsg[i]);
            Serial.print(values[i]);
            Serial.print(msgTail);
            lastUpdate[i] = values[i];
        }
    }
}

/* ************************************************************************** */

void setup() {
    pinMode(OUTPUT0_PIN, OUTPUT);
    digitalWrite(OUTPUT0_PIN, HIGH);
    pinMode(OUTPUT1_PIN, OUTPUT);
    digitalWrite(OUTPUT1_PIN, HIGH);

    pinMode(INPUT0_PIN, INPUT);
    pinMode(INPUT1_PIN, INPUT);

    // fix string quotes
    device_info.replace("'", "\"");
    for (int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        valueChangedMsg[i].replace("'", "\"");
    }

    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        check_comms(Serial.read());
    }

    check_inputs();
    send_update();
}