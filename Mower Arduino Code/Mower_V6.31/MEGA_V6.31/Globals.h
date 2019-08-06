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
#define LiftSensorBAD HIGH //PULLUP HIGH is BAD, LOW is GOOD
#endif

#ifdef UseBumperSensors
byte BumperFrontLeft;
byte BumperFrontRight;
#define BumperSensorHIT HIGH //PULLUP HIGH is HIT, LOW is open path
#endif

//-----------------------------------------------------------------------------------------------------------------------------------

#define MenuTypePROGMEM

#ifdef MenuTypePROGMEM
// Print a string from Program Memory directly to save RAM 
void LCDPrintProgStr(const char *str)
{
  char c;
  if (!str) 
    return;
  while ((c = pgm_read_byte(str++)))
    lcd.print(c);
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------

#endif
