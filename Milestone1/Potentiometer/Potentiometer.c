#include "potentiometer.h"

void potentiometer_init(uint8_t pot_pin) {
    pinMode(pot_pin, INPUT);  
}

uint16_t potentiometer_read(uint8_t pot_pin) {
    return analogRead(pot_pin);  
}

uint8_t potentiometer_read_percentage(uint8_t pot_pin) {
    uint16_t raw_value = analogRead(pot_pin);
    return (raw_value * 100) / 1023;  
}
