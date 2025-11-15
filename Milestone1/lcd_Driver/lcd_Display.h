#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"


#define I2C_PORT i2c1
#define LCD_I2C_ADDR 0x27 
// LCD Commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_FUNCTIONSET 0x20
#define LCD_DISPLAYCONTROL 0x08
#define LCD_ENTRYMODESET 0x04

// Flags for display control
#define LCD_DISPLAYON 0x04
#define LCD_CURSOROFF 0x00
#define LCD_BLINKOFF 0x00

// Flags for function set
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x8DOTS 0x00

// Flag for backlight
#define LCD_BACKLIGHT 0x08

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