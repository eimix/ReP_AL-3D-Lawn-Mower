#ifndef CONFIG_H
#define CONFIG_H

/***********************************************************************************************/
/*
                   SETUP OF MOWER

  The following setup parameters will setup the mower for your garden
  Turn on or off the settings to defien how you like the mower to behave.

  1 = Turned ON      0 = Turned OFF       Value = Value set for variable.
*/
/*************************************************************************************************/

//NOTE: "#define UseSomething 1" is better then "bool UseSomething = 1;" 
//      linker skips not needed code if #define value is used in comparisons

// DEBUG level limits Serial output
// Use =0 - to disable Serial output, and it reduces sketch size most
// Use >0 - to allow some output, highe value - more output
#define DEBUG_LEVEL 4           // 0 - None; 1 - Error; 2 - Warning; 3 - Info; 4 - ALL
#if (DEBUG_LEVEL == 0)
#define Serial 0
#endif

#define Show_TX_Data 0          // Show the values recieved from the Nano / ModeMCU in the serial monitor

//-----------------------------------------------------------------------------------------------------------------------------------
// MAIN config
//-----------------------------------------------------------------------------------------------------------------------------------

char Version[8] = "V6.4";

#define Cutting_Blades_Activate 1                          // Activates the cutting blades and disc in the code
#define Perimeter_Wire_Enabled  1                          // Activates use of the perimeter boundary wire

//-----------------------------------------------------------------------------------------------------------------------------------
// Motors: WHEELS
//-----------------------------------------------------------------------------------------------------------------------------------

#define WheelMotorDriver_L298N

#ifdef WheelMotorDriver_L298N
//Pin Setup for the wheel Motor Bridge Controller
//Motor A
#define ENAPin 7                // EN Pins need a digital pin with PWM
#define IN1Pin 6                // IN Pins dont need digital PWM 
#define IN2Pin 5
//Motor B
#define ENBPin 2                // EN Pins need a digital pin with PWM
#define IN3Pin 4                // IN Pins dont need digital PWM
#define IN4Pin 3
#endif

//Wheel Motors Setup
#define Max_Cycles               150                         // Number of loops the Sketch will run before the mower just turns around anyway.
int PWM_MaxSpeed_LH            = 255;     //EEPROM           // Straight line speed LH Wheel (Looking from back of mower)  Will be overidden if saved in EEPROM
int PWM_MaxSpeed_RH            = 255;     //EEPROM           // Straight line speed RH Wheel - adjust to keep mower tracking straight.  Will be overridden if saved in EEPROM

//int Max_Motor_PWM_LH           = 255;
//int Max_Motor_PWM_RH           = 255;
#define Mower_Turn_Delay_Min     1000                        // Min Max Turn time of the Mower after it reverses at the wire.
#define Mower_Turn_Delay_Max     2500                        // A random turn time between these numbers is selected by the software
#define Mower_Reverse_Delay      1800                        // Time the mower reverses at the wire

//DEBUG purpose
#define DisableWheels 0
#define DisableBlades 0

//-----------------------------------------------------------------------------------------------------------------------------------
// Motors: BLADES; MOW Motor
//-----------------------------------------------------------------------------------------------------------------------------------

#define MowMotorDriver_RELAY   // 30A 30VDC
//#define MowMotorDriver_BTS7960 // 43A

//Motor Blades
#ifdef MowMotorDriver_BTS7960
#define RPWM 8
#define L_EN 9
#define R_EN 10

//Blade Speed can be modified in the settings menu and will be written to EEPROM
//The number below will then be overidden
byte PWM_Blade_Speed            = 250;      //EEPROM          // PWM signal sent to the blade motor (speed of blade) new motor works well at 245.
#endif

#ifdef MowMotorDriver_RELAY
#define MowMotorRelayPin 8
#endif

//Relay Switch
#define Relay_Motors 24         // be careful that you really use PIN24.  The order is sometimes labelled
                                // so it looks like 24 is actually 22.
#define Relay_Motors_Off LOW
#define Relay_Motors_On HIGH


//-----------------------------------------------------------------------------------------------------------------------------------
// Membrane Switch
//-----------------------------------------------------------------------------------------------------------------------------------

