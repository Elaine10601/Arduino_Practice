 // ------------------------------------------------
// 範例: PulseSensor_ex1.ino
// 功能: LCD顯示心跳頻率,LED模擬心跳顯示 
//       當心跳頻率大於90時,打開BEEP 
//       透過藍芽傳送心跳頻率至手機,資料格式為1個BYTE(心跳頻率)
//       透過USB傳送心跳曲線與頻率至電腦,PC端需透過DEMO軟體顯示,
//       1.執行processing.exe
//       2.OPEN > 開啟舊檔 > 選擇檔名PulseSensorAmpd_Processing_1dot1.pde 
//       3.按下RUN按鈕 開始顯示
// 
// Copyright(c) 2015 by CHIRKAL 僑高科技
// E-mail: service@chirkal.com.tw
// Web: http://www.chirkal.com.tw
//
// Support: Atmel SAM3X8E  
//
// 接線說明  
// +----------+-----------+----------+
// | Arduino  | IO編號    | 20x2 LCD |
// +----------+-----------+----------+
// | D8       |  25        | E        |
// | D9       |  26        | R/W      |
// | D10      |  27       | RS       |
// | D11      |  28       |          |
// | D12      |  29       | D4       |
// | D13      |  30       | D5       |
// | D14      |  31       | D6       |
// | D15      |  32       | D7       |
// +----------+-----------+----------+
// | Arduino  | IO編號    | 模組-藍芽|
// +----------+-----------+----------+
// | Tx2      | 33        | TxD      | 
// | Rx2      | 34        | RxD      | 
// +----------+-----------+----------+
// | Arduino  | IO編號    | LED      |
// +----------+-----------+----------+
// | D7       | 24        | LED1       | 
// +----------+-----------+----------+
// | Arduino  | IO編號    | BEEP     |
// +----------+-----------+----------+
// | D6       | 23        | BEEP     | 
// +----------+-----------+----------+
// Date: 2015-09-21
//-------------------------------------------------

#include "DueTimer.h"
#include "LiquidCrystalEx.h"

LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15); // initialize the library with the numbers of the interface pins
int R_W = 9;

//  VARIABLES
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int beepPin = 6;                 // pin to blink led at each beat
int fadePin  = 7;                 // pin to do fancy classy fading blink at each beat

int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

volatile int rate[10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find IBI
volatile int P =512;                      // used to find peak in pulse wave, seeded
volatile int T = 512;                     // used to find trough in pulse wave, seeded
volatile int thresh = 512;                // used to find instant moment of heart beat, seeded
volatile int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM

// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


void setup()
{
   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
      
   pinMode(beepPin,OUTPUT);          // BEEP
   pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
   
   Serial.begin(115200);             
   Timer3.attachInterrupt(myHandler);
   Timer3.start(2000);               // Calls every 2ms 

   lcd.begin(20, 2);      
   lcd.print("Pulse ");  
   
   Serial2.begin(9600);                //啟動串列通信-藍芽傳送     
}



void loop()
{
   sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
   if (QS == true)
   {                       // Quantified Self flag is true when arduino finds a heartbeat
      fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
      sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
      sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
      QS = false;                      // reset the Quantified Self flag for next time
      
      lcd.setCursor(0, 2);      
      lcd.print("      ");        
      lcd.setCursor(0, 2);      
      lcd.print(BPM);    
      if(BPM > 75)
         digitalWrite(beepPin,HIGH);               
      else   
         digitalWrite(beepPin,LOW);
         
      //Serial2.write(BPM);              //使用藍芽傳送心跳頻率     
      Serial2.println(BPM);               
   }
  
   ledFadeToBeat();
  
   delay(20);                             //  take a break
}


void ledFadeToBeat()
{
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
}


void sendDataToProcessing(char symbol, int data )
{
    Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
    Serial.println(data);                // the data to send culminating in a carriage return
}

void myHandler()          //TIMER 2ms
{                        
  Signal = analogRead(pulsePin);              // read the Pulse Sensor 
  sampleCounter += 2;                         // keep track of the time in mS with this variable
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

    //  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                        // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse

      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat)
      {                        // if this is the second beat, if secondBeat == TRUE
         secondBeat = false;                  // clear secondBeat flag
         for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                      
         }
      }

      if(firstBeat)
      {                         // if it's the first time we found a beat, if firstBeat == TRUE
         firstBeat = false;                   // clear firstBeat flag
         secondBeat = true;                   // set the second beat flag
         return;                              // IBI value is unreliable so discard it
      }   


      // keep a running total of the last 10 IBI values
      word runningTotal = 0;                  // clear the runningTotal variable    

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over

    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (N > 2500){                           // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }
  
}












