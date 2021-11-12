// 範例: MIC_Ex2.ino.ino
// 功能: 16顆LED 隨音量大小顯示　
//       (建議靜音狀態時調整模組VR至570左右)
// Support: Atmel SAM3X8E

#include "Arduino.h"
#include "LiquidCrystalEx.h"
#include "DueTimer.h"

#define ZERO 570

//初始化各介面接腳
LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);
int R_W = 9;
int led1 = 22;
int led16 = 37;

int sound_ain=A0;

int timer1;
int ad_value,volume;


void myHandler()         //TIMER 40ms的中斷副程式
{
   timer1  = 1;
}

void setup()
{
   int i;

   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
   
   pinMode(sound_ain,INPUT);
  
   lcd.begin(20, 2);  // 設定LCD的行列數
      
   for (i = 0; i < 16; i++)
      pinMode(led1 + i, OUTPUT);     
   for (i = 0; i < 16; i++)
      digitalWrite(led1 + i, LOW);
   
   timer1 = 0;
   volume = 0;

   Timer3.attachInterrupt(myHandler);
   Timer3.start(40000); //啟動每40ms的時間中斷  
}

void loop()
{  
   if(timer1 == 1)
   {
      timer1 = 0;
  
      ad_value=analogRead(sound_ain);      
      if(ad_value >= ZERO)   //MIC朝正負向變化，只取值的大小
         volume = ad_value - ZERO;     
      else   
         volume = ZERO - ad_value;  
         
      if(volume > (363))
         led_control(16);
      else if(volume > (318))   
         led_control(15);        
      else if(volume > (276))   
         led_control(14);
      else if(volume > (237))   
         led_control(13);
      else if(volume > (201))   
         led_control(12);
      else if(volume > (168))   
         led_control(11);
      else if(volume > (138))   
         led_control(10);
      else if(volume > (111))   
         led_control(9);
      else if(volume > (87))
         led_control(8);
      else if(volume > (66))   
         led_control(7);        
      else if(volume > (48))   
         led_control(6);
      else if(volume > (33))   
         led_control(5);
      else if(volume > (21))   
         led_control(4);
      else if(volume > (12))   
         led_control(3);
      else if(volume > (6))   
         led_control(2);
      else if(volume > (3))   
         led_control(1);
      else
         led_control(0);                 
            
      lcd.setCursor(0, 2);  
      lcd.print("    ");                              
      lcd.setCursor(0, 2);        
      lcd.print(ad_value);
      //delay(500); //觀察MIC變化值       
   }        
}

void led_control(unsigned char number)
{
   int i;
   for (i = 0; i < 16; i++)
   {
      if(number > i)
         digitalWrite(led16 - i, HIGH);          
      else   
         digitalWrite(led16 - i, LOW);
   }   
}
