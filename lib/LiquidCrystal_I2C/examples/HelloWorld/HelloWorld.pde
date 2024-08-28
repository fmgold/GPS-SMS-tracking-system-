//Compatible with the Arduino IDE 1.8.5
//Library version:1.1
//https://www.youtube.com/channel/UC2XgdgwbUFigKiLrG7VL6fg?view_as=subscriber

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);   // set the LCD address to 0x27 for a 20 chars and 4 line display

void setup()
{
  lcd.init();                       // initialize the lcd
  lcd.init();
  lcd.backlight();         
}


void loop()
{
 // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("Infinite Xpro");
  delay(2000);
  lcd.setCursor(1,2);
  lcd.print("Wellcome To");
  lcd.setCursor(1,3);
  lcd.print("YOUTUBE.COM");
  delay(2000);
  lcd.clear();
}