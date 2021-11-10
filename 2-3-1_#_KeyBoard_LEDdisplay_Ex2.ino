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
  Serial.begin(9600);
  Serial.println("key_board");            
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
   int k;
   int t;
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
     switch(key_input)
     {
      case 0:
      {Serial.print("目前所按的按鍵為 ");
        Serial.print(key_input);
        Serial.println(" LED D1~D8座號BCD碼");
        for (i = 0; i < 16; i++)
        {
          if(i==7)
          {
            digitalWrite(led1 + i, HIGH);
          }
          else
          {
            digitalWrite(led1 + i, LOW);
          }
        }
        delay(50);
        break;
      }
      case 2:
      { Serial.print("目前所按的按鍵為 ");
        Serial.print(key_input);
        Serial.println(" LED D1~D16單燈左移");
        for (i = 15; i >= 0; i--)
        {
          for (j = 15; j >= 0; j--)
          {
            if (i == j)
            digitalWrite(led1 + j, HIGH);
            else
            digitalWrite(led1 + j, LOW);
          }
          delay(100);
        }
       delay(50);
        break;
      }
      case 1:
      {Serial.print("目前所按的按鍵為 ");
        Serial.print(key_input);
        Serial.println(" LED D1~D16單燈右移");
        for (i = 0; i < 16; i++)
        {
          for (j = 0; j < 16; j++)
          {
            if (i == j)
            digitalWrite(led1 + j, HIGH);
            else
            digitalWrite(led1 + j, LOW);
          }
          delay(100);
        } 
        delay(50);
        break;
      }
      case 3:
      {Serial.print("目前所按的按鍵為 ");
        Serial.print(key_input);
        Serial.println(" LED D1~D8依序逐漸點亮，再逐漸熄滅");
        for (t = 0; t < 16; t++)
        {
          digitalWrite(led1 + t, LOW);
        }
        for (i = 0; i < 8; i++)
        {
          for (j = 0; j < 8; j++)
          {
            if (i < j)
            digitalWrite(led1 + j, LOW);
            else
            digitalWrite(led1 + j, HIGH);
          }
          delay(100);
        }

         for (i = 7; i > 0; i--)
         {
          for (j = 7; j > 0; j--)
          {
            if (i == j)
            digitalWrite(led1 + j, LOW);
          }
          delay(100);
         }   // for i
         delay(50);
         break;
        }
       case 4:
              { Serial.print("目前所按的按鍵為 ");
        Serial.print(key_input);
        Serial.println(" LED D1~D16閃爍2次");
                for (t = 0; t < 16; t++)
        {
          digitalWrite(led1 + t, LOW);
        }
                for (k = 0; k < 2; k++)
                {
                  for (i = 0; i < 16; i++)
                  {
                    digitalWrite(led1 + i, HIGH);
                  }
                  delay(200);
                  for (j = 0; j < 16; j++)
                  {
                    digitalWrite(led1 + j, LOW);
                  }
                  delay(200);
                }
                delay(50);
                break;
              }
       default:
       {Serial.print("目前所按的按鍵為 ");
        Serial.print(key_input);
        Serial.println(" 保持前一功能執行後的狀態");
        for (i = 0; i < 16; i++)
        {
          if(digitalRead(led1 + i) == LOW)
          {
            digitalWrite(led1 + i, LOW);
          }
          else
          {
            digitalWrite(led1 + i, HIGH);
          }
        }
        delay(50);
        break;
       }
     }
   }
}
