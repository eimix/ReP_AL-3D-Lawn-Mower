// Loads the saved values from EEPROM
// to the settings menu.

#define EEPROMStructureVersion 2

//TODO:

//DONE
//1. Write structure version to EEPROM and ignore all config if older version is found
//---------------------------------------------------------------------------------------

void Save_EEPROM_Data()
{
  EEPROM.write(1, EEPROMStructureVersion);

  EEPROM.write(2, 1);
  EEPROM.write(3, Alarm_1_Hour);
  EEPROM.write(4, Alarm_1_Minute);
  EEPROM.write(5, Alarm_1_ON);

  EEPROM.write(6, 1);
  EEPROM.write(7, Alarm_2_Hour);
  EEPROM.write(8, Alarm_2_Minute);
  EEPROM.write(9, Alarm_2_ON);

  EEPROM.write(10, 1);
  EEPROM.write(11, Alarm_3_Hour);
  EEPROM.write(12, Alarm_3_Minute);
  EEPROM.write(13, Alarm_3_ON);

  EEPROM.write(14, 1);
  EEPROM.write(15, PWM_MaxSpeed_LH);

  EEPROM.write(16, 1);
  EEPROM.write(17, PWM_MaxSpeed_RH);

  EEPROM.write(18, 1);
  EEPROM.write(19, PWM_Blade_Speed);

  EEPROM.write(20 , 1);
  EEPROM.write(21 , Compass_Activate);

  EEPROM.write(22, 1);
  EEPROM.write(23, (PID_P * 100));
}
//---------------------------------------------------------------------------------------

void Load_EEPROM_Saved_Data()
{
  int Version = EEPROM.read(1);
  if (Version != EEPROMStructureVersion)
    return;

  byte Alarm_1_Saved_EEPROM = EEPROM.read(2);
  if (Alarm_1_Saved_EEPROM == 1)
  {
    Alarm_1_Hour   = EEPROM.read(3);
    Alarm_1_Minute = EEPROM.read(4);
    Alarm_1_ON     = EEPROM.read(5);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Alarm 1 time from EEPROM "));
    Serial.print(Alarm_1_Hour);
    Serial.print(":");
    if (Alarm_1_Minute < 10) Serial.print("0");
    Serial.println(Alarm_1_Minute);
#endif
  }

  byte Alarm_2_Saved_EEPROM = EEPROM.read(6);
  if (Alarm_2_Saved_EEPROM == 1)
  {
    Alarm_2_Hour   = EEPROM.read(7);
    Alarm_2_Minute = EEPROM.read(8);
    Alarm_2_ON     = EEPROM.read(9);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Alarm 2 time from EEPROM "));
    Serial.print(Alarm_2_Hour);
    Serial.print(":");
    if (Alarm_2_Minute < 10) Serial.print("0");
    Serial.println(Alarm_2_Minute);
#endif
  }

  byte Alarm_3_Saved_EEPROM = EEPROM.read(10);
  if (Alarm_3_Saved_EEPROM == 1)
  {
    Alarm_3_Hour   = EEPROM.read(11);
    Alarm_3_Minute = EEPROM.read(12);
    Alarm_3_ON     = EEPROM.read(13);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Alarm 3 time from EEPROM "));
    Serial.print(Alarm_3_Hour);
    Serial.print(":");
    if (Alarm_3_Minute < 10) Serial.print("0");
    Serial.println(Alarm_3_Minute);
#endif
  }

  byte PWM_LEFT_EEPROM = EEPROM.read(14);
  if (PWM_LEFT_EEPROM == 1)
  {
    PWM_MaxSpeed_LH = EEPROM.read(15);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("PWM Wheel Left value from EEPROM : "));
    Serial.println(PWM_MaxSpeed_LH);
#endif
  }

  byte PWM_RIGHT_EEPROM = EEPROM.read(16);
  if (PWM_RIGHT_EEPROM == 1)
  {
    PWM_MaxSpeed_RH = EEPROM.read(17);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("PWM Wheel Right value from EEPROM : "));
    Serial.println(PWM_MaxSpeed_RH);
#endif
  }

  byte PWM_BLADE_EEPROM = EEPROM.read(18);
  if (PWM_BLADE_EEPROM == 1)
  {
    PWM_Blade_Speed = EEPROM.read(19);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("PWM Blade value from EEPROM : "));
    Serial.println(PWM_Blade_Speed);
#endif
  }

  byte COMPASS_EEPROM = EEPROM.read(20);
  if (COMPASS_EEPROM == 1)
  {
    Compass_Activate = EEPROM.read(21);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Compass Settings from EEPROM : "));
#endif
#if (DEBUG_LEVEL >= 3)
    if (Compass_Activate == 0) Serial.println("OFF");
    if (Compass_Activate == 1) Serial.println("ON");
#endif
  }
  //Compass_Activate = 0;

  byte Traking_PID_P_EEPROM = EEPROM.read(22);
  if (Traking_PID_P_EEPROM == 1)
  {
    PID_P = EEPROM.read(23);
    PID_P = PID_P / 100;
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Traking PID P Setting from EEPROM : "));
    Serial.println(PID_P);
#endif
  }
//  delay(500); //WHY???
}
//---------------------------------------------------------------------------------------

void Clear_EERPOM()
{
  EEPROM.write(1, 0);     // Clear Version
  EEPROM.write(2, 0);     // Clear Alarm 1
  EEPROM.write(6, 0);     // Clear Alarm 2
  EEPROM.write(10, 0);     // Clear Alarm 3
  EEPROM.write(14, 0);    // Clear PWM Left Wheel
  EEPROM.write(16, 0);    // Clear PWM Right Wheel
  EEPROM.write(18, 0);    // Clear PWM Blade
  EEPROM.write(20, 0);    // Clear Compass
  EEPROM.write(22, 0);    // Clear PID
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("All EEPROM Settings Cleared"));
#endif
//  delay(1000); //WHY?
}
//---------------------------------------------------------------------------------------
