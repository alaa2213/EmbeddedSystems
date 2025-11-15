#ifndef RFIDDRIVER_H
#define RFIDDRIVER_H

#include "pico/stdlib.h"

// --- CONFIGURATION ---
// SPI0 pins
#define SPI_PORT spi0
#define SPI_MISO_PIN 16
#define SPI_SCK_PIN  18
#define SPI_MOSI_PIN 19
#define SPI_CS_PIN   17
#define RFID_RST_PIN 22
// ---------------------

void rfid_init();
bool rfid_get_clue(char *clue_buffer, uint8_t buffer_size);

#endif