#include "judi.hpp"

/* ************************************************************************** */

JUDI judi("judipedals", "6969696969");

void check_comms(char currentChar) {
    if (judi.update(currentChar)) {
        if (judi["request"] == "device_info") {
            serializeJson(judi.device_info, Serial);
        }
        judi.reset();
    }
}

/* -------------------------------------------------------------------------- */

#define NUMBER_OF_BUTTONS 4
const int pinToButtonOffset = 2;
int prevState[NUMBER_OF_BUTTONS] = {0, 0, 0, 0};

String buttonPressedMsg[NUMBER_OF_BUTTONS] = {
    "{'update':{'button_pressed':'1'}}",
    "{'update':{'button_pressed':'2'}}",
    "{'update':{'button_pressed':'3'}}",
    "{'update':{'button_pressed':'4'}}",
};

String buttonReleasedMsg[NUMBER_OF_BUTTONS] = {
    "{'update':{'button_released':'1'}}",
    "{'update':{'button_released':'2'}}",
    "{'update':{'button_released':'3'}}",
    "{'update':{'button_released':'4'}}",
};

void fix_strings(void) {
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
        buttonPressedMsg[i].replace("'", "\"");
        buttonReleasedMsg[i].replace("'", "\"");
    }
}

void check_buttons(void) {
    static unsigned long lastCheckTime = 0;
    if (millis() - lastCheckTime < 10) {
        return;
    }
    lastCheckTime = millis();

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
        int currentState = !digitalRead(i + pinToButtonOffset);
        if (currentState != prevState[i]) {
            prevState[i] = currentState;
            if (currentState) {
                Serial.print(buttonPressedMsg[i]);
            } else {
                Serial.print(buttonReleasedMsg[i]);
            }
        }
    }
}

/* ************************************************************************** */

void setup() {
    Serial.begin(9600);
    while (!Serial)
        continue;

    fix_strings();

    // Initialize Button Pins
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
}

void loop() {
    if (Serial.available()) {
        check_comms(Serial.read());
    }

    check_buttons();
}
