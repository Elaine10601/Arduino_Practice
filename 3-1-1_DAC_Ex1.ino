// ------------------------------------------------
// 範例: DAC_EX1.ino
// 功能: DAC數值變化 --- 電壓遞增遞減
// Support: Atmel SAM3X8E  
int DAC_SYNC = 46;
int DAC_SCLK = 47;
int DAC_DIN  = 48;

void SendDAC(unsigned int value);
void setup()
{
   pinMode(DAC_SYNC, OUTPUT);
   pinMode(DAC_SCLK, OUTPUT);
   pinMode(DAC_DIN,  OUTPUT);   
   digitalWrite(DAC_SYNC, LOW);
   digitalWrite(DAC_SCLK, LOW);
   digitalWrite(DAC_DIN,  LOW);  
}

void loop()
{
   unsigned int i;   
   for (i = 0; i < 1024; i = i + 15)    // 電壓遞增  
   {   
      SendDAC(i);
      delay(30);
   }
   for (i = 0; i < 1024; i = i + 15)    // 電壓遞減
   {
      SendDAC(1023 - i);
      delay(30);
   }   
}

void SendDAC(unsigned int value)
{
   unsigned int i;
   
   value = value & 0x03FF;     // 10 Bits
   value = value << 2;
   value = value & 0x0FFC;     // b0000_1111_1111_1100, PD0, PD1 set 0 is Normal Operation
   value = value | 0x0000;     // Set PD0, PD1, Bit13, Bit12, is Normal Operation
//   value = value | 0x1000;   // Set PD0, PD1, Bit13, Bit12, Power-Down with 1kΩ to GND
//   value = value | 0x2000;   // Set PD0, PD1, Bit13, Bit12, Power-Down with 100kΩ to GND
//   value = value | 0x3000;   // Set PD0, PD1, Bit13, Bit12, Power-Down with Hi-Z
   
   digitalWrite(DAC_SYNC, HIGH);
   digitalWrite(DAC_SYNC, LOW);

   for (i = 0; i < 16; i++)
   {
      if (value & 0x8000)    
         digitalWrite(DAC_DIN, HIGH);
      else
         digitalWrite(DAC_DIN, LOW);
      value <<= 1;
       
      digitalWrite(DAC_SCLK, HIGH);
      digitalWrite(DAC_SCLK, LOW);
   }   // for i
}
