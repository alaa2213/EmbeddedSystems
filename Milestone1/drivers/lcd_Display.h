

#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include <Arduino.h>

// This function initializes the LCD screen.
// Call this once in your main setup().
void lcd_init();

// Displays the target level (from the RFID) on the top line.
void lcd_show_target(int targetLevel);

// Updates the bottom line with the current water level.
void lcd_update_current(int currentLevel);

// Clears the screen and shows a "Success" message with the clue.
void lcd_show_success(String clue);

// Clears the screen and shows a "Failure" message.
void lcd_show_failure();

// Clears the LCD screen.
void lcd_clear();

#endif