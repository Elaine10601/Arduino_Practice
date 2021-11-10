// 範例: KeyBoard_LEDdisplay_EX2.ino
// 功能: KeyBoard 鍵值顯示16 bits LED模組　
// Support: Atmel SAM3X8E  

int C1 = 46;
int C2 = 47;
int C3 = 48;
int C4 = 49;
int R1 = 50;
int R2 = 51;
int R3 = 52;
int R4 = 53;


int led1 = 30;

void setup()
{                
    int i;                        //LED pin define
  
   for (i = 0; i < 16; i++)
      pinMode(led1 + i, OUTPUT);     

   for (i = 0; i < 16; i++)
      digitalWrite(led1 + i, LOW);

  
   pinMode(C1, OUTPUT);
   pinMode(C2, OUTPUT);
   pinMode(C3, OUTPUT);
   pinMode(C4, OUTPUT);
   pinMode(R1, INPUT_PULLUP);
   pinMode(R2, INPUT_PULLUP);
   pinMode(R3, INPUT_PULLUP);
   pinMode(R4, INPUT_PULLUP);
       
   digitalWrite(C1, HIGH);
   digitalWrite(C2, HIGH);
   digitalWrite(C3, HIGH);
   digitalWrite(C4, HIGH);
   
}

void loop()
{
   int i;
   int j;
   unsigned int col;
   unsigned int row;
   unsigned char row_data;
   int key_input;
   
   
   for (col = 0; col < 4; col++)   // 列掃描
   {
      digitalWrite(C1, (col == 0) ? LOW : HIGH);
      digitalWrite(C2, (col == 1) ? LOW : HIGH);
      digitalWrite(C3, (col == 2) ? LOW : HIGH);
      digitalWrite(C4, (col == 3) ? LOW : HIGH);
      delay(10);

      // 讀取掃描按鍵  
      row_data = 0x00;
      for (row = 0; row < 4; row++)
      {
         if (digitalRead(R1 + row) == LOW)
            row_data = row_data | (0x01 << row);
      }   // for row
         
      if (row_data != 0x00)
         break;
   }   // for col

   if (row_data == 0x01)
      key_input = 0;
   else if (row_data == 0x02)
      key_input = 1;
   else if (row_data == 0x04)
      key_input = 2;
   else if (row_data == 0x08)
      key_input = 3;

   key_input = col + key_input * 4 ;   // 按鍵值

   if (col < 4)   // 有按按鍵
   {

            for (i = 0; i < 16; i++)   //LED display
            {
                for (j = 0; j < 16; j++)
                {
                  if (key_input == j)
                  //if (i == j)
                      digitalWrite(led1 + j, HIGH);
                  else
                      digitalWrite(led1 + j, LOW);
                }   // for j
      
            }   // for i
   }

}
