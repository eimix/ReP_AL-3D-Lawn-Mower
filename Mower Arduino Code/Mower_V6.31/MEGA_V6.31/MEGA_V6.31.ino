
// ReP_AL Lawn Mower Arduino Code
// Please make sure you have installed all the library files to the Arduino libraries folder
// You will need to unzip all the libraries from the GitHuB site.
// Instructions onnhow to do this are available on my webpage
// www.repalmkaershop.com

//TODO
// 1. Variables with no config should be #define'ed, ex.: Battery_Max
// 2. Bumper sensors to use with or without sonar sensors
// 3. Manouvers - state machine to perform list of commands (now complex debuging, many dalay() that blocks loop execution
// {
//   {Command: 'Go forward', Condition: 'Time', Paramter1: 5}, 
//   {Command: 'Wait', Condition: 'Time', Paramter1: 10}, 
//   {Command: 'Turn', Condition: 'Angle', Paramter1: 90}, 
//   {Command: 'Turn', Condition: 'Angle', Paramter1: -180}, 
//   {Command: 'Go forward', Condition: 'Wire', Paramter1: 1},    // found
//   {Command: 'Go forward', Condition: 'Sonar', Paramter1: 20},  // <20 cm
//   {Command: 'Go forward', Condition: 'Bumper', Paramter1: 3},  // 1 left bumper hit, 2 right bumper hit, 3 any bumper hit
// }
// 4. Beeper for user notifications: need recharge, error, warning etc.

//DONE
// 1. Variable P is too short to be global, rename it - changed to P -> PID_P; D -> PID_D; I -> Perimeter_I (PID_I)
// 2. Lift sensor (kill switch), warning - it works in normal conditions, but not in "delay(>1000)"
// 3. Serial output is flooded every loop - debug levels could be used (1-error; 2-warning; 3-info; 4-debug; 5-full)
// 4. Turn_Adjust now is proportional (it could not become negative output for motor) 
// 5. BUG: Calculate_Compass_Wheel_Compensation code made worse when targer 359° and current direction is 2° (no compensation needed, but it overcompensates)
// 6. BUG: compass direction normalization [0°; 360°] and use of Lower Upper limits is not consistent
// 7. Loop_Cycle_Mowing is used in "Turn around" once left, once right, but this variable Loop_Cycle_Mowing is increasing with mowing length, not turn around count
// 8. RTC clock types: DS1302, DS1307(I2C)
// 9. Update to new ADCMAN version (current H is not for use with CPP - no 10bits ADC available, only 8bits VDC/2)
//   1.09rls version does not have differential signal, test version does not have 10bits reading - modified adcman.cpp to read 10bits values
//10. User configurable variables and defines should be in Config.h
//11. Custom delay function with sensor reading and motor stop
//12. Very many dalay(>1000) calls, and no safty checks could be done during delays - delays remained, changed delay it self
//13. Joystick type control using WiFi - more responsive driving

//Libraries for Perimeter Wire Receiver
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include "drivers.h"
#include "adcman.h"
#include "perimeter.h"

#include "Config.h"

//Libraries for the Mowing Calendar Function
#include <TimeLib.h>
#include <TimeAlarms.h>
AlarmId id;

#include "Globals.h"

//Global Variables

//Perimeter Variables
Perimeter perimeter;
unsigned long nextTime = 0;             // Next time wire sensor reading is required, incremented by 50ms
int WireCrossCounter = 0;               // ??? Probably not used
boolean inside = true;
int Wire_Detected;

byte Loop_Cycle_Mowing    = 0;
byte Turn_Around_Counter  = 0;          // No worry about max 255, it is used for turn around: once left, once right

//Sonar Variables
long duration1 = 0;                     // Time required for the sonar ping to be recieved by the echo pin.
long duration2 = 0;
long duration3 = 0;

int distance1 = 999;                    // Distance caculated  by the Sonar
int distance2 = 999;
int distance3 = 999;

int distance_blockage;

int  Sonar_Hit_1_Total;
int  Sonar_Hit_2_Total;
int  Sonar_Hit_3_Total;
bool Sonar_Hit_Any_Total;
bool Sonar_Hit_1 = 0;
bool Sonar_Hit_2 = 0;
bool Sonar_Hit_3 = 0;
bool Sonar_Hit   = 0;


//Mower Status Variables
bool Mower_Docked;
bool Mower_Parked;
bool Mower_Running;
bool Mower_Parked_Low_Batt;
bool Mower_Charging;
bool Mower_Error;
int Mower_SaftyWarning;
bool Manual_Mode;