#define StartKeyPin 50 //connect wire 1 to pin 2
#define PlusKeyPin  51 //connect wire 2 to pin 3
#define MinusKeyPin 52 //connect wire 3 to pin 4
#define StopKeyPin  53 //connect wire 4 to pin 5

#define ButtonPressDelay 200

//-----------------------------------------------------------------------------------------------------------------------------------
// Sonar
//-----------------------------------------------------------------------------------------------------------------------------------

//Sonar Setup for Front Sonars 1-3
#define echoPin1 34   //S1               
#define trigPin1 35
#define echoPin2 36   //S2 
#define trigPin2 37
#define echoPin3 38   //S3
#define trigPin3 39

#define EchoTimeout   24000L        //5882uS = ~1m distance

//Sonar Modules
#define Sonar_1_Activate   1        // Activate (1) Deactivate (0) Sonar 1
#define Sonar_2_Activate   1        // Activate (1) Deactivate (0) Sonar 2
#define Sonar_3_Activate   1        // Activate (1) Deactivate (0) Sonar 3
#define Max_Sonar_Hit      3        // Maximum number of Sonar hits before object is discovered
#define MaxDistanceSonar 30L        // distance in cm from the mower that the sonar will activate at.

//-----------------------------------------------------------------------------------------------------------------------------------
// Sensors
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseLocalSensors
//#define UseRemoteSensors //This is no needen any more, remove this option and code after confirmation

#ifdef UseLocalSensors
#define pinBatteryVoltage A0        // pin normally A0
#define pinChargeCurrent  A1        // pin normally A1
#define pinLoadCurrent    A2        // pin normally A2

#define MaxLoadCurrent    6         // 6A max allowed load current, warning if exceeded

#define pinRainSensor     A3        // pin normally A3
#endif

//Voltage and Current Sensor Settings
#define VoltageDeviderR1   6800L
#define VoltageDeviderR2  62000L

#define CurrentmVperA       100L                       //100mV/A
#define CurrentOffset      2500L                       //sensor -20A~20A 

//Battery Settings
#define Battery_Max        29.4                       // Max battery volts in Volts. 7S 4.2V/cell
float Battery_Min       =  23.1;    //EEPROM          // Lower Limit of battery charge before re-charge required. 3.3V/cell
#define MaxLowBatteryCount   14                       // Instances of low battery detected before a re-charge is called..

//-----------------------------------------------------------------------------------------------------------------------------------
// Bumper sensors
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseBumperSensors

#ifdef UseBumperSensors
#define BumperFrontLeftPin  42
#define BumperFrontRightPin 43
#endif

//-----------------------------------------------------------------------------------------------------------------------------------
// Lift sensors
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseLiftSensors

#ifdef UseLiftSensors
#define LiftFrontLeftPin    40
#define LiftFrontRightPin   41
#endif

//-----------------------------------------------------------------------------------------------------------------------------------
// Beeper
//-----------------------------------------------------------------------------------------------------------------------------------

//#define UseBeeper

#ifdef UseBeeper
#define BeeperPin    45
#endif

//-----------------------------------------------------------------------------------------------------------------------------------
// Rain sensor
//-----------------------------------------------------------------------------------------------------------------------------------

#define Rain_Sensor_Installed     1                      // 1 for Rain sensor installed    0 for no sensor installed.
#define Rain_Total_Hits_Go_Home  10                      // This sensor only makes sense in combination with a mower docking station
                                                         // as the mower is sent there to get out of the rain.
                                                              
//-----------------------------------------------------------------------------------------------------------------------------------
// Docking
//-----------------------------------------------------------------------------------------------------------------------------------

//Docking Station
#define Use_Charging_Station      1                           // 1 if you are using the docking/charging station     0 if not
#define CW_Tracking_To_Charge     1                           // Clock-Wise         tracking around the boundary wire to the charging station
#define CCW_Tracking_To_Charge    0                           // Counter-Clock-Wise tracking around the boundary wire to the charging station
#define CW_Tracking_To_Start      0                           // Clock-Wise         tracking around the boundary wire when tracking to the start position
#define CCW_Tracking_To_Start     1                           // Counter-Clock-Wise tracking around the boundary wire to the charging station
//#define Docked_Filter_Hits        1;                          // Number of charge signals to be detected before mower powers off

