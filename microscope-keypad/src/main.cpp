#include "Mouse.h"

// set pin numbers for the five buttons:
const int autoButton = 9;
const int manualButton = 10;
const int clickButton = 16;
const int lastButton = 2;

/* ************************************************************************** */
// mouse control functions

void mouse_to_menu(void) {
    for (int i = 0; i < 15; i++) {
        Mouse.move(-100, -100);
        delay(5);
    }
}

void set_autofocus(void) {
    mouse_to_menu();
    delay(50);
    Mouse.move(15, 27);
    delay(10);
    Mouse.click();
    delay(50);
    Mouse.move(100, -50);
}

void set_manualfocus(void) {
    mouse_to_menu();
    delay(50);
    Mouse.move(70, 27);
    delay(10);
    Mouse.click();
    delay(50);
    Mouse.move(100, -50);
}

void set_clickfocus(void) {
    mouse_to_menu();
    delay(50);
    Mouse.move(35, 27);
    delay(10);
    Mouse.click();
    delay(50);
    Mouse.move(100, -50);
}

/* ************************************************************************** */

void setup() {
    // initialize the buttons' inputs:
    pinMode(autoButton, INPUT_PULLUP);
    pinMode(manualButton, INPUT_PULLUP);
    pinMode(clickButton, INPUT_PULLUP);
    pinMode(lastButton, INPUT_PULLUP);
    // initialize mouse control:
    Mouse.begin();
}

void loop() {
    if (!digitalRead(autoButton)) {
        set_autofocus();
        delay(500);
    } else if (!digitalRead(manualButton)) {
        set_manualfocus();
        delay(500);
    } else if (!digitalRead(clickButton)) {
        set_clickfocus();
        delay(500);
    } else if (!digitalRead(lastButton)) {
        // set_manualfocus();
        delay(500);
    }

    delay(50);
}