//Please don't detele this lines
//This code from DIYMAN & CSHOP Channel
//Youtube channel: https://www.youtube.com/channel/UC6Hx5M04nFHhg6W7igUGrQA
//Facebook: https://www.facebook.com/pg/CShopPR/posts/
//email: buncalockg@gmail.com

#include <EEPROM.h>

#define inSensor 12
#define inButtonSetup 8

#define LedStand 13
#define LedPump 11
#define Buzzer 10
#define outPump 9

#define TimeToBegin 500

//int val1, val2=-1;
char buf[64];
int count = 0;
int TimeToPump=2000;

//int _valSetup=HIGH, _valSensor=LOW;
//int valSetup=HIGH, valSensor=LOW;

void EEPROMWriteInt(int address, int value)
{
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  
  EEPROM.update(address, two);
  EEPROM.update(address + 1, one);
}
 
int EEPROMReadInt(int address)
{
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);
 
  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}

void setup() {
  // put your setup code here, to run once:

  TimeToPump = EEPROMReadInt(0);
  if(TimeToPump <= 0) TimeToPump = 1000;

  Serial.begin(115200);
  Serial.println("HELLO");
  Serial.println(TimeToPump);
  
  pinMode(inSensor, INPUT_PULLUP);
  pinMode(inButtonSetup, INPUT);
  
  pinMode(LedPump, OUTPUT);
  pinMode(LedStand, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(outPump, OUTPUT);
  

  digitalWrite(Buzzer, LOW);
  digitalWrite(LedPump, LOW);
  digitalWrite(LedStand, HIGH);
  digitalWrite(outPump, LOW);

  digitalWrite(inButtonSetup, HIGH);
  //valSetup = _valSetup = HIGH;

  //Báo lỗi khi cảm biến bị nhiễu lúc mới mở máy
  while(digitalRead(inSensor) == LOW)
  {
    digitalWrite(LedPump, HIGH);
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedPump, LOW);
    delay(500);
  }
}


unsigned long time;

void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(inButtonSetup) == LOW) // Nhấn nút setup đếm thời gian bơm và đồng thời cũng bật máy bơm
  { 
      unsigned long time2 = millis();
      bool bCanPump = true;
      int c = 0;
      while(millis() - time2 < TimeToBegin)
      {
        if(digitalRead(inButtonSetup) == HIGH)
        {
          bCanPump = false;
          break;
        }
        if(c%2 == 0)
        {
          digitalWrite(LedPump, HIGH);
          digitalWrite(Buzzer, HIGH);
        }
          else
          {
            digitalWrite(LedPump, LOW);
            digitalWrite(Buzzer, LOW);
          }
        delay(50);
        c++;
      }
      digitalWrite(LedPump, LOW);
      digitalWrite(Buzzer, LOW);
    
        
    if(bCanPump)
    {
      time = millis();

      digitalWrite(LedPump, HIGH);
      digitalWrite(Buzzer, HIGH);
      digitalWrite(outPump, HIGH); //pump
      Serial.println("Pump and time counting up ...");

      while(digitalRead(inButtonSetup) == LOW)
      {
        delay(1);
      }
      // Tắt máy bơm và lưu thời gian bơm.
      TimeToPump = millis() - time;
      EEPROMWriteInt(0, TimeToPump);
      digitalWrite(Buzzer, LOW);
      digitalWrite(LedPump, LOW);
      digitalWrite(outPump, LOW); //stop pump
      sprintf(buf, "End pump and save time pump. %d", (TimeToPump));
      Serial.println(buf);
      delay(100);
    }
    else
    {
      Serial.println("Break setup.");
    }
  }

 
  if(digitalRead(inSensor) == LOW)
  {
      Serial.println("Wating to Pump.");
      
      unsigned long time2 = millis();
      bool bCanPump = true;
      int c = 0;
      while(millis() - time2 < TimeToBegin)
      {
        if(digitalRead(inSensor) == HIGH)
        {
          bCanPump = false;
          break;
        }
        if(c%2 == 0)
        {
          digitalWrite(LedPump, HIGH);
          digitalWrite(Buzzer, HIGH);
        }
          else
          {
            digitalWrite(LedPump, LOW);
            digitalWrite(Buzzer, LOW);
          }
        delay(50);
        c++;
      }
      digitalWrite(LedPump, LOW);
      digitalWrite(Buzzer, LOW);


      if(bCanPump==false)
      {
        Serial.println("Break pump.");
      }
      else
      {
            unsigned long time2 = millis();
            Serial.println("Pumping ..");
    
            digitalWrite(LedPump, HIGH);
            digitalWrite(outPump, HIGH); //pump
            

            
            while(digitalRead(inSensor) == LOW && millis() - time2 < TimeToPump)
            {
              delay(1);
            }
            
            digitalWrite(outPump, LOW); //stop pump

            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            digitalWrite(LedPump, LOW);

            Serial.println("Stop pump.");
    
            while(digitalRead(inSensor) == LOW)
            {
              delay(1);
            }
       }
    }
  delay(10);
}