// 範例: PulseSensor_ex1-1.ino
// 功能:手指接上心率偵測模組，能於Arduino實驗平台上的LCD顯示心跳頻率。 
// Support: Atmel SAM3X8E  

#include "DueTimer.h"
#include "LiquidCrystalEx.h"

LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15); //初始化LCD接腳
int R_W = 9;

//計算心率所需變數定義
int pulsePin = 0;                 //脈搏感測器，紫色線接analog pin 0

//以下參數宣告成volatile是因為在中斷服務程式時會使用到
volatile int rate[10];                     //陣列rate[10]保存最近的十筆IBI值
volatile unsigned long sampleCounter = 0;  //用於追蹤換算heart beat時間
volatile unsigned long lastBeatTime = 0;   //與sampleCounter併用找IBI(Inter Beat Interval)
volatile int P =512;                       //用於找脈搏波的波峰值，512為初始值
volatile int T = 512;                      //用於找脈搏波的波谷值，512為初始值
volatile int thresh = 512;                 //用於找脈搏波振幅的臨界值，512初始值
volatile int amp = 100;                    //脈搏波的振幅值，100為初始值
volatile boolean firstBeat = true;         //用於一開始找尋合理的心率BPM值，firstBeat不可靠，忽略不計
volatile boolean secondBeat = false;       //用於一開始找尋合理的心率BPM值，從secondBeat開始計數

volatile int BPM;                          //用於儲存BPM
volatile int Signal;                       //用於儲存剛收到的脈搏波原始資料
volatile int IBI = 600;                    //儲存心跳間的時間值，600為初始值，即1.2秒
volatile boolean Pulse = false;            //為true表示找到一個有效脈搏波
volatile boolean QS = false;               //找到有效心跳則為true

void setup()
{
   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
      
   Timer3.attachInterrupt(myHandler);  //每2ms發生時間中斷
   Timer3.start(2000);                

   lcd.begin(20, 2);      
   lcd.print("Pulse ");  
}

void loop()
{
   if (QS == true)          //當找到有效心率時，執行此程式
   {                       
      QS = false;          //重置the Quantified Self flag於下次測試使用
      
      lcd.setCursor(0, 2);      
      lcd.print("      ");        
      lcd.setCursor(0, 2);      
      lcd.print(BPM);              
   }  
   delay(20);                             
}

