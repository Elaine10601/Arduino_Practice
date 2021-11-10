// 範例: MatrixLED_EX1.ino
// 功能: MatrixLED 全彩8x8顯示 - 畫面顏色不間斷循環顯示 
// Support: Atmel SAM3X8E  

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
   {0x000000, 0xff0000, 0x00ff00, 0xffff00, 0xff00ff, 0x00ffff, 0x0000ff, 0x000000},   // 
   {0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000},   //RED 
   {0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00, 0x00ff00},   //GREEN
   {0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00},   //RED GREEN   
   {0x0000ff, 0x0000ff, 0x0000ff, 0x0000ff, 0x0000ff, 0x0000ff, 0x0000ff, 0x0000ff},   //BLUE
   {0xff00ff, 0xff00ff, 0xff00ff, 0xff00ff, 0xff00ff, 0xff00ff, 0xff00ff, 0xff00ff},   //BLUE RED
   {0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff, 0x00ffff},   //BLUE GREEN   
   {0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff},   //RGB   
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
      
      delayMicroseconds(300);
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

