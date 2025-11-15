#include "LcdDisplay.h"
#include <stdio.h>
#include <string.h>


//write 1 byte to the LCD
void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, LCD_I2C_ADDR, &val, 1, false);
}

// function to send command/data with backlight
void lcd_toggle_enable(uint8_t val) {
    
    sleep_us(600);
    i2c_write_byte(val | 0x04); // EN high
    sleep_us(600);
    i2c_write_byte(val & ~0x04); // EN low
    sleep_us(600);
}


void lcd_send(uint8_t val, int mode) {
    uint8_t high_nibble = (val & 0xF0) | mode | LCD_BACKLIGHT;
    uint8_t low_nibble = ((val << 4) & 0xF0) | mode | LCD_BACKLIGHT;

    i2c_write_byte(high_nibble);
    lcd_toggle_enable(high_nibble);
    i2c_write_byte(low_nibble);
    lcd_toggle_enable(low_nibble);
}

void lcd_send_cmd(uint8_t cmd) {
    lcd_send(cmd, 0); // Mode 0 for command
}

void lcd_send_data(uint8_t data) {
    lcd_send(data, 1); // Mode 1 for data
}

void lcd_init() {
    i2c_init(I2C_PORT, 100 * 1000); // 100KHz
    gpio_set_function(12, GPIO_FUNC_I2C); 
    gpio_set_function(13, GPIO_FUNC_I2C); 
 gpio_set_dir(12,1);
 gpio_set_dir(13, 1);
   
    lcd_send_cmd(0x33); 
    lcd_send_cmd(0x32); 
    lcd_send_cmd(0x28); 
    lcd_send_cmd(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    lcd_send_cmd(LCD_CLEARDISPLAY);
    sleep_ms(2);
}

void lcd_clear() {
    lcd_send_cmd(LCD_CLEARDISPLAY);
    sleep_ms(2);
}

void lcd_set_cursor(int line, int position) {
    uint8_t addr = (line == 0) ? 0x80 : 0xC0;
    addr += position;
    lcd_send_cmd(addr);
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

// --- Project-Specific Functions ---

void lcd_show_target(int targetLevel) {
    char buf[17];
    lcd_clear();
    lcd_set_cursor(0, 0);
    snprintf(buf, 17, "Target:  %d u", targetLevel);
    lcd_print(buf);
}

void lcd_update_current(int currentLevel) {
    char buf[17];
    lcd_set_cursor(1, 0);
    snprintf(buf, 17, "Current: %d u   ", currentLevel);
    lcd_print(buf);
}

void lcd_show_success(const char *clue) {
    char buf[17];
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("OCR COMPLETE!");
    lcd_set_cursor(1, 0);
    snprintf(buf, 17, "Clue: %s", clue);
    lcd_print(buf);
}

void lcd_show_failure() {
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Failure!");
    lcd_set_cursor(1, 0);
    lcd_print("Resetting pump...");
}

void lcd_show_scanning() {
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Level Correct!");
    lcd_set_cursor(1, 0);
    lcd_print("Scanning clue...");
}

void lcd_show_scan_failed() {
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("OCR Scan Failed");
    lcd_set_cursor(1, 0);
    lcd_print("Retrying...");
}