void myHandler()          //時間中斷副程式
{                        
  Signal = analogRead(pulsePin);              //讀取Pulse Sensor資料
  sampleCounter += 2;                         //保持追蹤取樣數，可轉換成heart beat發生時間
  int N = sampleCounter - lastBeatTime;       //心率取樣間隔時間，可用來判斷是否為誤判
  
  //找波谷值與波峰值
  if(Signal < thresh && N > (IBI/5)*3)        //透過設定threshold值與延遲(IBI/5)*3，避免重搏波造成誤判
  {
    if (Signal < T)                           //T為谷值
    {
      T=Signal;
    }
  }

  if(Signal > thresh && Signal > P)          //thresh condition避免雜訊誤判
  {
    P = Signal;                              //P為峰值
  }                                        

  //以下將找尋心率值
  if (N > 250)                              //避免高頻雜訊，250*2ms=500ms
  {
    if ((Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3))
    {        
      Pulse = true;                       //為true表示，有一個pulse

      IBI = sampleCounter - lastBeatTime;   //beats兩兩相隔數值，以2mS為倍率
      lastBeatTime = sampleCounter;         

      //於程式一開始時找合理10筆BPM值為啓始
      if(secondBeat)                        //secondBeat == TRUE，則為第二次心跳
      {                        
         secondBeat = false;              //清除secondBeat旗標
         for(int i=0; i<=9; i++)          //一開始餵入10筆合理的BPM
         {
          rate[i] = IBI;                      
         }         
      }

      if(firstBeat)                         //firstBeat == TRUE，表示第一次找到心跳值
      {                         
         firstBeat = false;                 //清除firstBeat旗標
         secondBeat = true;                 //設定secondBeat旗標
         return;                            //忽略第一次的心跳值
      }   

      //維持最近十筆IBI值於runningTotal
      word runningTotal = 0;                //清除runningTotal

      for(int i=0; i<=8; i++)               //將9筆心率值加起來
      {
        rate[i] = rate[i+1];
        runningTotal += rate[i];
      }

      rate[9] = IBI;                        //每次加入最新BPM，當作第10筆資料
      runningTotal += rate[9];              
      runningTotal /= 10;                   //平均最近10筆的IBI值
      BPM = 60000/runningTotal;             //每分鐘心跳值為BPM
      QS = true;                            //設定Quantified Self flag 
    }                       
  }

  if (Signal < thresh && Pulse == true)     //如果擷取訊號值變小，表示beat已過
  {
    Pulse = false;                         //重置Pulse flag
    amp = P - T;                           //取得脈搏波的振幅值
    thresh = amp/2 + T;                    //設定谷值thresh在振幅值的一半
    P = thresh;                            //重置P與T值
    T = thresh;
  }

  if (N > 2500)                            //如果2.5秒都沒有找到beat，則全部重置為預設值
  {
    thresh = 512;                          
    P = 512;                               
    T = 512;                               
    lastBeatTime = sampleCounter;          //設定lastBeatTime為最近偵測值
    firstBeat = true;                      //設定避免雜訊誤判
    secondBeat = false;
  }  
}


// 範例: PulseSensor_ex1-2.ino
// 功能:手指接上心率偵測模組，能於Arduino實驗平台上的LCD顯示心跳頻率。
//      而當心跳頻率大於某一設定值時，蜂鳴器響起。
// Support: Atmel SAM3X8E  

#include "DueTimer.h"
#include "LiquidCrystalEx.h"

LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15); //初始化LCD接腳
int R_W = 9;

int pulsePin = 0;                 //脈搏感測器，紫色線接analog pin 0
int beepPin = 6;                  //心跳警示腳位

//以下參數宣告成volatile是因為在中斷服務程式時會使用到
volatile int rate[10];                     //陣列rate[10]保存最近的十筆IBI值
volatile unsigned long sampleCounter = 0;  //用於追蹤換算heart beat時間
volatile unsigned long lastBeatTime = 0;   //與sampleCounter併用找IBI(Inter Beat Interval)
volatile int P =512;                       //用於找脈搏波的波峰值，512為初始值
volatile int T = 512;                      //用於找脈搏波的波谷值，512為初始值
volatile int thresh = 512;                 //用於找脈搏波振幅的臨界值，512初始值
volatile int amp = 100;                    //脈搏波的振幅值，100為初始值
volatile boolean firstBeat = true;         //用於一開始找尋合理的心率BPM值，firstBeat不可靠，忽略不計
volatile boolean secondBeat = false;       //用於一開始找尋合理的心率BPM值，從secondBeat開始計數

volatile int BPM;                          //用於儲存BPM
volatile int Signal;                       //用於儲存剛收到的脈搏波原始資料
volatile int IBI = 600;                    //儲存心跳間的時間值，600為初始值，即1.2秒
volatile boolean Pulse = false;            //為true表示找到一個有效脈搏波
volatile boolean QS = false;               //找到有效心跳則為true

void setup()
{
   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
   pinMode(beepPin,OUTPUT);                 // 心跳警示腳位宣告
      
   Timer3.attachInterrupt(myHandler);       //每2ms發生時間中斷
   Timer3.start(2000);                

   lcd.begin(20, 2);      
   lcd.print("Pulse ");  
}

