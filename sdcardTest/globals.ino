#include "globals.h"
struct statuses currentStatus; /**< The master global status struct. Contains all values that are updated frequently and used across modules */

//This function is called by the fatfs library to get the current time if a new file is created
uint32_t get_fattime (void){
  uint32_t time;
  time = (((uint32_t)rtc.getYear() + 20)<< 25) | (((uint32_t)rtc.getMonth())<< 21) | (((uint32_t)rtc.getDay())<< 16) | (((uint32_t)rtc.getHours())<< 11) | (((uint32_t)rtc.getMinutes())<< 5) | (((uint32_t)rtc.getSeconds())>>1);
  return time;
}