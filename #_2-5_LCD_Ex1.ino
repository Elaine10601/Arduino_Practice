// 功能: LCD閃爍 --- 顯示二子甲

#include "LiquidCrystalEx.h"

// initialize the library with the numbers of the interface pins

LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);

int R_W = 9;
byte smiley[8]={
  B00000,
  B01010,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
byte two[8]={
  B00000,
  B01110,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000
};
byte ele[8]={
  B11111,
  B00010,
  B01100,
  B11111,
  B00010,
  B10010,
  B01100,
  B00000
};
byte one[8]={
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B00100,
  B00100,
  B00000
};

void setup()
{
   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
   lcd.begin(20, 2);
   lcd.begin(20, 2);
   lcd.begin(20, 2);
   
   lcd.createChar(1,two);
   lcd.setCursor(0,0);
   lcd.print(char(0x01));
   
   lcd.createChar(2,ele);
   lcd.setCursor(2,0);
   lcd.print(char(0x02));
   
   lcd.createChar(3,one);
   lcd.setCursor(4,0);
   lcd.print(char(0x03));
   
     
   
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
