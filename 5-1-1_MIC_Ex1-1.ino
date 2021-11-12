// 範例: MIC_Ex1-1.ino.ino
// 功能: LCD第2行 顯示MIC音量類比值
// Support: Atmel SAM3X8E  

#include "Arduino.h"
#include "LiquidCrystalEx.h"
#include "DueTimer.h"

//初始化各介面接腳
LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);
int R_W = 9;
int sound_ain=A0;

int timer_200ms;
int ad_value;

void myHandler()         //TIMER 200ms
{
   timer_200ms  = 1;     
}

void setup()
{
   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
   
   pinMode(sound_ain,INPUT);
   
   lcd.begin(20, 2);      //設定LCD的行列數
   
   timer_200ms = 0;
   
   Timer3.attachInterrupt(myHandler);
   Timer3.start(200000);  //啟動每200ms的時間中斷   
   
}

void loop()
{  
   if(timer_200ms == 1)
   {
      timer_200ms = 0;
  
      ad_value=analogRead(sound_ain);   
      lcd.setCursor(0, 2);  
      lcd.print("    ");                              
      lcd.setCursor(0, 2);        
      lcd.print(ad_value);
    }        
}


