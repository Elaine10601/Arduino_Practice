/*https://webduinoio.github.io/demo/max7219/
  1.使用紅色字體顯示班級,即「二子甲」
  2.使用紫色字體顯示座號,ex.座號01號。
  3.座號十、個位數分別向左、右移出畫面。
  4.重覆1~3動作*/


int DATA_R0   = 22;   
int DATA_G0   = 30;
int DATA_B0   = 38;
int S0        = 46;
int S1        = 47;
int S2        = 48;
int EN        = 49;

/*====圖片定義===================*/
unsigned long NUM_LED[8][8] =
{ 
   {0xfdffff, 0xfdffff, 0xbdffff, 0xbdffff, 0xbdffff, 0xbdffff, 0xfdffff, 0xfdffff},    
   {0xefffff, 0x6fffff, 0x6dffff, 0x6effff, 0x4effff, 0x01ffff, 0x6fffff, 0xefffff},   
   {0xffffff, 0x07ffff, 0x57ffff, 0x00ffff, 0x00ffff, 0x57ffff, 0x07ffff, 0xffffff},  
   {0xffffff, 0x81ff81, 0xbdffbd, 0x81ff81, 0xffffff, 0xffffff, 0x81ff81, 0xffffff},   
   {0x81ff81, 0xbdffbd, 0x81ff81, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x81ff81}, 
   {0xbdffbd, 0x81ff81, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff}, 
   {0x81ff81, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff},    
   {0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff},
}; 
   
unsigned int count;
unsigned char page_num;   

void setup()
{                
   unsigned int i;
 
   pinMode(EN, OUTPUT);
   pinMode(S0, OUTPUT);
   pinMode(S1, OUTPUT);
   pinMode(S2, OUTPUT);         

   for (i = 0; i < 24; i++)
   {
      pinMode(DATA_R0 + i, OUTPUT);
      digitalWrite(DATA_R0 + i, HIGH);    
   }   

   digitalWrite(EN, LOW);   
   count    = 0; 
   page_num = 0; 
}

void loop()
{
   unsigned long i,j;
           
   for (i = 0; i < 8; i++)                   // 更換掃描線
   {
      digitalWrite(EN, LOW);                 // 關閉掃描線         
      delayMicroseconds(300);
               
      // 資料轉Bit輸出   
      for (j = 0; j < 24; j++)
      {                        
         if ((NUM_LED[page_num][i] & (0x800000 >> j)) == 0)
            digitalWrite( DATA_R0 + j, LOW);
         else
            digitalWrite( DATA_R0 + j, HIGH);                                  
      }   // for j
           
      select_line(i);                       // 選擇掃描線
      digitalWrite(EN, HIGH);               // 打開掃描線   
      
      delayMicroseconds(1000);
   }   // for i
   
   count = count + 1;
   
   if(count > 300)
   {
      count = 0;
      page_num = (page_num + 1 ) % 8;    
   }     
      
}


void select_line(unsigned char number)
{
   unsigned char i;
   
   if(number == 0)
   {
      digitalWrite(S0, LOW);        
      digitalWrite(S1, LOW);        
      digitalWrite(S2, LOW);                    
   }   
   else if(number == 1)
   {
      digitalWrite(S0, HIGH);        
      digitalWrite(S1, LOW);        
      digitalWrite(S2, LOW);                    
   }   
   else if(number == 2)
   {
      digitalWrite(S0, LOW);        
      digitalWrite(S1, HIGH);        
      digitalWrite(S2, LOW);                    
   }
   else if(number == 3)
   {
      digitalWrite(S0, HIGH);        
      digitalWrite(S1, HIGH);        
      digitalWrite(S2, LOW);                    
   }
   else if(number == 4)
   {
      digitalWrite(S0, LOW);        
      digitalWrite(S1, LOW);        
      digitalWrite(S2, HIGH);                    
   }
   else if(number == 5)
   {
      digitalWrite(S0, HIGH);        
      digitalWrite(S1, LOW);        
      digitalWrite(S2, HIGH);                    
   }
   else if(number == 6)
   {
      digitalWrite(S0, LOW);        
      digitalWrite(S1, HIGH);        
      digitalWrite(S2, HIGH);                    
   }
   else if(number == 7)
   {
      digitalWrite(S0, HIGH);        
      digitalWrite(S1, HIGH);        
      digitalWrite(S2, HIGH);                    
   }                     
}
