#include "globals.h"
struct statuses currentStatus; /**< The master global status struct. Contains all values that are updated frequently and used across modules */
struct config13 configPage13;
// //This function is called by the fatfs library to get the current time if a new file is created
// uint32_t get_fattime (void){
//   uint32_t time;
//   time = (((uint32_t)rtc.getYear() + 20)<< 25) | (((uint32_t)rtc.getMonth())<< 21) | (((uint32_t)rtc.getDay())<< 16) | (((uint32_t)rtc.getHours())<< 11) | (((uint32_t)rtc.getMinutes())<< 5) | (((uint32_t)rtc.getSeconds())>>1);
//   return time;
// }


uint8_t rtc_getSecond()
{
  uint8_t tempSecond = 0;
#ifdef RTC_ENABLED
  #if defined(CORE_TEENSY)
    tempSecond = second();
  #elif defined(CORE_STM32)
    tempSecond = rtc.getSeconds();
  #endif
#endif
  return tempSecond;
}

uint8_t rtc_getMinute()
{
  uint8_t tempMinute = 0;
#ifdef RTC_ENABLED
  #if defined(CORE_TEENSY)
    tempMinute = minute();
  #elif defined(CORE_STM32)
    tempMinute = rtc.getMinutes();
  #endif
#endif
  return tempMinute;
}

uint8_t rtc_getHour()
{
  uint8_t tempHour = 0;
#ifdef RTC_ENABLED
  #if defined(CORE_TEENSY)
    tempHour = hour();
  #elif defined(CORE_STM32)
    tempHour = rtc.getHours();
  #endif
#endif
  return tempHour;
}

uint8_t rtc_getDay()
{
  uint8_t tempDay = 0;
#ifdef RTC_ENABLED
  #if defined(CORE_TEENSY)
    tempDay = day();
  #elif defined(CORE_STM32)
    tempDay = rtc.getDay();
  #endif
#endif
  return tempDay;
}

uint8_t rtc_getDOW()
{
  uint8_t dow = 0;
#ifdef RTC_ENABLED
  #if defined(CORE_TEENSY)
    dow = weekday();
  #elif defined(CORE_STM32)
    dow = rtc.getWeekDay();
  #endif
#endif
  return dow;
}

uint8_t rtc_getMonth()
{
  uint8_t tempMonth = 0;
#ifdef RTC_ENABLED
  #if defined(CORE_TEENSY)
    tempMonth = month();
  #elif defined(CORE_STM32)
    tempMonth = rtc.getMonth();
  #endif
#endif
  return tempMonth;
}

uint16_t rtc_getYear()
{
  uint16_t tempYear = 0;
#ifdef RTC_ENABLED
  #if defined(CORE_TEENSY)
    tempYear = year();
  #elif defined(CORE_STM32)
    tempYear = rtc.getYear();
  #endif
#endif
  return tempYear;
}

void rtc_setTime(byte second, byte minute, byte hour, byte day, byte month, uint16_t year)
{
#ifdef RTC_ENABLED
  #if defined(CORE_TEENSY)
    setTime(second, minute, hour, day, month, year);
  #elif defined(CORE_STM32)
    rtc.setTime(hour, minute, second);
    rtc.setDate(day, month, year);
  #endif
#endif
}

