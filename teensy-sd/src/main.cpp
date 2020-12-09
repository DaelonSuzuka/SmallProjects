#include <Arduino.h>


extern "C" {
#include <stdarg.h>
int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buf[100];

    vsprintf(buf, format, args);

    Serial.printf(buf);

    va_end(args);
    Serial.flush();
    return 0;
}
#include "shell/shell.h"
}

void setup() {
    Serial.begin(9600);
    shell_init();
}

void loop() {
    while (1) {
        shell_update((char)Serial.read());
    }
}