//Membrane Key Variables
byte  Start_Key_X;
byte  Plus_Key_X;
byte  Minus_Key_X;
byte  Stop_Key_X;
bool  Menu_Complete;
byte  Menu_Mode_Selection;
int   Menu_View;
int   Mow_Time_Set;

//Mow Calendar Variables
byte Alarm_Hour_Now;
bool Alarm_Timed_Mow_ON = 0;
byte Alarm_Timed_Mow_Hour;                         // Mowing Hour Number 3
byte Alarm_Timed_Mow_Minute;                       // Alarm minute 3

//Perimeter Wire Tracking
int   Perimeter_I;
int   Track_Wire_Itterations;
bool  Outside_Wire;
byte  Exit_Zone;
int   MAG_Now;
int   MAG_OUT_Stop;
int   MAG_IN_Stop;
int   MAG_TURN;
int   MAG_Average_Start;
int   MAG_Last;
byte  Outside_Wire_Count = 0;
int   Tracking_Wire = 0;
bool  Wire_ON_Printed;
int   Wire_Off;

int  Tracking_Turn_Left;
int  Tracking_Turn_Right;
bool Mower_Track_To_Charge;
bool Mower_Track_To_Exit;

bool Abort_Wire_Find;
bool No_Wire_Found;

int  PWM_Right;
int  PWM_Left;
int  MAG_Goal;
int  MAG_Error;
int  MAG_Start;
//byte PWM_Blade_Speed_Min;
//byte PWM_Blade_Speed_Max;
bool Blade_Override = 0;

//Compass Variables
float Compass_Heading;
int   Compass_Heading_Degrees;
bool  Compass_Heading_Locked = 0;
int   Compass_Heading_Lock_Degrees;
//int   Heading_Upper_Limit_Compass;
//int   Heading_Lower_Limit_Compass;
int   Compass_Target;
int   Compass_Cycle = 0; //Compass_Leg
//int   Turn_Adjust = 0;

unsigned long LastTime    = 0;
unsigned long CurrentTime = 0;
unsigned long LoopLength;
//---------------------------------------------------------------------------------------

void setup()
{
#if (DEBUG_LEVEL >= 1)
  Serial.begin(115200);
#endif

#ifdef UseRemoteSensors
  Serial1.begin(1200);							                      		// Open Serial port 1 for the nano communication
#endif

#ifdef UseWiFi
  Serial2.begin(9600); //9600                           		// If WIFI is on open Serial port 2 for the NodeMCU communication
#endif

#if (DEBUG_LEVEL >= 3)
  Serial.println(" ");
  Serial.println(" ");
  Serial.print(F("ReP_AL Robot :"));
  Serial.println(Version);
  Serial.println(F("==================="));
  Serial.println("");
  Serial.println(F("Starting Mower Setup"));
  Serial.println(F("==================="));
#endif

  Setup_RTC();

  Load_EEPROM_Saved_Data();

#if (SetStartupTime == 1)
#if (DEBUG_LEVEL >= 2)
  Serial.println(F("Setting Time"));
#endif
  Set_Time_On_RTC();
#endif  
  DisplayTime();

#if (DEBUG_LEVEL >= 3)
  Serial.println("");
#endif

  Prepare_Mower_from_Settings();
  Setup_Run_LCD_Intro ();
  Setup_Sonar();
  Setup_Relays();
  Setup_Membrane_Buttons();
  Setup_Motor_Pins();
  Setup_Compass();
  Setup_ADCMan();

  Setup_Sensors();
}
//---------------------------------------------------------------------------------------

