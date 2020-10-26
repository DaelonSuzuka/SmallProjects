#include <ArduinoJson.h>

/* ************************************************************************** */

struct {
    char data[256];
    uint8_t length;
    uint8_t depth;
} buffer;

void insert_character(char currentChar) {
    if (currentChar == '{') {
        buffer.depth++;
    }
    if (buffer.depth > 0) {
        buffer.data[buffer.length++] = currentChar;
    }
    if (currentChar == '}') {
        buffer.depth--;
    }
}

StaticJsonDocument<200> doc;
// {"request":"device_info"}

void check_comms(char currentChar) {
    if (currentChar != 0) {
        insert_character(currentChar);
        if (buffer.length > 0 && buffer.depth == 0) {
            buffer.data[buffer.length] = 0;
            deserializeJson(doc, buffer.data);

            if (doc.as<JsonObject>().containsKey("request")) {
                if (!strcmp(doc["request"], "device_info")) {
                    Serial.write(
                        "{\"update\":{\"device_info\":{\"product_name\":"
                        "\"judipedals\",\"serial_number\":\"6969696969\"}}}");
                }
            }

            memset(&buffer, 0, sizeof(buffer));
        }
    }
}

/* -------------------------------------------------------------------------- */

#define NUMBER_OF_BUTTONS 4
const int pinToButtonOffset = 2;
int prevState[NUMBER_OF_BUTTONS] = {0, 0, 0, 0};

const char *buttonPressedMsg[NUMBER_OF_BUTTONS] = {
    "{\"update\":{\"button_pressed\":\"1\"}}",
    "{\"update\":{\"button_pressed\":\"2\"}}",
    "{\"update\":{\"button_pressed\":\"3\"}}",
    "{\"update\":{\"button_pressed\":\"4\"}}",
};

const char *buttonReleasedMsg[NUMBER_OF_BUTTONS] = {
    "{\"update\":{\"button_released\":\"1\"}}",
    "{\"update\":{\"button_released\":\"2\"}}",
    "{\"update\":{\"button_released\":\"3\"}}",
    "{\"update\":{\"button_released\":\"4\"}}",
};

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
                Serial.write(buttonPressedMsg[i]);
            } else {
                Serial.write(buttonReleasedMsg[i]);
            }
        }
    }
}

/* ************************************************************************** */

void setup() {
    Serial.begin(9600);
    while (!Serial)
        continue;

    memset(&buffer, 0, sizeof(buffer));

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
