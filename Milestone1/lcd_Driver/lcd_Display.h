#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"


#define I2C_PORT i2c1
#define LCD_I2C_ADDR 0x27 
// ---------------------

void lcd_init();
void lcd_clear();
void lcd_set_cursor(int line, int position);
void lcd_print(const char *str);
void lcd_show_target(int targetLevel);
void lcd_update_current(int currentLevel);
void lcd_show_success(const char *clue);
void lcd_show_failure();
void lcd_show_scanning();
void lcd_show_scan_failed();

#endif