void loop()
{
  CurrentTime = millis(); 
  LoopLength  = calcTimeDiff(LastTime, CurrentTime); 
  LastTime    = CurrentTime; 
  
  ADCMan.run();

#ifdef UseRemoteSensors
  Read_Serial1_Nano();						                                                	  		// Read the Serial data from the nano
#endif
#ifdef UseLocalSensors
  Loop_Sensors();                                                                         // Read sensors data
#endif

  Print_Mower_Status();                                                                 	// Update the Serial monitor with the current mower status.

  // Mower is docked waiting for a command to leave and mow.
  if (Mower_Docked == 1)          Print_LCD_Volt_Info();                                  // Print the voltage to the LCD screen
  if (Mower_Docked == 1)          Process_Volt_Information();                             // Take action based on the voltage readings
  if (Mower_Docked == 1)          Check_if_Charging();
  //if (Mower_Docked == 1)          Check_if_Raining_From_Nano();
  if (Mower_Docked == 1)          Print_LCD_Info_Docked();                                // Print information to the LCD screen
  if (Mower_Docked == 1)          Print_LCD_Time();
  if (Mower_Docked == 1)          Check_Membrane_Switch_Input_Docked();                   // Check the membrane buttons for any input
  if (Mower_Docked == 1)          TestforBoundaryWire();                                  // Test is the boundary wire is live
  if (Mower_Docked == 1)          Manouver_Dock_The_Mower();
  if (Mower_Docked == 1)          Print_Serial_Time();
  if (Mower_Docked == 1)          Display_Next_Alarm();
  if (Mower_Docked == 1)          Activate_Alarms();

  // Mower is Parked ready to be started / re-started.
  if (Mower_Parked == 1)          Print_LCD_Volt_Info();                                  // Print the voltage to the LCD screen
  if (Mower_Parked == 1)          Process_Volt_Information();                             // Take action based on the voltage readings
  if (Mower_Parked == 1)          Check_if_Charging();
  if (Mower_Parked == 1)          Check_if_Raining_From_Nano();                           // Checks if the water sensor detects Rain
  if (Mower_Parked == 1)          Print_LCD_Info_Parked();                                // Print information to the LCD screen
  if (Mower_Parked == 1)          Print_LCD_Time();
  if (Mower_Parked == 1)          Check_Membrane_Switch_Input_Parked();                   // Check the membrane buttons for any input
  if (Mower_Parked == 1)          TestforBoundaryWire();
  if (Mower_Parked == 1)          Manouver_Park_The_Mower();
  if (Mower_Parked == 1)          Print_Serial_Time();
//  if (Mower_Parked == 1)          Display_Next_Alarm();
//  if (Mower_Parked == 1)          Activate_Alarms();

  // Mower is Parked with Low Battery needing manual charging
  if (Mower_Parked_Low_Batt == 1) Print_LCD_Time();
  if (Mower_Parked_Low_Batt == 1) Print_LCD_Volt_Info();                                  // Print the battery voltage
  if (Mower_Parked_Low_Batt == 1) Print_LCD_Recharge();                                   // Print re-charge on the LCD screen
  if (Mower_Parked_Low_Batt == 1) Check_Membrane_Switch_Input_Parked();
  if (Mower_Parked_Low_Batt == 1) Print_Serial_Time();

  // Lost mower is put into standby mode

  if (Mower_SaftyWarning > 0)  Print_LCD_Mower_Warning();                                 // Safety(pause) mode in case the mower in an warning state
  if (Mower_Error == 1)        Print_LCD_Mower_Error();                                   // Safety mode incase the mower is lost or in an error state
  if (Mower_Error == 1)        Check_Membrane_Switch_Input_Parked();

  // Mower is running cutting the grass.
  if (Mower_Running == 1)                                                                             Print_LCD_Volt_Info();              // Print the voltage to the LCD screen
  if (Mower_Running == 1)                                                                             Process_Volt_Information();         // Take action based on the voltage readings
  if (Mower_Running == 1)                                                                             Check_if_Raining_From_Nano();       // Test the rain sensor for rain. If detected sends the mower home
  if (Mower_Running == 1)                                                                             Check_Membrane_Keys_Running();      // Check to see if the mower needs to be stopped via keypad
  if (Mower_Running == 1)                                                                             Check_Timed_Mow();                  // Check to see if the time to go home has come.
  if (Mower_Running == 1)                                                                             TestforBoundaryWire();              // Test is the boundary wire is live
  if ((Mower_Running == 1) && (Wire_Detected == 1))                                                   Check_Wire_In_Out();                // Test if the mower is in or out of the wire fence.
  if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0))                            Check_Sonar_Sensors();              // If the mower is  the boundary wire check the sonars for obsticles and prints to the LCD
  if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0) && (Sonar_Hit == 0))        Manouver_Mow_The_Grass();           // Inputs to the wheel motors / blade motors according to surroundings
  if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 1) && (Loop_Cycle_Mowing > 0)) Manouver_Turn_Around();             // If outside the wire and mowing turn around
  if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0) && (Sonar_Hit == 1))        Manouver_Turn_Around_Sonar();       // If sonar hit is detected and mower is  the wire, manouver around obsticle

  // WIFI Commands from and to APP
#ifdef UseWiFi
  if (Manual_Mode == 1) Receive_WIFI_Manual_Commands();
#endif

  if (Manual_Mode == 1) Print_LCD_Time();
  if (Manual_Mode == 1) Print_LCD_Volt_Info();                                  // Print the battery voltage
  if (Manual_Mode == 1) Print_LCD_Info_Manual();
  
#ifdef UseWiFi
  if (Manual_Mode == 0) Get_WIFI_Commands();                                   // TX and RX data from NodeMCU
#endif

#if (DEBUG_LEVEL >= 2)
  Serial.println();
#endif
}
//---------------------------------------------------------------------------------------