byte getStatusEntry(uint16_t byteNum)
{
  byte statusValue = 0;

  switch(byteNum)
  {
    case 0: statusValue = currentStatus.secl; break; //secl is simply a counter that increments each second. Used to track unexpected resets (Which will reset this count to 0)
    case 1: statusValue = currentStatus.status1; break; //status1 Bitfield
    case 2: statusValue = currentStatus.engine; break; //Engine Status Bitfield
    case 3: statusValue = currentStatus.syncLossCounter; break;
    case 4: statusValue = lowByte(currentStatus.MAP); break; //2 bytes for MAP
    case 5: statusValue = highByte(currentStatus.MAP); break;
    case 6: statusValue = (byte)(currentStatus.IAT); break; //mat
    case 7: statusValue = (byte)(currentStatus.coolant); break; //Coolant ADC
    case 8: statusValue = currentStatus.batCorrection; break; //Battery voltage correction (%)
    case 9: statusValue = currentStatus.battery10; break; //battery voltage
    case 10: statusValue = currentStatus.O2; break; //O2
    case 11: statusValue = currentStatus.egoCorrection; break; //Exhaust gas correction (%)
    case 12: statusValue = currentStatus.iatCorrection; break; //Air temperature Correction (%)
    case 13: statusValue = currentStatus.wueCorrection; break; //Warmup enrichment (%)
    case 14: statusValue = lowByte(currentStatus.RPM); break; //rpm HB
    case 15: statusValue = highByte(currentStatus.RPM); break; //rpm LB
    case 16: statusValue = (byte)(currentStatus.AEamount >> 1); break; //TPS acceleration enrichment (%) divided by 2 (Can exceed 255)
    case 17: statusValue = lowByte(currentStatus.corrections); break; //Total GammaE (%)
    case 18: statusValue = highByte(currentStatus.corrections); break; //Total GammaE (%)
    case 19: statusValue = currentStatus.VE1; break; //VE 1 (%)
    case 20: statusValue = currentStatus.VE2; break; //VE 2 (%)
    case 21: statusValue = currentStatus.afrTarget; break;
    case 22: statusValue = currentStatus.tpsDOT; break; //TPS DOT
    case 23: statusValue = currentStatus.advance; break;
    case 24: statusValue = currentStatus.TPS; break; // TPS (0% to 100%)
    
    case 25: 
      if(currentStatus.loopsPerSecond > 60000) { currentStatus.loopsPerSecond = 60000;}
      statusValue = lowByte(currentStatus.loopsPerSecond); 
      break;
    case 26: 
      if(currentStatus.loopsPerSecond > 60000) { currentStatus.loopsPerSecond = 60000;}
      statusValue = highByte(currentStatus.loopsPerSecond); 
      break;
    
    case 27: 
      currentStatus.freeRAM = 50000;
      statusValue = lowByte(currentStatus.freeRAM); //(byte)((currentStatus.loopsPerSecond >> 8) & 0xFF);
      break; 
    case 28: 
      currentStatus.freeRAM = 50000;
      statusValue = highByte(currentStatus.freeRAM); 
      break;

    case 29: statusValue = (byte)(currentStatus.boostTarget >> 1); break; //Divide boost target by 2 to fit in a byte
    case 30: statusValue = (byte)(currentStatus.boostDuty / 100); break;
    case 31: statusValue = currentStatus.spark; break; //Spark related bitfield

    //rpmDOT must be sent as a signed integer
    case 32: statusValue = lowByte(currentStatus.rpmDOT); break;
    case 33: statusValue = highByte(currentStatus.rpmDOT); break;

    case 34: statusValue = currentStatus.ethanolPct; break; //Flex sensor value (or 0 if not used)
    case 35: statusValue = currentStatus.flexCorrection; break; //Flex fuel correction (% above or below 100)
    case 36: statusValue = currentStatus.flexIgnCorrection; break; //Ignition correction (Increased degrees of advance) for flex fuel

    case 37: statusValue = currentStatus.idleLoad; break;
    case 38: statusValue = currentStatus.testOutputs; break;

    case 39: statusValue = currentStatus.O2_2; break; //O2
    case 40: statusValue = currentStatus.baro; break; //Barometer value

    case 41: statusValue = lowByte(currentStatus.canin[0]); break;
    case 42: statusValue = highByte(currentStatus.canin[0]); break;
    case 43: statusValue = lowByte(currentStatus.canin[1]); break;
    case 44: statusValue = highByte(currentStatus.canin[1]); break;
    case 45: statusValue = lowByte(currentStatus.canin[2]); break;
    case 46: statusValue = highByte(currentStatus.canin[2]); break;
    case 47: statusValue = lowByte(currentStatus.canin[3]); break;
    case 48: statusValue = highByte(currentStatus.canin[3]); break;
    case 49: statusValue = lowByte(currentStatus.canin[4]); break;
    case 50: statusValue = highByte(currentStatus.canin[4]); break;
    case 51: statusValue = lowByte(currentStatus.canin[5]); break;
    case 52: statusValue = highByte(currentStatus.canin[5]); break;
    case 53: statusValue = lowByte(currentStatus.canin[6]); break;
    case 54: statusValue = highByte(currentStatus.canin[6]); break;
    case 55: statusValue = lowByte(currentStatus.canin[7]); break;
    case 56: statusValue = highByte(currentStatus.canin[7]); break;
    case 57: statusValue = lowByte(currentStatus.canin[8]); break;
    case 58: statusValue = highByte(currentStatus.canin[8]); break;
    case 59: statusValue = lowByte(currentStatus.canin[9]); break;
    case 60: statusValue = highByte(currentStatus.canin[9]); break;
    case 61: statusValue = lowByte(currentStatus.canin[10]); break;
    case 62: statusValue = highByte(currentStatus.canin[10]); break;
    case 63: statusValue = lowByte(currentStatus.canin[11]); break;
    case 64: statusValue = highByte(currentStatus.canin[11]); break;
    case 65: statusValue = lowByte(currentStatus.canin[12]); break;
    case 66: statusValue = highByte(currentStatus.canin[12]); break;
    case 67: statusValue = lowByte(currentStatus.canin[13]); break;
    case 68: statusValue = highByte(currentStatus.canin[13]); break;
    case 69: statusValue = lowByte(currentStatus.canin[14]); break;
    case 70: statusValue = highByte(currentStatus.canin[14]); break;
    case 71: statusValue = lowByte(currentStatus.canin[15]); break;
    case 72: statusValue = highByte(currentStatus.canin[15]); break;

    case 73: statusValue = currentStatus.tpsADC; break;
    case 74: statusValue = 0;

    case 75: statusValue = lowByte(currentStatus.PW1); break; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
    case 76: statusValue = highByte(currentStatus.PW1); break; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
    case 77: statusValue = lowByte(currentStatus.PW2); break; //Pulsewidth 2 multiplied by 10 in ms. Have to convert from uS to mS.
    case 78: statusValue = highByte(currentStatus.PW2); break; //Pulsewidth 2 multiplied by 10 in ms. Have to convert from uS to mS.
    case 79: statusValue = lowByte(currentStatus.PW3); break; //Pulsewidth 3 multiplied by 10 in ms. Have to convert from uS to mS.
    case 80: statusValue = highByte(currentStatus.PW3); break; //Pulsewidth 3 multiplied by 10 in ms. Have to convert from uS to mS.
    case 81: statusValue = lowByte(currentStatus.PW4); break; //Pulsewidth 4 multiplied by 10 in ms. Have to convert from uS to mS.
    case 82: statusValue = highByte(currentStatus.PW4); break; //Pulsewidth 4 multiplied by 10 in ms. Have to convert from uS to mS.

    case 83: statusValue = currentStatus.status3; break;
    case 84: statusValue = currentStatus.engineProtectStatus; break;
    case 85: statusValue = lowByte(currentStatus.fuelLoad); break;
    case 86: statusValue = highByte(currentStatus.fuelLoad); break;
    case 87: statusValue = lowByte(currentStatus.ignLoad); break;
    case 88: statusValue = highByte(currentStatus.ignLoad); break;
    case 89: statusValue = lowByte(currentStatus.dwell); break;
    case 90: statusValue = highByte(currentStatus.dwell); break;
    case 91: statusValue = currentStatus.CLIdleTarget; break;
    case 92: statusValue = currentStatus.mapDOT; break;
    case 93: statusValue = (int8_t)currentStatus.vvt1Angle; break;
    case 94: statusValue = currentStatus.vvt1TargetAngle; break;
    case 95: statusValue = currentStatus.vvt1Duty; break;
    case 96: statusValue = lowByte(currentStatus.flexBoostCorrection); break;
    case 97: statusValue = highByte(currentStatus.flexBoostCorrection); break;
    case 98: statusValue = currentStatus.baroCorrection; break;
    case 99: statusValue = currentStatus.VE; break; //Current VE (%). Can be equal to VE1 or VE2 or a calculated value from both of them
    case 100: statusValue = currentStatus.ASEValue; break; //Current ASE (%)
    case 101: statusValue = lowByte(currentStatus.vss); break;
    case 102: statusValue = highByte(currentStatus.vss); break;
    case 103: statusValue = currentStatus.gear; break;
    case 104: statusValue = currentStatus.fuelPressure; break;
    case 105: statusValue = currentStatus.oilPressure; break;
    case 106: statusValue = currentStatus.wmiPW; break;
    case 107: statusValue = currentStatus.wmiEmpty; break;
    case 108: statusValue = (int8_t)currentStatus.vvt2Angle; break;
    case 109: statusValue = currentStatus.vvt2TargetAngle; break;
    case 110: statusValue = currentStatus.vvt2Duty; break;
    case 111: statusValue = currentStatus.outputsStatus; break;
    case 112: statusValue = (byte)(currentStatus.fuelTemp); break; //Fuel temperature from flex sensor
    case 113: statusValue = currentStatus.fuelTempCorrection; break; //Fuel temperature Correction (%)
    case 114: statusValue = currentStatus.advance1; break; //advance 1 (%)
    case 115: statusValue = currentStatus.advance2; break; //advance 2 (%)
    case 116: statusValue = currentStatus.TS_SD_Status; break; //SD card status
  }

  return statusValue;

  //Each new inclusion here need to be added on speeduino.ini@L78, only list first byte of an integer and second byte as "INVALID"
  //Every 2-byte integer added here should have it's lowByte index added to fsIntIndex array on globals.ino@L116
}