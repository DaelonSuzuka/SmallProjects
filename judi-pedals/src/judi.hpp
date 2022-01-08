#ifndef _JUDI_H_
#define _JUDI_H_

#include <ArduinoJson.h>

/* ************************************************************************** */

class JudiBuffer {
  public:
    char data[256] = {};
    uint16_t length = 0;
    uint16_t depth = 0;

    void reset(void);
    void insert_char(char currentChar);
    bool completed(void);
    void terminate(void);
};

void JudiBuffer::reset(void) {
    memset(&data, 0, sizeof(data));
    length = 0;
    depth = 0;
}

void JudiBuffer::insert_char(char currentChar) {
    if (currentChar == '{') {
        depth++;
    }
    if (depth > 0) {
        data[length++] = currentChar;
    }
    if (currentChar == '}' && depth > 0) {
        depth--;
    }
}

bool JudiBuffer::completed(void) {
    if (depth == 0 && length > 0) {
        return true;
    }
    return false;
}

void JudiBuffer::terminate(void) {
    data[length] = 0; //
}

/* ************************************************************************** */

class JUDI {
  public:
    JudiBuffer buffer;
    StaticJsonDocument<200> document;
    StaticJsonDocument<200> device_info;
    JsonObject message;

    JUDI(String name, String serial_number);

    bool update(char c);
    void reset(void);

    bool contains(String string);
    String operator[](String string);
};

JUDI::JUDI(String name, String serial_number) {
    // initialize the device_info json object
    device_info["update"]["device_info"]["product_name"] = name;
    device_info["update"]["device_info"]["serial_number"] = serial_number;
}

bool JUDI::update(char c) {
    buffer.insert_char(c);

    if (buffer.completed()) {
        buffer.terminate();
        deserializeJson(document, buffer.data);
        message = document.as<JsonObject>();
        return true;
    }
    return false;
}

void JUDI::reset(void) {
    buffer.reset(); //
}

bool JUDI::contains(String string) {
    return message.containsKey(string); //
}

String JUDI::operator[](String string) {
    if (contains(string)) {
        return String((const char *)message[string]);
    }
    return String("");
}

#endif // _JUDI_H_