#define Track_Wire_Zone_1_Cycles 1300                         // Zone 1 - Number of Itterations the PID function does before the mower exits the wire track
#define Track_Wire_Zone_2_Cycles 2200                         // Zone 2 - Therefore how long the mower is tracking the wire can be set = distance tracked.

#define Max_Tracking_Turn_Right   270                         // The maximum number of turn right commands during wire tracking before a renewed wire find function is called
#define Max_Tracking_Turn_Left    270                         // This helps to re-find the wire should the mower loose the wire for any reason.
#define Max_Cycle_Wire_Find_Fwd   320                         // Maximum number of Forward tracking cycles in finding wire before the mower restarts a compass turn and wire find.
#define Max_Cycle_Wire_Find_Back  50                          // Maximum number of Backward tracking cycles in finding wire before the mower restarts a compass turn and wire find.  

//-----------------------------------------------------------------------------------------------------------------------------------
// Perimeter
//-----------------------------------------------------------------------------------------------------------------------------------

//Perimeter Wire Pins
#define pinPerimeterLeft  A5       // perimeter pin normally A5
#define pinPerimeterRight A4       // leave open
#define pinLED LED_BUILTIN

/* Description of how the below values are displayed in the Serial Monitor Print Out for the wire
   function
   (InMax)                   Wire = 0                 (OutMax)
       |      (InMid)           |           (OutMid)     |
       |--------|--------|------|------|--------|--------|
       |        |        |      |      |        |        |
                      (InMin)       (OutMin)
*/

// Wire detection Values
/*Negative Values for In*/                                    // These values are based on the signal received by the wire sensor for my perimeter loop
#define InMin                 -200
#define InMid                 -700
#define InMax                 -1500                           // the maximum received signal value  the wire
/*General Setup PID numbers for wire tracking*/
float PID_P                 = 0.08;              //EEPROM     // Multiplication factor to the error measured to the wire center. If jerky movement when tracking reduce number
float PID_D                 = 20;                             // Dampening value to avoid the mower snaking on the wire.
//float PID_I ...
#define Scale                 36                              // Serial Monitor Line Tracking Print Scale

/*Positive Values for Out*/
#define OutMin                150
#define OutMid                400
#define OutMax                1500                            // the maximum received signal value outside the wire

#define Outside_Wire_Count_Max          15                    // If the mower is outside the wire this many times the mower is stopped
#define Action_On_Over_Wire_Count_Max   3                     // Set 1 to hibernate mower (Power Off and Stop)   
                                                              // Set 2 to refind garden using sonar and wire detect function
                                                              // Set 3 to do a refind wire function

//-----------------------------------------------------------------------------------------------------------------------------------
// Compass
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseCompassQMC5883 //I2C

#ifdef UseCompassQMC5883
// Set declination angle. Find your location declination on: http://magnetic-declination.com/
// (+) Positive or (-) for negative,
float Declination_Angle = (7.0 + (23.0 / 60.0)) / (180 / PI);   // Bad Krozingen is 2° 19'

//Compass Setup
#include <DFRobot_QMC5883.h>
DFRobot_QMC5883 compass;

//Compass Settings 
bool Compass_Activate               =   1;                       // Turns on the Compass (needs to be 1 to activate further compass features)
#define Compass_Heading_Hold_Enabled    1                        // Activates the compass heading hold function to keep the mower straight
int Home_Wire_Compass_Heading       = 110;                       // Heading the Mower will search for the wire once the mowing is completed.
#define CompassCorectionPower           2                        // Magnification of heading to PWM - How strong the mower corrects itself in Compass Mowing
byte Pattern_Mow                    =   0;                       // Activate pattern mowing instead of random mowing direction
#define Compass_Mow_Direction         110                        // Mow Direction of line when pattern mow is activated

#endif

//-----------------------------------------------------------------------------------------------------------------------------------
// WiFi
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseWiFi

//-----------------------------------------------------------------------------------------------------------------------------------
// LCD
//-----------------------------------------------------------------------------------------------------------------------------------

