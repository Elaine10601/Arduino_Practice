// 範例: LCD_EX1.ino
// 功能: LCD閃爍 --- 顯示hello world!
// Support: Atmel SAM3X8E  

#include "LiquidCrystalEx.h"

// initialize the library with the numbers of the interface pins
LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);

int R_W = 9;

byte smiley[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000
};

void setup()
{
   pinMode(R_W, OUTPUT);
   
   digitalWrite(R_W, LOW);

   // set up the LCD's number of columns and rows: 
   lcd.begin(20, 2);

   // Print a message to the LCD.
   lcd.setCursor(0,0);
   lcd.write("hello, world!");
   
   lcd.createChar(0, smiley);
   lcd.setCursor(0,1);
   lcd.write(byte(0));
  
}

void loop()
{
   // Turn off the display:
   lcd.noDisplay();
   delay(500);
  
   // Turn on the display:
   lcd.display();
   delay(500);
  
}
