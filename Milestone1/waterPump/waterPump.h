#ifndef WATER_PUMP_H
#define WATER_PUMP_H

#include "pico/stdlib.h"

// Initialize the water pump GPIO pins.
void pump_init(uint en_pin, uint in1_pin, uint in2_pin);

// Set pump direction to pump IN.
void pump_in();

// Set pump direction to pump OUT.
void pump_out();

// Turn the pump ON.
void pump_on();

// Turn the pump OFF.
void pump_off();

// Get current pump state (1 = ON, 0 = OFF).
int pump_is_on();

#endif
