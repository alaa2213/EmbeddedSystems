#include "waterLevel.h"

/// Default calibration (you should overwrite using water_level_set_raw_range)
/// dry_raw  = sensor not touching water
/// wet_raw  = sensor fully in water
static uint16_t s_dry_raw = 200;     // example default, adjust after testing
static uint16_t s_wet_raw = 3500;    // example default, adjust after testing

void water_level_init(void)
{
    // Initialize ADC hardware
    adc_init();

    // Enable GPIO28 (ADC2) for analog input
    adc_gpio_init(28);

    // Select ADC input channel (2 => GPIO28)
    adc_select_input(WATER_LEVEL_ADC_INPUT);
}

void water_level_set_raw_range(uint16_t dry_raw, uint16_t wet_raw)
{
    // Make sure wet is not less than dry
    if (wet_raw <= dry_raw) {
        // Fallback to a safe small range to avoid division by zero
        s_dry_raw = dry_raw;
        s_wet_raw = dry_raw + 1;
    } else {
        s_dry_raw = dry_raw;
        s_wet_raw = wet_raw;
    }
}

uint16_t water_level_read_raw(void)
{
    // Always select correct ADC input in case other code changed it
    adc_select_input(WATER_LEVEL_ADC_INPUT);

    // Perform a single ADC conversion (12-bit, 0–4095)
    uint16_t raw = adc_read();
    return raw;
}

float water_level_read_voltage(void)
{
    uint16_t raw = water_level_read_raw();

    // 12-bit ADC, reference 3.3 V
    const float adc_max = 4095.0f;
    const float vref    = 3.3f;

    float voltage = (raw * vref) / adc_max;
    return voltage;
}

float water_level_get_percentage(void)
{
    uint16_t raw = water_level_read_raw();

    // Clamp raw value between dry and wet
    if (raw <= s_dry_raw) {
        return 0.0f;
    }
    if (raw >= s_wet_raw) {
        return 100.0f;
    }

    float range   = (float)(s_wet_raw - s_dry_raw);
    float offset  = (float)(raw - s_dry_raw);
    float percent = (offset / range) * 100.0f;

    return percent;
}
