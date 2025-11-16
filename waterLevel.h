#ifndef WATER_LEVEL_H
#define WATER_LEVEL_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdint.h>

/// ADC channel index for A2 / GPIO28 on Raspberry Pi Pico
#define WATER_LEVEL_ADC_INPUT  2

/**
 * @brief Initialize the water level sensor ADC input.
 * 
 * - Initializes ADC hardware
 * - Configures GPIO28 for analog input
 */
void water_level_init(void);

/**
 * @brief Set calibration values for dry and full (wet).
 * 
 * @param dry_raw  Raw ADC reading when sensor is completely dry
 * @param wet_raw  Raw ADC reading when sensor is fully submerged
 */
void water_level_set_raw_range(uint16_t dry_raw, uint16_t wet_raw);

/**
 * @brief Read the raw 12-bit ADC value (0–4095).
 */
uint16_t water_level_read_raw(void);

/**
 * @brief Read the water level sensor voltage (Volts).
 * 
 * Uses ADC reference of 3.3 V:
 *   V = raw * 3.3 / 4095
 */
float water_level_read_voltage(void);

/**
 * @brief Get water level as a percentage (0.0–100.0).
 * 
 * Uses the calibration range set by water_level_set_raw_range().
 */
float water_level_get_percentage(void);

#ifdef __cplusplus
}
#endif

#endif // WATER_LEVEL_H
