// Infornation to be printed to the Mower LCD screen
//---------------------------------------------------------------------------------------

void Setup_Run_LCD_Intro()
{
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Setup LCD"));
#endif
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print(F("ReP_AL Robot"));
  lcd.setCursor(0, 1);
  lcd.print(Version);
#ifdef UseWiFi
  lcd.setCursor(7, 1);
  lcd.print(F("WIFI ON"));
#endif
  lawn_delay(1000);
  lcd.clear();
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("LCD Setup OK"));
#endif
}
//---------------------------------------------------------------------------------------

void Print_LCD_Mower_Warning()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Mower Warning"));

#if (DEBUG_LEVEL >= 2)
  Serial.print(F("Mower Warning:"));
#endif

#ifdef UseLiftSensors
  if (LiftFrontLeft == LiftSensorBAD && LiftFrontRight == LiftSensorBAD)
  {
    lcd.setCursor(0, 1);
    lcd.print(F("Mower lifted"));

#if (DEBUG_LEVEL >= 2)
    Serial.print(F("Mower lifted;"));
#endif
  }
#endif

  if (LoadCurrent > MaxLoadCurrent)
  {
    lcd.setCursor(0, 1);
    lcd.print(F("High load"));

#if (DEBUG_LEVEL >= 1)
    Serial.print(F("High load;"));
#endif
  }

#if (DEBUG_LEVEL >= 2)
  Serial.println();
#endif
}
//---------------------------------------------------------------------------------------

void Print_LCD_Mower_Error()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Mower Error"));

#if (DEBUG_LEVEL >= 1)
  Serial.println(F("Mower Error"));
#endif
  
  if (Wire_Off > 5)
  {
    lcd.setCursor(0, 1);
    lcd.print(F("Wire OFF"));

#if (DEBUG_LEVEL >= 1)
    Serial.println(F("Wire OFF"));
#endif    
  }
}
//---------------------------------------------------------------------------------------

void Print_LCD_Volt_Info()
{
  lcd.setCursor(10, 0);
  lcd.print("V:");
  //lcd.setCursor(12, 0);
  lcd.print(BatteryVoltage);
}
//---------------------------------------------------------------------------------------

void Print_Charging_LCD()
{
  lcd.setCursor(0, 0);
  if (Mower_Charging == 1)
    lcd.print(F("Charging"));
  if (Rain_Detected == 0 && Mower_Charging != 1)
    lcd.print(F("        "));
}
//---------------------------------------------------------------------------------------

void Print_Raining_LCD()
{
  lcd.setCursor(4, 0);
  if (Rain_Detected == 1)
    lcd.print("Rain");
  // See raining and charging clause if this is not displying correctly.
  if (Rain_Detected == 0 && Mower_Charging == 0)
    lcd.print("    ");
}
//---------------------------------------------------------------------------------------

void Print_LCD_Recharge()
{
  lcd.setCursor(0, 1);
  lcd.print(F("Recharge Battery"));
}
//---------------------------------------------------------------------------------------

void Print_LCD_Wire()
{
  lcd.setCursor(0, 1);
  lcd.print(F("WIRE Detect "));
}
//---------------------------------------------------------------------------------------

void Print_LCD_Mowing()
{
  if (Alarm_Timed_Mow_ON == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(F("Mowing..    "));
  }
  //if (Alarm_Timed_Mow_ON == 1)
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Timer:");
    lcd.print(Alarm_Timed_Mow_Hour);
    lcd.print(":");
    if (Alarm_Timed_Mow_Minute < 10) lcd.print("0");
    lcd.print(Alarm_Timed_Mow_Minute);
    Mow_Time_Set = 1;
  }
}
//---------------------------------------------------------------------------------------

void Print_LCD_Compass_Mowing()
{
  if (Compass_Heading_Locked == 1)
  {
    lcd.setCursor(0, 1);
    if (PWM_Right > PWM_Left)  lcd.print(F("<H-Lock Mow "));
    if (PWM_Left  > PWM_Right)  lcd.print(F(" H-Lock Mow>"));
    if (PWM_Left == PWM_Right) lcd.print(F("|H-Lock Mow|"));
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print(F("            "));
  }
}
//---------------------------------------------------------------------------------------

void Print_LCD_Info_Parked()
{
  lcd.setCursor(0, 1);
  lcd.print("Parked");
}
//---------------------------------------------------------------------------------------

void Print_LCD_Info_Manual()
{
  lcd.setCursor(0, 1);
  lcd.print("Manual");
}
//---------------------------------------------------------------------------------------

void Print_LCD_Info_Docked()
{
  lcd.setCursor(0, 1);
  lcd.print("Docked");
}
//---------------------------------------------------------------------------------------

void Print_LCD_NO_Wire()
{
  if (Mower_Docked == 1 || Mower_Parked == 1)
  {
    lcd.setCursor(7, 1);
    lcd.print(F(":WIRE OFF"));
    Wire_ON_Printed = 0;
  }
  //if (Mower_Docked == 0 && Mower_Parked == 0)
  else
  {
    lcd.setCursor(0, 1);
    lcd.print(F(":WIRE OFF        "));
    Wire_ON_Printed = 0;
  }
}
//---------------------------------------------------------------------------------------

void Print_LCD_Wire_ON()
{
  if (Wire_ON_Printed == 0)
    return;
    
  if (Mower_Docked == 1 || Mower_Parked == 1)
  {
    lcd.setCursor(7, 1);
    lcd.print(F(":               "));
    Wire_ON_Printed = 1;
  }
  //if (Mower_Docked == 0 && Mower_Parked == 0)
  else
  {
    lcd.setCursor(0, 1);
    lcd.print(F(":               "));
    Wire_ON_Printed = 1;
  }
}
//---------------------------------------------------------------------------------------

void Print_Sonar_Hit()
{
  lcd.setCursor(0, 1);
  lcd.print(F("Sonar Object "));
}
//---------------------------------------------------------------------------------------

void Print_LCD_Compass_Home()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Compass Home"));
  if (Rain_Detected == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("RAINING");
  }
}
//---------------------------------------------------------------------------------------

void Print_LCD_Heading_for_Home()
{
  lcd.setCursor(0, 1);
  lcd.print(F("Target:"));
  lcd.print(Compass_Target); //Was some strange: (Heading_Lower_Limit_Compass - Heading_Lower_Limit_Compass) / 2 + Heading_Lower_Limit_Compass;
}
//---------------------------------------------------------------------------------------

void Print_LCD_Time()
{
  if (Mower_Charging == 0 && Mower_Running == 0 && Rain_Detected == 0)
  {
#ifdef UseClockDS1302
    Time t      = rtc.time();
    byte hour   = t.hr;
    byte minute = t.min;
#endif
#ifdef UseClockDS1307
    DateTime t  = rtc.now();
    byte hour   = t.hour();
    byte minute = t.minute();
#endif

    lcd.setCursor(0, 0);            // Spaces to the right & down
    if (hour < 10) lcd.print ("0");
    lcd.print(hour);
    lcd.print(":");
    if (minute < 10) lcd.print ("0");
    lcd.print(minute);
  }
}
//---------------------------------------------------------------------------------------
