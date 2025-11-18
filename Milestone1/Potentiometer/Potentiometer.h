#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>

#ifndef POTENTIOMETER_PIN
#define POTENTIOMETER_PIN A3  
#endif

void potentiometer_init(uint8_t pot_pin);

uint16_t potentiometer_read(uint8_t pot_pin);

uint8_t potentiometer_read_percentage(uint8_t pot_pin);

#endif