void loop()
{
   if (QS == true)          //當找到有效心率時，執行此程式
   {                       
      QS = false;          //重置the Quantified Self flag於下次測試使用
      
      lcd.setCursor(0, 2);      
      lcd.print("      ");        
      lcd.setCursor(0, 2);      
      lcd.print(BPM);
      if(BPM > 90)   //啟動心跳警示
        digitalWrite(beepPin,HIGH);               
      else   
        digitalWrite(beepPin,LOW);       
   }  
   delay(20);                             
}

void myHandler()          //時間中斷副程式
{                        
  Signal = analogRead(pulsePin);              //讀取Pulse Sensor資料
  sampleCounter += 2;                         //保持追蹤取樣數，可轉換成heart beat發生時間
  int N = sampleCounter - lastBeatTime;       //心率取樣間隔時間，可用來判斷是否為誤判
  
  //找波谷值與波峰值
  if(Signal < thresh && N > (IBI/5)*3)        //透過設定threshold值與延遲(IBI/5)*3，避免重搏波造成誤判
  {
    if (Signal < T)                           //T為谷值
    {
      T=Signal;
    }
  }

  if(Signal > thresh && Signal > P)          //thresh condition避免雜訊誤判
  {
    P = Signal;                              //P為峰值
  }                                        

  //以下將找尋心率值
  if (N > 250)                              //避免高頻雜訊，250*2ms=500ms
  {
    if ((Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3))
    {        
      Pulse = true;                       //為true表示，有一個pulse

      IBI = sampleCounter - lastBeatTime;   //beats兩兩相隔數值，以2mS為倍率
      lastBeatTime = sampleCounter;         

      //於程式一開始時找合理10筆BPM值為啓始
      if(secondBeat)                        //secondBeat == TRUE，則為第二次心跳
      {                        
         secondBeat = false;              //清除secondBeat旗標
         for(int i=0; i<=9; i++)          //一開始餵入10筆合理的BPM
         {
          rate[i] = IBI;                      
         }         
      }

      if(firstBeat)                         //firstBeat == TRUE，表示第一次找到心跳值
      {                         
         firstBeat = false;                 //清除firstBeat旗標
         secondBeat = true;                 //設定secondBeat旗標
         return;                            //忽略第一次的心跳值
      }   

      //維持最近十筆IBI值於runningTotal
      word runningTotal = 0;                //清除runningTotal

      for(int i=0; i<=8; i++)               //將9筆心率值加起來
      {
        rate[i] = rate[i+1];
        runningTotal += rate[i];
      }

      rate[9] = IBI;                        //每次加入最新BPM，當作第10筆資料
      runningTotal += rate[9];              
      runningTotal /= 10;                   //平均最近10筆的IBI值
      BPM = 60000/runningTotal;             //每分鐘心跳值為BPM
      QS = true;                            //設定Quantified Self flag 
    }                       
  }

  if (Signal < thresh && Pulse == true)     //如果擷取訊號值變小，表示beat已過
  {
    Pulse = false;                         //重置Pulse flag
    amp = P - T;                           //取得脈搏波的振幅值
    thresh = amp/2 + T;                    //設定谷值thresh在振幅值的一半
    P = thresh;                            //重置P與T值
    T = thresh;
  }

  if (N > 2500)                            //如果2.5秒都沒有找到beat，則全部重置為預設值
  {
    thresh = 512;                          
    P = 512;                               
    T = 512;                               
    lastBeatTime = sampleCounter;          //設定lastBeatTime為最近偵測值
    firstBeat = true;                      //設定避免雜訊誤判
    secondBeat = false;
  }  
}


// 範例: PulseSensor_ex1-3.ino
// 功能: 手指接上心率偵測模組，能於Arduino實驗平台上的LCD顯示心跳頻率。
//       而當心跳頻率大於某一設定值時，蜂鳴器響起。
//       當心率偵測模組於有效偵測時LED閃爍，否則LED熄滅。
// Support: Atmel SAM3X8E  

#include "DueTimer.h"
#include "LiquidCrystalEx.h"

LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15); //初始化LCD接腳
int R_W = 9;

int pulsePin = 0;                 //脈搏感測器，紫色線接analog pin 0
int beepPin = 6;                  //心跳警示腳位
int fadePin  = 7;                 //偵測心跳有效與否LED腳位
int fadeRate = 0;               

