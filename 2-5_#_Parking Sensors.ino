//https://atceiling.blogspot.com/2017/03/arduino_28.html


//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int trigPin = 12;                  //Trig Pin
int echoPin = 11;                  //Echo Pin
long duration, cm, inches;

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
  Serial.begin (9600);             // Serial Port begin
  pinMode(trigPin, OUTPUT);        // 定義輸入及輸出 
  pinMode(echoPin, INPUT);
  
  lcd.init();                      // initialize the lcd 

  // Print a message to the LCD.
  lcd.backlight();

  lcd.createChar(1,two);
  lcd.setCursor(0,0);
  lcd.print(char(0x01));
   
  lcd.createChar(2,ele);
  lcd.setCursor(1,0);
  lcd.print(char(0x02));
   
  lcd.createChar(3,one);
  lcd.setCursor(2,0);
  lcd.print(char(0x03));
  
  
  lcd.print("01 HC-SR04");
  lcd.setCursor(0,1);
  lcd.print("Distance:");
  lcd.setCursor(14,1);
  lcd.print("cm");
  
}


void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);     // 給 Trig 高電位，持續 10微秒
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);             // 讀取 echo 的電位
  duration = pulseIn(echoPin, HIGH);   // 收到高電位時的時間
 
  cm = (duration/2) / 29.1;         // 將時間換算成距離 cm
  
  Serial.print("Distance : ");  
  lcd.setCursor(10,1);
  lcd.print("    ");
  lcd.setCursor(10,1);
  lcd.print((int)cm);
  lcd.setCursor(14,1);
  lcd.print("cm");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  //無源蜂鳴器--tone()函數
  if(cm>100)
  {
    tone(13, 1000);//發聲tone(腳位, 頻率, 持續時間);
    delay(500);
    noTone(13);//停止發聲noTone(腳位);
   }
   else if(30<=cm)
  {
    tone(13, 1000);
    delay(100);
    noTone(13);
  }
  else if(30>cm)
  {
    tone(13, 1000,0);
    
   }
  }
