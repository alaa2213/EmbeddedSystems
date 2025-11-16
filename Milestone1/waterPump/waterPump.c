#include "waterPump.h"

static uint pumpPin = 0;
static int pumpState = 0;   // 0 = off, 1 = on

void pump_init(uint pumpPin) {
    pumpPin = pumpPin;
    gpio_init(pumpPin);               // Initialize pin
    gpio_set_dir(pumpPin, GPIO_OUT);  // Set as output
    gpio_put(pumpPin, 0);             // Start OFF
    pumpState = 0;
}

void pump_on() {
    gpio_put(pumpPin, 1);
    pumpState = 1;
}

void pump_off() {
    gpio_put(pumpPin, 0);
    pumpState = 0;
}

int pump_is_on() {
    return pumpState;
}
