// 範例: ADC0804_LCD.ino
// 功能: LCD顯示VR 電壓值
// Support: Atmel SAM3X8E  
#include "LiquidCrystalEx.h"
// initialize the library with the numbers of the interface pins
LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);
int R_W = 9;
int ADC_WR = 38;
int ADC0804Value[] = { 46,47,48,49,50,51,52,53 };
unsigned int LightValue;

void setup()
{  
   pinMode(R_W, OUTPUT);   
   digitalWrite(R_W, LOW);

   // set up the LCD's number of columns and rows: 
   lcd.begin(20, 2);
   // Print a message to the LCD.
   
   pinMode(ADC_WR, OUTPUT);  
   digitalWrite(ADC_WR, HIGH);      
   
  for(unsigned char i=0;i<8;i++)  //宣告ADC0804數位輸出腳為46,47,48,49,50,51,52,53 
     pinMode(ADC0804Value[i], INPUT);
      
   lcd.setCursor(0, 0);
   lcd.print("ADC0804 READ");
}

void loop()
{
   float vfloat;
   int adcValue; 
   LightValue=0;        
   digitalWrite(ADC_WR, LOW);
   delay(10);      
   digitalWrite(ADC_WR, HIGH);      
   delay(10);    
        
   for(unsigned char i=0;i<8;i++)    //讀取ADC數位輸出腳至Arduino
   {
     adcValue=digitalRead(ADC0804Value[i]); //一個一個位元讀資料ADC數位輸出腳
     LightValue=LightValue + (adcValue*pow(2,i)); //每位元讀出的值做加權值轉換成總和
   }   
   
   vfloat = (float(LightValue) *5)/ 256;
   
   lcd.setCursor(0, 2);   
   lcd.print("VOLTAGE = ");   
   lcd.print(vfloat, 2);      // 顯示到小數點後2位
   
   delay(200);  
}
