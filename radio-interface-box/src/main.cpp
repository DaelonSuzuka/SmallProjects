#include <Arduino.h>

#include <ArduinoJson.h>

/* ************************************************************************** */

#define OUTPUT_PIN 2
#define INPUT_PIN 3

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

const char *updateMsg[] = {
    "{\"update\":{\"input\":\"1\"}}",
    "{\"update\":{\"input\":\"0\"}}",
};

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
                        "\"RadioInterface\",\"serial_number\":\"420420420\"}}"
                        "}");
                }
            } else if (doc.as<JsonObject>().containsKey("command")) {
                if (doc["command"].as<JsonObject>().containsKey("set_output")) {
                    if (!strcmp(doc["command"]["set_output"], "1")) {
                        digitalWrite(2, HIGH);
                        Serial.write("{\"update\":{\"output\":\"1\"}}");
                    } else if (!strcmp(doc["command"]["set_output"], "0")) {
                        digitalWrite(2, LOW);
                        Serial.write("{\"update\":{\"output\":\"0\"}}");
                    }
                }
            }

            memset(&buffer, 0, sizeof(buffer));
        }
    }
}

/* -------------------------------------------------------------------------- */

void check_buttons(void) {
    static unsigned long lastCheckTime = 0;
    if (millis() - lastCheckTime < 20) {
        return;
    }
    lastCheckTime = millis();
    static int prevState = 0;
    
    int currentState = digitalRead(INPUT_PIN);
    
    if (currentState != prevState) {

        if (currentState) {
            Serial.write("{\"update\":{\"input\":\"1\"}}");
        } else {
            Serial.write("{\"update\":{\"input\":\"0\"}}");
        }
    }
    prevState = currentState;
}

/* ************************************************************************** */

void setup() {
    // goes to TUNE btn on tuner
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, HIGH);

    // comes from RADIO_CMD_PIN on tuner
    pinMode(INPUT_PIN, INPUT_PULLUP);

    memset(&buffer, 0, sizeof(buffer));

    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        check_comms(Serial.read());
    }

    check_buttons();
}