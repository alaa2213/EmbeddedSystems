#include "lcd_Display.h"
#include <LiquidCrystal_I2C.h>




const int LCD_ADDRESS = 0x27;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;
// ---------------------

// Create our LCD object
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

void lcd_init() {
  lcd.init();
  lcd.backlight();
  gpio_init()
}

void lcd_show_target(int targetLevel) {
  lcd.clear();
  lcd.setCursor(0, 0); // Top line
  lcd.print("Target:  ");
  lcd.print(targetLevel);
  lcd.print(" units");
}

void lcd_update_current(int currentLevel) {
  lcd.setCursor(0, 1); // Bottom line
  lcd.print("Current: ");
  lcd.print(currentLevel);
  lcd.print(" units");
  lcd.print("   "); // Add spaces to clear any old characters
}

void lcd_show_success(String clue) {
  lcd.clear();
  lcd.setCursor(0, 0); // Top line
  lcd.print("LEVEL STABLE!");
  lcd.setCursor(0, 1); // Bottom line
  lcd.print("Clue: ");
  lcd.print(clue);
}

void lcd_show_failure() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Failure!");
  lcd.setCursor(0, 1);
  lcd.print("Resetting...");
}

void lcd_clear() {
  lcd.clear();
}