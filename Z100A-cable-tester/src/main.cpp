#include <Arduino.h>

#define LED_GREEN_PIN 2
#define LED_RED_PIN 3

#define CABLE_A1_PIN 6
#define CABLE_A2_PIN 7
#define CABLE_A3_PIN 8
#define CABLE_A4_PIN 9
#define CABLE_B1_PIN 15
#define CABLE_B2_PIN 14
#define CABLE_B3_PIN 16
#define CABLE_B4_PIN 10

#define NUMBER_OF_PINS 4

int cableA[NUMBER_OF_PINS] = {6, 7, 8, 9};
int cableB[NUMBER_OF_PINS] = {15, 14, 16, 10};

void setup() {
    // put your setup code here, to run once:
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    for (int i = 0; i < NUMBER_OF_PINS; i++) {
        pinMode(cableA[i], OUTPUT);
        pinMode(cableB[i], INPUT);
    }

    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(CABLE_A1_PIN, HIGH);
}

void reset_pins(void) {
    for (int i = 0; i < NUMBER_OF_PINS; i++) {
        digitalWrite(cableA[i], LOW);
    }
}

bool scan_pins(void) {
    for (int i = 0; i < NUMBER_OF_PINS; i++) {
        reset_pins();
        digitalWrite(cableA[i], HIGH);

        for (int j = 0; j < NUMBER_OF_PINS; j++) {
            if (i == j) {
                if (!digitalRead(cableB[j])) {
                    return false;
                }
            } else {
                if (digitalRead(cableB[j])) {
                    return false;
                }
            }
        }
    }

    return true;
}

void loop() {
    if (scan_pins()) {
        digitalWrite(LED_GREEN_PIN, HIGH);
        digitalWrite(LED_RED_PIN, LOW);
    } else {
        digitalWrite(LED_GREEN_PIN, LOW);
        digitalWrite(LED_RED_PIN, HIGH);
    }
}
