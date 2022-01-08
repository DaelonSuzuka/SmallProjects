#include "judi.hpp"
#include <ArduinoUniqueID.h>

/* ************************************************************************** */

// {"request": "device_info"}

String get_id(void) {
    String id;
    // std::stringstream buffer("");
    for (size_t i = 0; i < UniqueIDsize; i++) {
        char buffer[10];
        sprintf(buffer, "%X", UniqueID[i]);
        id += String(buffer);
        // id += UniqueID[i];
    }
    return id;
}

JUDI judi("Stomp 5", get_id());

String device_info1 = "{'update':{'device_info':{'product_name':'Stomp 5','serial_number':'";
String device_info2 = "'}}}";

void check_comms(char currentChar) {
    if (judi.update(currentChar)) {
        if (judi["request"] == "device_info") {
            // serializeJson(judi.device_info, Serial);

            Serial.print(device_info1);
            Serial.print(get_id());
            Serial.print(device_info2);
        }
        judi.reset();
    }
}

/* -------------------------------------------------------------------------- */

#define NUMBER_OF_BUTTONS 5
const int pinToButtonOffset = 0;
int prevState[NUMBER_OF_BUTTONS] = {};

String buttonPressedMsg[NUMBER_OF_BUTTONS] = {
    "{'update':{'button_pressed':'1'}}", "{'update':{'button_pressed':'2'}}",
    "{'update':{'button_pressed':'3'}}", "{'update':{'button_pressed':'4'}}",
    "{'update':{'button_pressed':'5'}}",
};

String buttonReleasedMsg[NUMBER_OF_BUTTONS] = {
    "{'update':{'button_released':'1'}}", "{'update':{'button_released':'2'}}",
    "{'update':{'button_released':'3'}}", "{'update':{'button_released':'4'}}",
    "{'update':{'button_released':'5'}}",
};

void fix_strings(void) {
    device_info1.replace("'", "\"");
    device_info2.replace("'", "\"");
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
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
        pinMode(i + pinToButtonOffset, INPUT_PULLUP);
    }
}

void loop() {
    if (Serial.available()) {
        check_comms(Serial.read());
    }

    check_buttons();
}
