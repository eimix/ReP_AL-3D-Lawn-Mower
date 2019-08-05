#ifndef GLOBALS_H
#define GLOBALS_H

//Battery voltage if <Min - need charge; if >Max - stop charge
float BatteryVoltage; //Volts
//Charge current: if < 0.4A - charging finished
float ChargeCurrent;  //Amps
//Load curreent: if > 6A - motor locked, pause wait for user
float LoadCurrent;

int LowBatteryCount = 0;

#ifdef UseLiftSensors
byte LiftFrontLeft;
byte LiftFrontRight;
#endif

#ifdef UseBumperSensors
byte BumperFrontLeft;
byte BumperFrontRight;
#endif

#endif
