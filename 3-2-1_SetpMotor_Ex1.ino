// 範例: SetpMotor_EX1.ino
// 功能: SetpMotor一相激磁
// Support: Atmel SAM3X8E  
char STEP_PIN[4] = {46,47,48,49}; //STEP_A_=46;STEP_B=47;STEP_A=48;STEP_B_=49;
char STEP_TABLE[4] = {0x01, 0x02, 0x04, 0x08};
void stepM_decode(char x);
void setup()
{                
   for(char i=0;i<4;i++)
   {
     pinMode(STEP_PIN[i], OUTPUT);
     digitalWrite(STEP_PIN[i], LOW);
   } 
}

void loop()
{    
   for (char i = 0; i < 4; i++)  // 順時針
   { 
         stepM_decode( STEP_TABLE[i] );
         delay(2);
   }   // for i
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
