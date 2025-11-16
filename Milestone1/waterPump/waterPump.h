#ifndef WATER_PUMP_H
#define WATER_PUMP_H

#include "pico/stdlib.h"

// Initialize the water pump GPIO pin.
void pump_init(uint pumpPin);

// Turn the pump ON.
void pump_on();

// Turn the pump OFF.
void pump_off();

// Get current pump state (1 = ON, 0 = OFF)
int pump_is_on();

#endif
