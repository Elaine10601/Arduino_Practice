//步進馬達順轉二圈逆轉一圈並顯示在lcd上

// 範例: KeyBoard_EX1.ino
// 功能: KeyBoard 鍵值顯示在LCD20x2螢幕上　
// Support: Atmel SAM3X8E  

#include "LiquidCrystalEx.h"

// initialize the library with the numbers of the interface pins
LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);
int R_W = 9;

char STEP_PIN[4] = {46,47,48,49}; 
char STEP_TABLE[4] = {0x01, 0x02, 0x04, 0x08};
void stepM_decode(char x);




int l,j,k;
int a[3]={0,0,1};

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

  for(char i=0;i<4;i++)
   {
     pinMode(STEP_PIN[i], OUTPUT);
     digitalWrite(STEP_PIN[i], LOW);
   } 
   pinMode(R_W, OUTPUT);   
   digitalWrite(R_W, LOW);
   
   

   lcd.begin(20, 2);
   lcd.begin(20, 2);
   lcd.begin(20, 2);
   
   lcd.createChar(1,two);
   lcd.setCursor(0,0);
   lcd.print(char(0x01));
   
   lcd.createChar(2,ele);
   lcd.setCursor(1,0);
   lcd.print(char(0x02));
   
   lcd.createChar(3,one);
   lcd.setCursor(2,0);
   lcd.print(char(0x03));
   
   lcd.setCursor(4, 0); 
                  // set up the LCD's number of columns and rows: 
   lcd.print("01 Elaine");      // Print a message to the LCD.
   

   lcd.setCursor(0, 1); 
   lcd.print("stepMotor fwd2rev1");
   
   

}
void loop()
{              
   if (l==0)
  {
 for (l=0;l<3;l++)
 {
  j=0;
  for (int g=0;g<512;g++)
  {
   for (int i = 0; i < 4; i++)  // 順時針
   { 
    if (a[l]==0)
    {k=i;}
    else if(a[l]==1)
    {k=3-i;}
         stepM_decode( STEP_TABLE[k] );
         delay(2);
   }   // for i
 
  }
 }
  }
}


void stepM_decode(char x) //4bits 步進馬達根據查表輸出腳位4次
{
  for (char cnt = 0; cnt < 4; cnt++) 
  {
    if(x % 2==1) //取出查表值的最低位元
      digitalWrite(STEP_PIN[cnt], HIGH);
    else  
      digitalWrite(STEP_PIN[cnt],LOW);
    x=x>>1;   //查表值右移一位元  
  }
}
