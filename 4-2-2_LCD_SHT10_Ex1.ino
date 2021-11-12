// 範例: LCD_SHT10_Ex1.ino
// 功能: TempHumidity量測 ,量測結果顯示在LCD20*2
// Support: Atmel SAM3X8E  

#include <stdio.h>
#include "LiquidCrystalEx.h"
#include "SHT1x.h"

LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15);
int R_W = 9;

#define TH_SCK   47
#define TH_DATA  46

SHT1x sht1x(TH_DATA, TH_SCK);

void _nop_(void);
unsigned int GetTemperature(void); // 讀取溫度副程式
unsigned int GetHumidity(void);	  // 讀取濕度副程式

int Count;
int FunctionCount;
#define RUNNING_TABLE "* "

int Temp_C;
int Temp_F;
int Humidity;

void setup()
{
   int i;
   
   pinMode(R_W, OUTPUT);   
   digitalWrite(R_W, LOW);      
   lcd.begin(20, 2);
   
   Count = 0;
   FunctionCount = 0;

   Temp_C = 0;
   Temp_F = 0;
   Humidity = 0; 

}

void loop()
{
   char value0[21];
   char value1[21];
   
   //temp = GetTemperature();   // 讀取溫度
   if (FunctionCount == 0)
      Temp_C = (int) (sht1x.readTemperatureC() * 10);
   else if (FunctionCount == 1)
      Temp_F = (int) (sht1x.readTemperatureF() * 10);
   else
      Humidity = (int) (sht1x.readHumidity() * 10);   // 讀取濕度
   
   delay(10);
   
   FunctionCount = FunctionCount + 1;
   FunctionCount = FunctionCount % 3;
   
   if (FunctionCount == 1)
   {   
      sprintf(value0, "TEMP:%d.%dC/%d.%dF", Temp_C/10, Temp_C%10, Temp_F/10, Temp_F%10);
      sprintf(value1, "%-20s", value0);
      
      lcd.setCursor(0, 0);   
      lcd.print(value1);
   }
   else if (FunctionCount == 2)
   {
      sprintf(value0, "HUMIDITY:%d.%d", Humidity/10, Humidity%10);
      sprintf(value1, "%-19s%c", value0, RUNNING_TABLE[Count]);
      
      lcd.setCursor(0, 1);   
      lcd.print(value1);      
   }
   
   Count = Count + 1;
   Count = Count %2;
   
   delay(100);
   
}

void _nop_(void)
{
   delay(1);
}