//以下參數宣告成volatile是因為在中斷服務程式時會使用到
volatile int rate[10];                     //陣列rate[10]保存最近的十筆IBI值
volatile unsigned long sampleCounter = 0;  //用於追蹤換算heart beat時間
volatile unsigned long lastBeatTime = 0;   //與sampleCounter併用找IBI(Inter Beat Interval)
volatile int P =512;                       //用於找脈搏波的波峰值，512為初始值
volatile int T = 512;                      //用於找脈搏波的波谷值，512為初始值
volatile int thresh = 512;                 //用於找脈搏波振幅的臨界值，512初始值
volatile int amp = 100;                    //脈搏波的振幅值，100為初始值
volatile boolean firstBeat = true;         //用於一開始找尋合理的心率BPM值，firstBeat不可靠，忽略不計
volatile boolean secondBeat = false;       //用於一開始找尋合理的心率BPM值，從secondBeat開始計數

volatile int BPM;                          //用於儲存BPM
volatile int Signal;                       //用於儲存剛收到的脈搏波原始資料
volatile int IBI = 600;                    //儲存心跳間的時間值，600為初始值，即1.2秒
volatile boolean Pulse = false;            //為true表示找到一個有效脈搏波
volatile boolean QS = false;               //找到有效心跳則為true

void setup()
{
   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
   pinMode(beepPin,OUTPUT);                 // 心跳警示腳位宣告
   pinMode(fadePin,OUTPUT);
      
   Timer3.attachInterrupt(myHandler);       //每2ms發生時間中斷
   Timer3.start(2000);                

   lcd.begin(20, 2);      
   lcd.print("Pulse ");  
}

void loop()
{
   if (QS == true)          //當找到有效心率時，執行此程式
   {                       
      QS = false;          //重置the Quantified Self flag於下次測試使用
      fadeRate = 255;
      
      lcd.setCursor(0, 2);      
      lcd.print("      ");        
      lcd.setCursor(0, 2);      
      lcd.print(BPM);
      if(BPM > 90)   //啟動心跳警示
        digitalWrite(beepPin,HIGH);               
      else   
        digitalWrite(beepPin,LOW);       
   }  
   ledFadeToBeat();
   delay(20);                             
}

void myHandler()          //時間中斷副程式
{                        
  Signal = analogRead(pulsePin);              //讀取Pulse Sensor資料
  sampleCounter += 2;                         //保持追蹤取樣數，可轉換成heart beat發生時間
  int N = sampleCounter - lastBeatTime;       //心率取樣間隔時間，可用來判斷是否為誤判
  
  //找波谷值與波峰值
  if(Signal < thresh && N > (IBI/5)*3)        //透過設定threshold值與延遲(IBI/5)*3，避免重搏波造成誤判
  {
    if (Signal < T)                           //T為谷值
    {
      T=Signal;
    }
  }

  if(Signal > thresh && Signal > P)          //thresh condition避免雜訊誤判
  {
    P = Signal;                              //P為峰值
  }                                        

  //以下將找尋心率值
  if (N > 250)                              //避免高頻雜訊，250*2ms=500ms
  {
    if ((Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3))
    {        
      Pulse = true;                       //為true表示，有一個pulse

      IBI = sampleCounter - lastBeatTime;   //beats兩兩相隔數值，以2mS為倍率
      lastBeatTime = sampleCounter;         

      //於程式一開始時找合理10筆BPM值為啓始
      if(secondBeat)                        //secondBeat == TRUE，則為第二次心跳
      {                        
         secondBeat = false;              //清除secondBeat旗標
         for(int i=0; i<=9; i++)          //一開始餵入10筆合理的BPM
         {
          rate[i] = IBI;                      
         }         
      }

      if(firstBeat)                         //firstBeat == TRUE，表示第一次找到心跳值
      {                         
         firstBeat = false;                 //清除firstBeat旗標
         secondBeat = true;                 //設定secondBeat旗標
         return;                            //忽略第一次的心跳值
      }   

      //維持最近十筆IBI值於runningTotal
      word runningTotal = 0;                //清除runningTotal

      for(int i=0; i<=8; i++)               //將9筆心率值加起來
      {
        rate[i] = rate[i+1];
        runningTotal += rate[i];
      }

      rate[9] = IBI;                        //每次加入最新BPM，當作第10筆資料
      runningTotal += rate[9];              
      runningTotal /= 10;                   //平均最近10筆的IBI值
      BPM = 60000/runningTotal;             //每分鐘心跳值為BPM
      QS = true;                            //設定Quantified Self flag 
    }                       
  }

  if (Signal < thresh && Pulse == true)     //如果擷取訊號值變小，表示beat已過
  {
    Pulse = false;                         //重置Pulse flag
    amp = P - T;                           //取得脈搏波的振幅值
    thresh = amp/2 + T;                    //設定谷值thresh在振幅值的一半
    P = thresh;                            //重置P與T值
    T = thresh;
  }

  if (N > 2500)                            //如果2.5秒都沒有找到beat，則全部重置為預設值
  {
    thresh = 512;                          
    P = 512;                               
    T = 512;                               
    lastBeatTime = sampleCounter;          //設定lastBeatTime為最近偵測值
    firstBeat = true;                      //設定避免雜訊誤判
    secondBeat = false;
  }  
}

