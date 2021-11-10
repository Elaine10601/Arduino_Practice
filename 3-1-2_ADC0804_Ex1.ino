// 範例: ADC0804_Ex1.ino
// 功能: 16 BITS LED中的八顆LED顯示VR => ADC轉換值(2進制)
// Support: Atmel SAM3X8E 
int led1 = 22;
int ledPins[] = { 22,23,24,25,26,27,28,29 }; 
int ADC_WR = 38;
int ADC0804Value[] = { 46,47,48,49,50,51,52,53 };

void setup()
{ 
  for(unsigned char i=0;i<8;i++)  //宣告八顆LED腳位為22,23,24,25,26,27,28,29
  {
     pinMode(ledPins[i], OUTPUT);
     digitalWrite(ledPins[i], LOW);
  } 
  pinMode(ADC_WR, OUTPUT);  
  digitalWrite(ADC_WR, HIGH);   //ADC0804的WR=1
  for(unsigned char i=0;i<8;i++)  //宣告ADC0804數位輸出腳為46,47,48,49,50,51,52,53 
     pinMode(ADC0804Value[i], INPUT);
}

void loop()
{  
   int adcValue;  
   digitalWrite(ADC_WR, LOW);  //ADC0804的WR=0
   delay(10);
   digitalWrite(ADC_WR, HIGH); //ADC0804的WR=1      
   delay(10);

   for(unsigned char i=0;i<8;i++)    //讀取ADC數位輸出腳至Arduino
   {
     adcValue=digitalRead(ADC0804Value[i]);  //一個一個位元讀資料ADC數位輸出腳
     digitalWrite(ledPins[i], adcValue);     //將讀出值在對應的LED顯示
   }    
}
