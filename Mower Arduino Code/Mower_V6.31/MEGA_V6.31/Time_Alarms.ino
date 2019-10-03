//-----------------------------------------------------------------------------------------------------------------------------------
/*
#ifdef UseClockDS1302
String dayAsString(const Time::Day day)
{
  switch (day)
  {
    case Time::kSunday:    return "Sunday";
    case Time::kMonday:    return "Monday";
    case Time::kTuesday:   return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday:  return "Thursday";
    case Time::kFriday:    return "Friday";
    case Time::kSaturday:  return "Saturday";
  }
  return "(unknown day)";
}
//-----------------------------------------------------------------------------------------------------------------------------------
#endif
*/
//-----------------------------------------------------------------------------------------------------------------------------------

void Setup_RTC()
{
#ifdef UseClockDS1307
  if (!rtc.begin()) 
  {
#if (DEBUG_LEVEL >= 1)
    Serial.println(F("Couldn't find RTC"));
#endif
  } 
  
  if (!rtc.isrunning()) //Lost track of time, need adjust
  {
#if (DEBUG_LEVEL >= 1)
    Serial.println(F("RTC is NOT running!"));    
#endif
    Set_Time_On_RTC();
  }
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------

// digital clock display of the time
void DisplayTime()
{
#if (DEBUG_LEVEL >= 2)

#ifdef UseClockDS1302
  Time t      = rtc.time();
  int year    = t.yr;
  byte month  = t.mon;
  byte day    = t.date;
  byte hour   = t.hr;
  byte minute = t.min;
  byte second = t.sec;
#endif
#ifdef UseClockDS1307
  DateTime t  = rtc.now();
  int year    = t.year();
  byte month  = t.month();
  byte day    = t.day();
  byte hour   = t.hour();
  byte minute = t.minute();
  byte second = t.second();
#endif

  Serial.print(F("Time:"));

  // Name the day of the week.
  // const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           year, month, day,
           hour, minute, second);
  /*  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
             day.c_str(),
             t.yr, t.mon, t.date,
             t.hr, t.min, t.sec);
  */
  // Print the formatted string to serial so we can see the time.
  Serial.print(buf);
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Print_Serial_Time()
{
#if (DEBUG_LEVEL >= 2)

#ifdef UseClockDS1302
  Time t      = rtc.time();
  byte hour   = t.hr;
  byte minute = t.min;
  byte second = t.sec;
#endif
#ifdef UseClockDS1307
  DateTime t  = rtc.now();
  byte hour   = t.hour();
  byte minute = t.minute();
  byte second = t.second();
#endif

  Serial.print("Time:");
  Serial.print(hour);
  Serial.print(":");
  if (minute < 10) Serial.print ("0");
  Serial.print(minute);
  Serial.print(".");
  if (second < 10) Serial.print ("0");
  Serial.print(second);
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Activate_Alarms()
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

  // Manual ALARM 1
  if (Alarm_1_ON == 1)
  {
    if (hour == Alarm_1_Hour && minute == Alarm_1_Minute)
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println("");
      Serial.println(F("ALARM 1"));
#endif
      lawn_delay(2000);
      // Insert action for Alarm 1 Here
      Exit_Zone = 1;
      Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
      Manouver_Exit_To_Zone_X();
    }
  }

  // Manual ALARM 2
  if (Alarm_2_ON == 1)
  {
    if (hour == Alarm_2_Hour && minute == Alarm_2_Minute)
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println("");
      Serial.println(F("ALARM 2"));
#endif
      lawn_delay(2000);
      //Insert action for Alarm 2 Here
      //
      //
    }
  }

  // Manual ALARM 3
  if (Alarm_3_ON == 1)
  {
    if (hour == Alarm_3_Hour && minute == Alarm_3_Minute)
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println("");
      Serial.println(F("ALARM 3"));
#endif
      lawn_delay(2000);
      //Insert action for Alarm 3 Here
      //
      //

    }
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

// Set when choosing an option of 1hr or 2hr mow etc.
void Check_Timed_Mow()
{
  if (Alarm_Timed_Mow_ON == 1)
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

    if (hour == Alarm_Timed_Mow_Hour && minute == Alarm_Timed_Mow_Minute)
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println(F("Timed Mow Complete"));
#endif
      lawn_delay(2000);
      //Insert action for Timed Mow Alarm Here
      if (Use_Charging_Station == 1) Manouver_Go_To_Charging_Station();                       // Stops the mowing and sends the mower back to the charging station via the perimeter wire
      if (Use_Charging_Station == 0) Manouver_Park_The_Mower_Low_Batt();                      // Parks the mower with a low battery warning
    }
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

// Prints the alarms set to the serial monitor
void Display_Next_Alarm()
{
#if (DEBUG_LEVEL >= 2)
  //Print_Day();

  if (Alarm_1_ON == 1)
  {
    Serial.print(F("|Alarm 1:"));
    Serial.print(Alarm_1_Hour);
    Serial.print(F(":"));
    if (Alarm_1_Minute < 10) Serial.print ("0");
    Serial.print(Alarm_1_Minute);
    Serial.print("|");
  }
  if (Alarm_1_ON == 0)
    Serial.print(F("|Alarm 1 OFF"));

  if (Alarm_2_ON == 1)
  {
    Serial.print(F("|Alarm 2:"));
    Serial.print(Alarm_2_Hour);
    Serial.print(F(":"));
    if (Alarm_2_Minute < 10) Serial.print ("0");
    Serial.print(Alarm_2_Minute);
    Serial.print("|");
  }
  if (Alarm_2_ON == 0)
    Serial.print(F("|Alarm 2 OFF"));

  if (Alarm_3_ON == 1)
  {
    Serial.print(F("|Alarm 3:"));
    Serial.print(Alarm_3_Hour);
    Serial.print(F(":"));
    if (Alarm_3_Minute < 10) Serial.print ("0");
    Serial.print(Alarm_3_Minute);
    Serial.print("|");
  }
  if (Alarm_3_ON == 0)
    Serial.print(F("|Alarm 3 OFF"));
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Set_Time_On_RTC()
{
  // SetStartupTime to 1 in the setting menu to set time.  Load the sketch then immediatley SetStartupTime = 0 and reload the sketch.
#ifdef UseClockDS1302
  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(2019, 07, 12, 15, 8, 00, Time::kFriday);            // Year XXXX, Month XX, Day XX, Hour XX, Minute XX, Second, kXYZday
  rtc.time(t);
  delay(10);
#endif
#ifdef UseClockDS1307
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Manage_Alarms()
{
  Alarm_Timed_Mow_ON = 0;                                           // Turns off the 1 hr Alarm
  if (Alarm_1_Repeat == 0) Alarm_1_ON = 0;
  if (Alarm_2_Repeat == 0) Alarm_2_ON = 0;
  if (Alarm_3_Repeat == 0) Alarm_3_ON = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------