void ledFadeToBeat()
{
    fadeRate -= 15;                        //設定LED fade value
    fadeRate = constrain(fadeRate,0,255);  //LED變化值範圍
    analogWrite(fadePin,fadeRate);         //fade LED
}


// 範例: PulseSensor_ex1-4.ino
// 功能: 手指接上心率偵測模組，能於Arduino實驗平台上的LCD顯示心跳頻率。
//       而當心跳頻率大於某一設定值時，蜂鳴器響起。
//       當心率偵測模組於有效偵測時LED閃爍，否則LED熄滅。
//       並透過藍芽模組將心跳頻率值傳至手機APP並顯示。 
// Support: Atmel SAM3X8E  

#include "DueTimer.h"
#include "LiquidCrystalEx.h"

LiquidCrystalEx lcd(10, 8, 12, 13, 14, 15); //初始化LCD接腳
int R_W = 9;

int pulsePin = 0;                 //脈搏感測器，紫色線接analog pin 0
int beepPin = 6;                  //心跳警示腳位
int fadePin  = 7;                 //偵測心跳有效與否LED腳位
int fadeRate = 0;               

//以下參數宣告成volatile是因為在中斷服務程式時會使用到
volatile int rate[10];                     //陣列rate[10]保存最近的十筆IBI值
volatile unsigned long sampleCounter = 0;  //用於追蹤換算heart beat時間
volatile unsigned long lastBeatTime = 0;   //與sampleCounter併用找IBI(Inter Beat Interval)
volatile int P =512;                       //用於找脈搏波的波峰值，512為初始值
volatile int T = 512;                      //用於找脈搏波的波谷值，512為初始值
volatile int thresh = 512;                 //用於找脈搏波振幅的臨界值，512初始值
volatile int amp = 100;                    //脈搏波的振幅值，100為初始值
volatile boolean firstBeat = true;         //用於一開始找尋合理的心率BPM值，firstBeat不可靠，忽略不計
volatile boolean secondBeat = false;       //用於一開始找尋合理的心率BPM值，從secondBeat開始計數

volatile int BPM;                          //用於儲存BPM
volatile int Signal;                       //用於儲存剛收到的脈搏波原始資料
volatile int IBI = 600;                    //儲存心跳間的時間值，600為初始值，即1.2秒
volatile boolean Pulse = false;            //為true表示找到一個有效脈搏波
volatile boolean QS = false;               //找到有效心跳則為true

void setup()
{
   pinMode(R_W, OUTPUT);
   digitalWrite(R_W, LOW);
   pinMode(beepPin,OUTPUT);                 // 心跳警示腳位宣告
   pinMode(fadePin,OUTPUT);
      
   Timer3.attachInterrupt(myHandler);       //每2ms發生時間中斷
   Timer3.start(2000);                

   lcd.begin(20, 2);      
   lcd.print("Pulse ");

   Serial2.begin(9600);                     //啟動串列通信-藍芽傳送
}

