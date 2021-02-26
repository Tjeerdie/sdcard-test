// #include <logFile.h>
#include "SD_logger.h"
#include "SPI.h"
#include "globals.h"

uint8_t RandomBuffer[8192]; 
#define SD_LOGGING

#define RTC_ENABLED

#ifdef STM32F407xx
  #define SD_LOGGING //SD logging enabled for STM32F407 because it has the SDIO interface
  #define SD_LIB_H "src/STM32SD/STM32SD.h"
#endif

void setup()
{
  Serial.begin(115200);
  // Serial1.begin(115200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
    /* Get the rtc object */
  rtc.setClockSource(STM32RTC::LSE_CLOCK); //Initialize external clock for RTC. That is the only clock running of VBAT
  rtc.begin(); // initialize RTC 24H format
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW);
  
  SDinit();
  SDopenLogFile();
  Serial.print("sd_status=");
  Serial.println(currentStatus.sd_status);
  Serial.println("Setup done. if Status=8 then error");
}

void loop()
{
  uint32_t starttime = millis();

  digitalWrite(LED_BUILTIN, LOW);
  SDwriteLogEntry();
  digitalWrite(LED_BUILTIN, HIGH);

  //Put some random data in de currentStatus so the logger is doing something
  currentStatus.runSecs += 1;
  currentStatus.spark += 3;


  //do somthing else for until 30 milliseconds have passed.
  while (millis()-starttime<30)
  {
    if(Serial.available() > 0)
    {
      Serial.println("Key pressed");
      switch(Serial.read())
      {
      case 'O':
        SDopenLogFile();
        Serial.print("sd_status=");
        Serial.println(currentStatus.sd_status);
        Serial.println("Log file opend");
        break;   
      case 'W':
        {
        }
        break;
      case 'C':
        SDcloseLogFile();
        Serial.print("sd_status=");
        Serial.println(currentStatus.sd_status);
        Serial.println("Log file closed");
        break;     
      } 
    }
  }
}
