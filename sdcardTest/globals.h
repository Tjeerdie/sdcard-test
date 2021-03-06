#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>

#ifdef STM32F407xx
  #define SD_LOGGING //SD logging enabled for STM32F407 because it has the SDIO interface
  #define SD_LIB_H "src/STM32SD/STM32SD.h"
#endif

#define LOGGER_CSV_SEPARATOR_SEMICOLON  0
#define LOGGER_CSV_SEPARATOR_COMMA      1
#define LOGGER_CSV_SEPARATOR_TAP        2
#define LOGGER_CSV_SEPARATOR_SPACE      3

#define LOGGER_DISABLED                 0
#define LOGGER_CSV                      1
#define LOGGER_BINARY                   2

#define LOGGER_RATE_1HZ                 0
#define LOGGER_RATE_4HZ                 1
#define LOGGER_RATE_10HZ                2
#define LOGGER_RATE_30HZ                3

#define LOGGER_FILENAMING_OVERWRITE     0
#define LOGGER_FILENAMING_DATETIME      1
#define LOGGER_FILENAMING_SEQENTIAL     2


//The status struct contains the current values for all 'live' variables
//In current version this is 64 bytes
struct statuses {
  volatile bool hasSync;
  uint16_t RPM;
  byte RPMdiv100;
  long longRPM;
  int mapADC;
  int baroADC;
  long MAP; //Has to be a long for PID calcs (Boost control)
  int16_t EMAP;
  int16_t EMAPADC;
  byte baro; //Barometric pressure is simply the inital MAP reading, taken before the engine is running. Alternatively, can be taken from an external sensor
  byte TPS; /**< The current TPS reading (0% - 100%). Is the tpsADC value after the calibration is applied */
  byte tpsADC; /**< 0-255 byte representation of the TPS. Downsampled from the original 10-bit reading, but before any calibration is applied */
  byte tpsDOT; /**< TPS delta over time. Measures the % per second that the TPS is changing. Value is divided by 10 to be stored in a byte */
  byte mapDOT; /**< MAP delta over time. Measures the kpa per second that the MAP is changing. Value is divided by 10 to be stored in a byte */
  volatile int rpmDOT;
  byte VE; /**< The current VE value being used in the fuel calculation. Can be the same as VE1 or VE2, or a calculated value of both */
  byte VE1; /**< The VE value from fuel table 1 */
  byte VE2; /**< The VE value from fuel table 2, if in use (and required conditions are met) */
  byte O2;
  byte O2_2;
  int coolant;
  int cltADC;
  int IAT;
  int iatADC;
  int batADC;
  int O2ADC;
  int O2_2ADC;
  int dwell;
  byte dwellCorrection; /**< The amount of correction being applied to the dwell time. */
  byte battery10; /**< The current BRV in volts (multiplied by 10. Eg 12.5V = 125) */
  int8_t advance; /**< The current advance value being used in the spark calculation. Can be the same as advance1 or advance2, or a calculated value of both */
  int8_t advance1; /**< The advance value from ignition table 1 */
  int8_t advance2; /**< The advance value from ignition table 2 */
  uint16_t corrections; /**< The total current corrections % amount */
  uint16_t AEamount; /**< The amount of accleration enrichment currently being applied. 100=No change. Varies above 255 */
  byte egoCorrection; /**< The amount of closed loop AFR enrichment currently being applied */
  byte wueCorrection; /**< The amount of warmup enrichment currently being applied */
  byte batCorrection; /**< The amount of battery voltage enrichment currently being applied */
  byte iatCorrection; /**< The amount of inlet air temperature adjustment currently being applied */
  byte baroCorrection; /**< The amount of correction being applied for the current baro reading */
  byte launchCorrection; /**< The amount of correction being applied if launch control is active */
  byte flexCorrection; /**< Amount of correction being applied to compensate for ethanol content */
  byte fuelTempCorrection; /**< Amount of correction being applied to compensate for fuel temperature */
  int8_t flexIgnCorrection; /**< Amount of additional advance being applied based on flex. Note the type as this allows for negative values */
  byte afrTarget;
  byte idleDuty; /**< The current idle duty cycle amount if PWM idle is selected and active */
  byte CLIdleTarget; /**< The target idle RPM (when closed loop idle control is active) */
  bool idleUpActive; /**< Whether the externally controlled idle up is currently active */
  bool CTPSActive; /**< Whether the externally controlled closed throttle position sensor is currently active */
  bool fanOn; /**< Whether or not the fan is turned on */
  volatile byte ethanolPct; /**< Ethanol reading (if enabled). 0 = No ethanol, 100 = pure ethanol. Eg E85 = 85. */
  volatile int8_t fuelTemp;
  unsigned long AEEndTime; /**< The target end time used whenever AE is turned on */
  volatile byte status1;
  volatile byte spark;
  volatile byte spark2;
  uint8_t engine;
  unsigned int PW1; //In uS
  unsigned int PW2; //In uS
  unsigned int PW3; //In uS
  unsigned int PW4; //In uS
  unsigned int PW5; //In uS
  unsigned int PW6; //In uS
  unsigned int PW7; //In uS
  unsigned int PW8; //In uS
  volatile byte runSecs; /**< Counter of seconds since cranking commenced (Maxes out at 255 to prevent overflow) */
  volatile byte secl; /**< Counter incrementing once per second. Will overflow after 255 and begin again. This is used by TunerStudio to maintain comms sync */
  volatile uint32_t loopsPerSecond; /**< A performance indicator showing the number of main loops that are being executed each second */ 
  bool launchingSoft; /**< Indicator showing whether soft launch control adjustments are active */
  bool launchingHard; /**< Indicator showing whether hard launch control adjustments are active */
  uint16_t freeRAM;
  unsigned int clutchEngagedRPM; /**< The RPM at which the clutch was last depressed. Used for distinguishing between launch control and flat shift */ 
  bool flatShiftingHard;
  volatile uint32_t startRevolutions; /**< A counter for how many revolutions have been completed since sync was achieved. */
  uint16_t boostTarget;
  byte testOutputs;
  bool testActive;
  uint16_t boostDuty; //Percentage value * 100 to give 2 points of precision
  byte idleLoad; /**< Either the current steps or current duty cycle for the idle control. */
  uint16_t canin[16];   //16bit raw value of selected canin data for channel 0-15
  uint8_t current_caninchannel = 0; /**< Current CAN channel, defaults to 0 */
  uint16_t crankRPM = 400; /**< The actual cranking RPM limit. This is derived from the value in the config page, but saves us multiplying it everytime it's used (Config page value is stored divided by 10) */
  volatile byte status3;
  int16_t flexBoostCorrection; /**< Amount of boost added based on flex */
  byte nitrous_status;
  byte nSquirts;
  byte nChannels; /**< Number of fuel and ignition channels.  */
  int16_t fuelLoad;
  int16_t fuelLoad2;
  int16_t ignLoad;
  bool fuelPumpOn; /**< Indicator showing the current status of the fuel pump */
  byte syncLossCounter;
  byte knockRetard;
  bool knockActive;
  bool toothLogEnabled;
  bool compositeLogEnabled;
  //int8_t vvt1Angle;
  long vvt1Angle;
  byte vvt1TargetAngle;
  byte vvt1Duty;
  uint16_t injAngle;
  byte ASEValue;
  uint16_t vss; /**< Current speed reading. Natively stored in kph and converted to mph in TS if required */
  bool idleUpOutputActive; /**< Whether the idle up output is currently active */
  byte gear; /**< Current gear (Calculated from vss) */
  byte fuelPressure; /**< Fuel pressure in PSI */
  byte oilPressure; /**< Oil pressure in PSI */
  byte engineProtectStatus;
  byte wmiPW;
  bool wmiEmpty;
  long vvt2Angle;
  byte vvt2TargetAngle;
  byte vvt2Duty;
  byte outputsStatus;
  byte TS_SD_Status; //TunerStudios SD card status
};

