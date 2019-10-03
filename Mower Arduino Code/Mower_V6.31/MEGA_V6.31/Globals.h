#ifndef GLOBALS_H
#define GLOBALS_H

//-----------------------------------------------------------------------------------------------------------------------------------

//Battery voltage if <Min - need charge; if >Max - stop charge
float BatteryVoltage; //Volts
//Charge current: if < 0.4A - charging finished
float ChargeCurrent;  //Amps
//Load current: if > 6A - motor locked, pause wait for user
float LoadCurrent;    //Amps

int LowBatteryCount = 0;

//-----------------------------------------------------------------------------------------------------------------------------------

#ifdef UseLiftSensors
byte LiftFrontLeft;
byte LiftFrontRight;
#define LiftSensorBAD LOW //PULLUP LOW is BAD, HIGH is GOOD
#endif

#ifdef UseBumperSensors
byte BumperFrontLeft;
byte BumperFrontRight;
#define BumperSensorHIT HIGH //PULLUP HIGH is HIT, LOW is open path
#endif

#define DirectionLeft  10
#define DirectionRight 11

//-----------------------------------------------------------------------------------------------------------------------------------

#define NUMBER_OF_ELEMENTS 11
#define MAX_SIZE 15 //14+1
//-----------------------------------------------------------------------------------------------------------------------------------

const char DockedMenuItems[NUMBER_OF_ELEMENTS][MAX_SIZE] PROGMEM = {
  {"Exit Dock Z-1"},
  {"Exit Dock Z-2"},
  {"Quick Start"},
  {"Trampoline Cut"},
  {"Mow the Line"},
  {"Test Menu   ->"},
  {"Setup Mower ->"},
  {"-- Spare  8 --"},
  {"-- Spare  9 --"},
  {"-- Spare 10 --"},
  {"-- Spare 11 --"},
};

const char ParkedMenuItems[NUMBER_OF_ELEMENTS][MAX_SIZE] PROGMEM = {
  {"Mow Grass"},
  {"Go To Dock"}, //if (Use_Charging_Station == 0) lcd.print(F("Dock Removed"));
  {"Trampoline Cut"},
  {"Test Menu   ->"},
  {"Setup Mower ->"},
  {"-- Spare 6  --"},
  {"-- Spare 7  --"},
  {"-- Spare 8  --"},
  {"-- Spare 9  --"},
  {"-- Spare 10 --"},
  {"-- Spare 11 --"},
};

const char TestingMenuItems[NUMBER_OF_ELEMENTS][MAX_SIZE] PROGMEM = { 
 {"Wire Test"}, 
 {"Relay Test"}, 
 {"Wheel Test"}, 
 {"Blade Test"}, 
 {"Sonar Test"}, 
 {"Turn Test"}, 
 {"Volt Amp Test"}, 
 {"Compass Test"}, 
 {"Spare 9"},
 {"Spare 10"}, 
 {"Spare 11"}, 
};

const char SettingsMenuItems[NUMBER_OF_ELEMENTS][MAX_SIZE] PROGMEM = { 
 {"Alarm 1"}, 
 {"Alarm 2"}, 
 {"Alarm 3"}, 
 {"Wheel Speed LH"}, 
 {"Wheel Speed RH"}, 
 {"Blade Speed"}, 
 {"Compass ON/OFF"}, 
 {"Tracking PID"}, 
 {"Set Clock"},
 {"-----"}, 
 {"CLEAR EEPROM"}, 
};

//-----------------------------------------------------------------------------------------------------------------------------------

void Activate_Menu_Option_Docked();
void Activate_Menu_Option_Parking();
void Activate_Menu_Option_Testing();
void Activate_Menu_Option_Settings();

//-----------------------------------------------------------------------------------------------------------------------------------

// Calculate the time difference, and account for roll over too
unsigned long calcTimeDiff(unsigned long start, unsigned long end)
{
  // inspired by http://www.arduino.cc/playground/Code/TimingRollover saves 200bytes
  return (long)(end - start);
}
//-----------------------------------------------------------------------------------------------------------------------------------


#endif
