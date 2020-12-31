#include <Arduino.h>
#include <EEPROM.h>

extern "C" {
#include <stdarg.h>
int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buf[100];

    vsprintf(buf, format, args);

    Serial.printf(buf);

    va_end(args);
    return 0;
}

#include "shell/shell.h"
#include "shell/shell_command_processor.h"
}



static void sh_eeprom(int argc, char **argv) {
    switch (argc) {
    case 1:
        sh_println("usage: \teeprom write <address> <data>");
        sh_println("\teeprom read <address>");
        return;

    case 3:
        if (!strcmp(argv[1], "read")) {
            uint16_t address = atoi(argv[2]);

            printf("%02x\r\n", EEPROM.read(address));
            return;
        }
        break;

    case 4:
        if (!strcmp(argv[1], "write")) {
            uint16_t address = atoi(argv[2]);
            uint8_t data = atoi(argv[3]);

            EEPROM.write(address, data);
            printf("%02x\r\n", EEPROM.read(address));
            return;
        }
        break;

    default:
        break;
    }
    sh_println("invalid arguments");
    return;
}




void setup() {
    Serial.begin(9600);
    shell_init();

    register_command(sh_eeprom, "eeprom");
}

void loop() {
    while (1) {
        shell_update((char)Serial.read());
    }
}