/*
Page 13 - Programmable outputs conditions.
128 bytes long
*/
struct config13 {
  uint8_t outputInverted;
  uint8_t unused12_1;
  uint8_t outputPin[8];
  uint8_t outputDelay[8]; //0.1S
  uint8_t firstDataIn[8];
  uint8_t secondDataIn[8];
  uint8_t unused_13[16];
  int16_t firstTarget[8];
  int16_t secondTarget[8];
  //89bytes
  byte operation[89];

  uint16_t candID[8]; //Actual CAN ID need 16bits, this is a placeholder

  byte unused12_106_116[10];

  byte onboard_log_csv_separator :2;  //";", ",", "tab", "space"  
  byte onboard_log_file_style    :2;  // "Disabled", "CSV", "Binary", "INVALID" 
  byte onboard_log_file_rate     :2;  // "1Hz", "4Hz", "10Hz", "30Hz" 
  byte onboard_log_filenaming    :2;  // "Overwrite", "Date-time", "Sequential", "INVALID" 
  byte onboard_log_storage       :2;  // "sd-card", "INVALID", "INVALID", "INVALID" ;In the future maybe an onboard spi flash can be used, or switch between SDIO vs SPI sd card interfaces.
  byte onboard_log_trigger_boot  :1;  // "Disabled", "On boot"
  byte onboard_log_trigger_RPM   :1;  // "Disabled", "Enabled"
  byte onboard_log_trigger_prot  :1;  // "Disabled", "Enabled"
  byte onboard_log_trigger_Vbat  :1;  // "Disabled", "Enabled"
  byte onboard_log_trigger_Epin  :2;  // "Disabled", "polling", "toggle" , "INVALID" 
  byte onboard_log_tr2_thr_on;        //  "RPM",      100.0,  0.0,    0,     10000,  0
  byte onboard_log_tr2_thr_off;       //  "RPM",      100.0,  0.0,    0,     10000,  0
  byte onboard_log_tr3_thr_RPM   :1;  // "Disabled", "Enabled"
  byte onboard_log_tr3_thr_MAP   :1;  // "Disabled", "Enabled"
  byte onboard_log_tr3_thr_Oil   :1;  // "Disabled", "Enabled"
  byte onboard_log_tr3_thr_AFR   :1;  // "Disabled", "Enabled"     
  byte onboard_log_tr4_thr_on;        // "V",        0.1,   0.0,  0.0,  15.90,      2 ; * (  1 byte)    
  byte onboard_log_tr4_thr_off;       // "V",        0.1,   0.0,  0.0,  15.90,      2 ; * (  1 byte)   
  byte onboard_log_tr5_thr_on;        // "pin",      0,    0, 0,  1,    255,        0 ;  
  

  byte unused12_125_127[4];

#if defined(CORE_AVR)
  };
#else
  } __attribute__((__packed__)); //The 32 bit systems require all structs to be fully packed
#endif


extern struct statuses currentStatus; //The global status object
extern struct config13 configPage13;

/*
***********************************************************************************************************
* RTC init
*/

#include <STM32RTC.h>
STM32RTC& rtc = STM32RTC::getInstance();
extern "C" {uint32_t get_fattime (void);}



#endif