void loop()
{
   if (QS == true)          //當找到有效心率時，執行此程式
   {                       
      QS = false;          //重置the Quantified Self flag於下次測試使用
      fadeRate = 255;
      
      lcd.setCursor(0, 2);      
      lcd.print("      ");        
      lcd.setCursor(0, 2);      
      lcd.print(BPM);
      if(BPM > 90)   //啟動心跳警示
        digitalWrite(beepPin,HIGH);               
      else   
        digitalWrite(beepPin,LOW);

      //Serial2.println(BPM);             //使用藍芽傳送心跳頻率
      Serial2.write(BPM);
   }  
   ledFadeToBeat();
   delay(20);                             
}

void myHandler()          //時間中斷副程式
{                        
  Signal = analogRead(pulsePin);              //讀取Pulse Sensor資料
  sampleCounter += 2;                         //保持追蹤取樣數，可轉換成heart beat發生時間
  int N = sampleCounter - lastBeatTime;       //心率取樣間隔時間，可用來判斷是否為誤判
  
  //找波谷值與波峰值
  if(Signal < thresh && N > (IBI/5)*3)        //透過設定threshold值與延遲(IBI/5)*3，避免重搏波造成誤判
  {
    if (Signal < T)                           //T為谷值
    {
      T=Signal;
    }
  }

  if(Signal > thresh && Signal > P)          //thresh condition避免雜訊誤判
  {
    P = Signal;                              //P為峰值
  }                                        

  //以下將找尋心率值
  if (N > 250)                              //避免高頻雜訊，250*2ms=500ms
  {
    if ((Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3))
    {        
      Pulse = true;                       //為true表示，有一個pulse

      IBI = sampleCounter - lastBeatTime;   //beats兩兩相隔數值，以2mS為倍率
      lastBeatTime = sampleCounter;         

      //於程式一開始時找合理10筆BPM值為啓始
      if(secondBeat)                        //secondBeat == TRUE，則為第二次心跳
      {                        
         secondBeat = false;              //清除secondBeat旗標
         for(int i=0; i<=9; i++)          //一開始餵入10筆合理的BPM
         {
          rate[i] = IBI;                      
         }         
      }

      if(firstBeat)                         //firstBeat == TRUE，表示第一次找到心跳值
      {                         
         firstBeat = false;                 //清除firstBeat旗標
         secondBeat = true;                 //設定secondBeat旗標
         return;                            //忽略第一次的心跳值
      }   

      //維持最近十筆IBI值於runningTotal
      word runningTotal = 0;                //清除runningTotal

      for(int i=0; i<=8; i++)               //將9筆心率值加起來
      {
        rate[i] = rate[i+1];
        runningTotal += rate[i];
      }

      rate[9] = IBI;                        //每次加入最新BPM，當作第10筆資料
      runningTotal += rate[9];              
      runningTotal /= 10;                   //平均最近10筆的IBI值
      BPM = 60000/runningTotal;             //每分鐘心跳值為BPM
      QS = true;                            //設定Quantified Self flag 
    }                       
  }

  if (Signal < thresh && Pulse == true)     //如果擷取訊號值變小，表示beat已過
  {
    Pulse = false;                         //重置Pulse flag
    amp = P - T;                           //取得脈搏波的振幅值
    thresh = amp/2 + T;                    //設定谷值thresh在振幅值的一半
    P = thresh;                            //重置P與T值
    T = thresh;
  }

  if (N > 2500)                            //如果2.5秒都沒有找到beat，則全部重置為預設值
  {
    thresh = 512;                          
    P = 512;                               
    T = 512;                               
    lastBeatTime = sampleCounter;          //設定lastBeatTime為最近偵測值
    firstBeat = true;                      //設定避免雜訊誤判
    secondBeat = false;
  }  
}

void ledFadeToBeat()
{
    fadeRate -= 15;                        //設定LED fade value
    fadeRate = constrain(fadeRate,0,255);  //LED變化值範圍
    analogWrite(fadePin,fadeRate);         //fade LED
}
