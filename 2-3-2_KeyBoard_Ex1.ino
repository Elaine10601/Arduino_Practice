// 範例: KeyBoard_EX1.ino
// 功能: KeyBoard 鍵值顯示在LCD20x2螢幕上　
// Support: Atmel SAM3X8E  

#include "LiquidCrystalEx.h"

// initialize the library with the numbers of the interface pins
LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);
int R_W = 9;

int C1 = 46;
int C2 = 47;
int C3 = 48;
int C4 = 49;
int R1 = 50;
int R2 = 51;
int R3 = 52;
int R4 = 53;


int key_count;
int count;

void setup()
{                
   pinMode(R_W, OUTPUT);   
   digitalWrite(R_W, LOW);
   
   lcd.begin(20, 2);                // set up the LCD's number of columns and rows: 
   lcd.print("KEY BOARD 4X4");      // Print a message to the LCD.
   lcd.setCursor(0, 2);
   key_count = 0;    
   
   pinMode(C1, OUTPUT);
   pinMode(C2, OUTPUT);
   pinMode(C3, OUTPUT);
   pinMode(C4, OUTPUT);
   pinMode(R1, INPUT_PULLUP);
   pinMode(R2, INPUT_PULLUP);
   pinMode(R3, INPUT_PULLUP);
   pinMode(R4, INPUT_PULLUP);
       
   digitalWrite(C1, HIGH);
   digitalWrite(C2, HIGH);
   digitalWrite(C3, HIGH);
   digitalWrite(C4, HIGH);
   
   count = 0;
}

void loop()
{
   int i;
   unsigned int col;
   unsigned int row;
   unsigned char row_data;
   int key_input;
   
   
   for (col = 0; col < 4; col++)   // 列掃描
   {
      digitalWrite(C1, (col == 0) ? LOW : HIGH);
      digitalWrite(C2, (col == 1) ? LOW : HIGH);
      digitalWrite(C3, (col == 2) ? LOW : HIGH);
      digitalWrite(C4, (col == 3) ? LOW : HIGH);
      delay(10);

      // 讀取掃描按鍵  
      row_data = 0x00;
      for (row = 0; row < 4; row++)
      {
         if (digitalRead(R1 + row) == LOW)
            row_data = row_data | (0x01 << row);
      }   // for row
         
      if (row_data != 0x00)
         break;
   }   // for col

   if (row_data == 0x01)
      key_input = 0;
   else if (row_data == 0x02)
      key_input = 1;
   else if (row_data == 0x04)
      key_input = 2;
   else if (row_data == 0x08)
      key_input = 3;

   key_input = col + key_input * 4 ;   // 按鍵值

   if (col < 4)   // 有按按鍵
   {
      if (key_count == 1)   // 延遲
      {         
         count++;
         if(count > 20)
         {
            count = 0;   
            
            lcd.begin(20, 2);                // set up the LCD's number of columns and rows: 
            lcd.print("KEY BOARD 4X4");      // Print a message to the LCD.
            lcd.setCursor(0, 2);   
         }            
         if(key_input<10)
            lcd.print(key_input);         
         else
            lcd.print(char(key_input+0x37));             
      }
      
      key_count = key_count + 1;
      if (key_count > 100)
         key_count = 100;
   }
   else
   {
      key_count = 0;
   }

}
