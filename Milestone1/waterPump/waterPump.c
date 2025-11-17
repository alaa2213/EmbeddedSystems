#include "waterPump.h"

static uint en_pin = 0;
static uint in1_pin = 0;
static uint in2_pin = 0;

static int pumpState = 0;      // 0 = off, 1 = on
static int pumpDirection = 0;  // 0 = out, 1 = in

//PINS ARE 18 15 25.
void pump_init(uint en, uint in1, uint in2) { 
    en_pin = en;
    in1_pin = in1;
    in2_pin = in2;

    gpio_init(en_pin);
    gpio_set_dir(en_pin, GPIO_OUT);
    gpio_put(en_pin, 0);  // Disabled at start

    gpio_init(in1_pin);
    gpio_set_dir(in1_pin, GPIO_OUT);
    gpio_put(in1_pin, 0);

    gpio_init(in2_pin);
    gpio_set_dir(in2_pin, GPIO_OUT);
    gpio_put(in2_pin, 0);

    pumpState = 0;
    pumpDirection = 0; // Default OUT
}

void pump_in() {
    pumpDirection = 1;
    gpio_put(in1_pin, 1);
    gpio_put(in2_pin, 0);
}

void pump_out() {
    pumpDirection = 0;
    gpio_put(in1_pin, 0);
    gpio_put(in2_pin, 1);
}

void pump_on() {
    // Set direction
    if (pumpDirection == 1) {
        gpio_put(in1_pin, 1);
        gpio_put(in2_pin, 0);
    } else {
        gpio_put(in1_pin, 0);
        gpio_put(in2_pin, 1);
    }

    // Enable motor
    gpio_put(en_pin, 1);

    pumpState = 1;
}

void pump_off() {
    gpio_put(en_pin, 0);
    gpio_put(in1_pin, 0);
    gpio_put(in2_pin, 0);

    pumpState = 0;
}

int pump_is_on() {
    return pumpState;
}
