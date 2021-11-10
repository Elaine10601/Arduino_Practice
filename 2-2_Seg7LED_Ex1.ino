// 範例: Seg7LED_EX1.ino
// 功能: 七段顯示--- 掃描顯示1234
// Support: Atmel SAM3X8E  

int S1 = 4;
int S2 = 5;
int S3 = 6;
int S4 = 7;

int Seg7_A   = 8;
int Seg7_B   = 9;
int Seg7_C   = 10;
int Seg7_D   = 11;
int Seg7_E   = 12;
int Seg7_F   = 13;
int Seg7_G   = 14;
int Seg7_DOT = 15;

int SegCount;

#define NUMBER_TEXT "1234"

const
   unsigned char NUMBER[16] =
   {
      0x3F,   // --0--
      0x06,   // --1--
      0x5B,   // --2--
      0x4F,   // --3--
      0x66,   // --4--
      0x6D,   // --5--
      0x7D,   // --6--
      0x07,   // --7--
      0x7F,   // --8--
      0x6F,   // --9--
      0x77,   // --A--
      0x7C,   // --B--
      0x39,   // --C--
      0x5E,   // --D--
      0x79,   // --E--
      0x71,   // --F--
   };

void setup()
{                
   SegCount = 0;
   
   pinMode(Seg7_A,    OUTPUT);
   pinMode(Seg7_B,    OUTPUT);
   pinMode(Seg7_C,    OUTPUT);
   pinMode(Seg7_D,    OUTPUT);
   pinMode(Seg7_E,    OUTPUT);
   pinMode(Seg7_F,    OUTPUT);
   pinMode(Seg7_G,    OUTPUT);
   pinMode(Seg7_DOT,  OUTPUT);
  
   pinMode(S1, OUTPUT);
   pinMode(S2, OUTPUT);
   pinMode(S3, OUTPUT);
   pinMode(S4, OUTPUT);
   
   digitalWrite(Seg7_A, HIGH);
   digitalWrite(Seg7_B, HIGH);
   digitalWrite(Seg7_C, HIGH);
   digitalWrite(Seg7_D, HIGH);
   digitalWrite(Seg7_E, HIGH);
   digitalWrite(Seg7_F, HIGH);
   digitalWrite(Seg7_G, HIGH);
   digitalWrite(Seg7_DOT, HIGH);
  
   digitalWrite(S1, HIGH);
   digitalWrite(S2, HIGH);
   digitalWrite(S3, HIGH);
   digitalWrite(S4, HIGH);
  
}

void loop()
{
   int i;
   unsigned char seg7_data;

   // 關閉掃描線
   for (i = 0; i < 4; i++)
      digitalWrite(S1 + i, HIGH);
   delayMicroseconds(300);

   seg7_data = NUMBER[NUMBER_TEXT[SegCount] - 0x30];
   
   // 資料Byte轉Bit輸出      
   for (i = 0; i < 8; i++)
   {
      if ((seg7_data & (0x01 << i)) == 0)
         digitalWrite(Seg7_A + i, HIGH);
      else
         digitalWrite(Seg7_A + i, LOW);
   }   // for i
   
   // 開啟掃描線
   digitalWrite(S1 + SegCount, LOW);
   delayMicroseconds(300);
   
   SegCount = SegCount + 1;
   SegCount = SegCount % 4;

}
