// 範例: LED_Ex1.ino
// 功能: 16LED左右移 
// Support: Atmel SAM3X8E  

int led1 = 38;


#define DELAY_TIME 100

void setup()
{                
   int i;
  
   for (i = 0; i < 16; i++)
      pinMode(led1 + i, OUTPUT);     

   for (i = 0; i < 16; i++)
      digitalWrite(led1 + i, LOW);
 
}

void loop()
{
   int i;
   int j;
   
   for (i = 0; i < 16; i++)
   {
      for (j = 0; j < 16; j++)
      {
         if (i == j)
            digitalWrite(led1 + j, HIGH);
         else
            digitalWrite(led1 + j, LOW);
      }   // for j
      
      delay(DELAY_TIME);
   }   // for i

   for (i = 15; i >= 0; i--)
   {
      for (j = 15; j >= 0; j--)
      {
         if (i == j)
            digitalWrite(led1 + j, HIGH);
         else
            digitalWrite(led1 + j, LOW);
      }   // for j
      
      delay(DELAY_TIME);
   }   // for i

}