// For Aruino Uno (32k/2k) disable LCD, and use WiFi control only
// THIS IS PLACE HOLDER ONLY, NO FUNCTIONALITY YET
#define UseLCD16x2
//#define UseLCD20x4

#ifdef UseLCD16x2
#define LCDI2CAddress 0x20
//Libraries for ic2 Liquid Crystal
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(LCDI2CAddress, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#endif

//-----------------------------------------------------------------------------------------------------------------------------------
// Clock
//-----------------------------------------------------------------------------------------------------------------------------------

//#define UseClockDS1302
#define UseClockDS1307 //I2C

// Time Setup
#define SetStartupTime 0                          // Turn to 1 to set time on RTC (Set time in Time tab Set_Time_On_RTC)  After setting time turn to 0 and reload sketch.

//const char daysOfTheWeek[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
#define RTCSketchYear  2019
#define RTCSketchMonth 8
#define RTCSketchDay   22

#ifdef UseClockDS1302

#define kCePin   29  // RST
#define kIoPin   30  // DAT
#define kSclkPin 31  // CLK

//Libraries for Real Time Clock
#include <stdio.h>
#include <DS1302.h>

DS1302 rtc(kCePin, kIoPin, kSclkPin);
#endif

#ifdef UseClockDS1307
#include <RTClib.h>
RTC_DS1307 rtc;
#endif

//-----------------------------------------------------------------------------------------------------------------------------------
// Alarm settings
//-----------------------------------------------------------------------------------------------------------------------------------

// If the Alarm is changed in settings it will be written to EEPROM and the settings below will be overriden.
// Action for Alarm 1 is set to exit the dock and mow at this time.
// To change this action go to "void Activate_Alarms()"
bool Alarm_1_ON                 = 0;       //EEPROM            // Activate Alarm 1  (1 = ON 0 = OFF)
int Alarm_1_Hour                = 12;      //EEPROM            // Mowing Hour Number 1
int Alarm_1_Minute              = 0;       //EEPROM            // Alarm Minute 1
bool Alarm_1_Repeat             = 0;                           // Repeat the Alarm at the same time

// Action for Alarm 2 can be set in "void Activate_Alarms()"
bool Alarm_2_ON                 = 0;       //EEPROM            // Activate Alarm 2 (1 = ON 0 = OFF)
int Alarm_2_Hour               = 12;       //EEPROM            // Mowing Hour Number 2
int Alarm_2_Minute              = 0;       //EEPROM            // Alarm minute 2
bool Alarm_2_Repeat             = 0;                           // Repeat the Alarm at the same time

// Action for Alarm 3 can be set in "void Activate_Alarms()"
// Go Home Alarm
bool Alarm_3_ON                 = 0;       //EEPROM            // Activate Alarm 3 (1 = ON 0 = OFF)
int Alarm_3_Hour               = 12;       //EEPROM            // Mowing Hour Number 3
int Alarm_3_Minute              = 0;       //EEPROM            // Alarm minute 3
bool Alarm_3_Repeat             = 0;                           // Repeat the Alarm at the same time

//byte Alarm_Second               = 5;                           // Seconds

//-----------------------------------------------------------------------------------------------------------------------------------
// Error check
//-----------------------------------------------------------------------------------------------------------------------------------

#if defined(UseRemoteSensors) && defined(UseLocalSensors)
#error Only one type of sensors should be used
#endif

#if !defined(UseRemoteSensors) && !defined(UseLocalSensors)
#error At least one type of sensors should be used
#endif
//-----------------------------------------------------------------------------------------------------------------------------------

#if defined(MowMotorDriver_RELAY) && defined(MowMotorDriver_BTS7960)
#error Only one type of mow motor driver should be used
#endif

#if !defined(MowMotorDriver_RELAY) && !defined(MowMotorDriver_BTS7960)
#error At least one type of mow motor driver should be used
#endif
//-----------------------------------------------------------------------------------------------------------------------------------

#if defined(UseClockDS1302) && defined(UseClockDS1307)
#error Only one type of RTC should be used
#endif

#if !defined(UseClockDS1302) && !defined(UseClockDS1307)
#error At least one type of RTC should be used
#endif
//-----------------------------------------------------------------------------------------------------------------------------------

#endif
