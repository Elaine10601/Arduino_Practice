// 範例: SetpMotor_EX3.ino
// 功能: SetpMotor先逆再順轉 一、二相激磁
// Support: Atmel SAM3X8E  
char STEP_PIN[4] = {46,47,48,49};//STEP_A_=46;STEP_B=47;STEP_A=48;STEP_B_=49;
char STEP_TABLE[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};
void stepM_decode(char x);
char ctr;  //ctr=1順時鐘旋轉，ctr=0逆時鐘旋轉
int count_step;  
//計算步進馬達走幾步,s600馬達是四相且步進角5.625度/64,所以一二相激磁走一圈需要360/(5.625/64)=4096步
//因為一二相激磁是八個狀態為一個週期,所以一圈走4096/8=512步
void setup()
{                
   for(char i=0;i<4;i++)
   {
     pinMode(STEP_PIN[i], OUTPUT);
     digitalWrite(STEP_PIN[i], LOW);
   } 
   ctr=1;
   count_step=0;
}

void loop()
{     
   if( ctr==1) //順時鐘旋轉
   {       
      if(count_step < 512)
      {
        for (char i = 0; i < 8; i++)  
        { 
             stepM_decode( STEP_TABLE[i] );
             delay(2);
        }   // for i
        count_step++;
      }  
      else 
      {
        count_step=0;ctr=0;
      }  
   }
   else   //逆時鐘旋轉
   {
      if(count_step < 512)
      {
        for (char i = 0; i < 8; i++)  // 逆時針
        { 
             stepM_decode( STEP_TABLE[7-i] );
             delay(2);
        }   // for i
        count_step++;
      }  
      else
      {
        count_step=0;ctr=1;
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
