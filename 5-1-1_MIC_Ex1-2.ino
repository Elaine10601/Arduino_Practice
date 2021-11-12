// 範例: MIC_Ex1-2.ino
// 功能: LCD第2行 顯示MIC音量類比值
//       (建議靜音狀態時調整模組VR至570左右)
//       LCD第1行 當偵測到聲音時,會持續顯示HELLO WORLD!! 1秒
// Support: Atmel SAM3X8E

#include "Arduino.h"
#include "LiquidCrystalEx.h"
#include "DueTimer.h"

//初始化各介面接腳
LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);
int R_W = 9;
int sound_ain=A0;

int timer_200ms;
int timer_1s;
int ad_value;
int flag_d0;

void process_count()     //邊緣觸發中斷
{   
  flag_d0 = 1;                    
}

void myHandler()         //TIMER 200ms的中斷副程式
{
  timer_200ms  = 1;     
}

void setup()
{
   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
   
   pinMode(sound_ain,INPUT);
   
   lcd.begin(20, 2);  //設定LCD的行列值
   
   timer_200ms = 0;
   timer_1s = 0; 
   flag_d0  = 0;
   
   attachInterrupt(46, process_count, RISING);      
   
   Timer3.attachInterrupt(myHandler);
   Timer3.start(200000); //啟動每200ms的時間中斷  
}

void loop()
{  
   if(flag_d0 == 1) 
   {
      flag_d0 = 0;                
      lcd.setCursor(0, 0);       
      lcd.print("HELLO WORLD!!       "); 
      timer_1s = 5;
   }
  
   if(timer_200ms == 1)
   {
      timer_200ms = 0;
  
      ad_value=analogRead(sound_ain);   
      lcd.setCursor(0, 2);  
      lcd.print("    ");                              
      lcd.setCursor(0, 2);        
      lcd.print(ad_value);       
 
      if(timer_1s>0)
      {
         timer_1s--;
         if(timer_1s == 0)
         {
            lcd.setCursor(0, 0);       
            lcd.print("                    ");                        
         }
      }
   }        
}


