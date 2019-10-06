// BUTTONS SETTINGS MENU

// TODO
// 1. BUG Recursion (infinite): (Print_Membrane_Switch_Input_Settings -> Activate_Menu_Option_Settings)
//    Solution: Use local Menu_Complete and disable Print_Membrane_Switch_Input_Settings call in last function
// 2. 6.4 ReP_AL version has additional settings menu items (SettingsMenuItems)

// DONE
// 1. OPTIMZE Activate_Menu_Option_Settings code optimize, remove commented code after tests
// 2. BUG First level menu has 12 items, and code allows go beond that
// 3. OPTIMZE Use array for menu List: http://www.gammon.com.au/progmem
//-----------------------------------------------------------------------------------------------------------------------------------

void Activate_Menu_Option_Settings()
{
  bool NeedSaveEEPROM = false;
  if (Menu_Mode_Selection == 1)
  {
    // Alarm 1 Settings
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Alarm_1 : "));
    lcd.print(Alarm_1_Hour);
    lcd.print(F(":"));
    if (Alarm_1_Minute < 10) lcd.print ("0");
    lcd.print(Alarm_1_Minute);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Alarm 1 Status : "));
    Serial.println(Alarm_1_ON);
#endif
    lcd.setCursor(0, 1);
    if (Alarm_1_ON == 1) lcd.print("Active");
    if (Alarm_1_ON == 0) lcd.print("OFF");
    Menu_Complete = false;
    lawn_delay(500);

    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_1 :"));
        lcd.print(Alarm_1_Hour);
        lcd.print(F(":"));
        if (Alarm_1_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_1_Minute);
        lcd.setCursor(0, 1);
        lcd.print(F("ON  SAVED"));
        Alarm_1_ON = 1;
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;

        NeedSaveEEPROM = true;
        //        EEPROM.write(2, 1);
        //        EEPROM.write(3, Alarm_1_Hour);
        //        EEPROM.write(4, Alarm_1_Minute);
        //        EEPROM.write(5, Alarm_1_ON);
      }

      if (!Plus_Key_X)
      {
        Alarm_1_Minute++;
        if (Alarm_1_Minute > 59)
        {
          Alarm_1_Minute = 0;
          Alarm_1_Hour++;
        }
        if (Alarm_1_Hour > 23) Alarm_1_Hour = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_1 : "));
        lcd.print(Alarm_1_Hour);
        lcd.print(F(":"));
        if (Alarm_1_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_1_Minute);
      }

      if (!Minus_Key_X)
      {
        Alarm_1_Minute--;
        if (Alarm_1_Minute < 0)
        {
          Alarm_1_Minute = 59;
          Alarm_1_Hour--;
          if (Alarm_1_Hour < 0) Alarm_1_Hour = 23;
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_1 : "));
        lcd.print(Alarm_1_Hour);
        lcd.print(F(":"));
        if (Alarm_1_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_1_Minute);
      }

      if (!Stop_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Alarm Cancelled"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_1 :"));
        lcd.print(Alarm_1_Hour);
        lcd.print(F(":"));
        if (Alarm_1_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_1_Minute);
        lcd.setCursor(0, 1);
        lcd.print(F("Alarm OFF"));
        Alarm_1_ON = 0;
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;

        NeedSaveEEPROM = true;
        //        EEPROM.write(2, 1);                                // Show the Alarm has been saved to the EEPROM
        //        EEPROM.write(5, Alarm_1_ON);                       // Saves that the alarm has been cancelled.
      }
    }
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Alarm 1 Status : "));
    Serial.println(Alarm_1_ON);
#endif
    lawn_delay(1000);
  } //Menu_Mode_Selection

  if (Menu_Mode_Selection == 2)
  {
    // Alarm 2 Settings
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Alarm_2 : "));
    lcd.print(Alarm_2_Hour);
    lcd.print(F(":"));
    if (Alarm_2_Minute < 10) lcd.print ("0");
    lcd.print(Alarm_2_Minute);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Alarm 2 Status : "));
    Serial.println(Alarm_2_ON);
#endif
    lcd.setCursor(0, 1);
    if (Alarm_2_ON == 1) lcd.print("Active");
    if (Alarm_2_ON == 0) lcd.print("OFF");
    Menu_Complete = false;
    lawn_delay(500);
    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_2 :"));
        lcd.print(Alarm_2_Hour);
        lcd.print(F(":"));
        if (Alarm_2_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_2_Minute);
        lcd.setCursor(0, 1);
        lcd.print(F("ON  SAVED"));
        Alarm_2_ON = 1;
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;

        NeedSaveEEPROM = true;
        //        EEPROM.write(6, 1);
        //        EEPROM.write(7, Alarm_2_Hour);
        //        EEPROM.write(8, Alarm_2_Minute);
        //        EEPROM.write(9, Alarm_2_ON);
      }
      if (!Plus_Key_X)
      {
        Alarm_2_Minute++;
        if (Alarm_2_Minute > 59)
        {
          Alarm_2_Minute = 0;
          Alarm_2_Hour++;
        }
        if (Alarm_2_Hour > 23) Alarm_2_Hour = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_2 : "));
        lcd.print(Alarm_2_Hour);
        lcd.print(F(":"));
        if (Alarm_2_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_2_Minute);
      }
      if (!Minus_Key_X)
      {
        Alarm_2_Minute--;
        if (Alarm_2_Minute < 0)
        {
          Alarm_2_Minute = 59;
          Alarm_2_Hour--;
          if (Alarm_2_Hour < 0) Alarm_2_Hour = 23;
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alarm_2 : ");
        lcd.print(Alarm_2_Hour);
        lcd.print(F(":"));
        if (Alarm_2_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_2_Minute);
      }

      if (!Stop_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Alarm Cancelled"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_2 :"));
        lcd.print(Alarm_2_Hour);
        lcd.print(F(":"));
        if (Alarm_2_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_2_Minute);
        lcd.setCursor(0, 1);
        lcd.print(F("Alarm OFF"));
        Alarm_2_ON = 0;
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
        NeedSaveEEPROM = true;
        //        EEPROM.write(6, 1);
        //        EEPROM.write(9, Alarm_2_ON);
      }
    }
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Alarm 2 Status : "));
    Serial.println(Alarm_2_ON);
#endif
    lawn_delay(1000);
  }

  if (Menu_Mode_Selection == 3)
  {
    // Alarm 3 Settings
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Alarm_3 : "));
    lcd.print(Alarm_3_Hour);
    lcd.print(F(":"));
    if (Alarm_3_Minute < 10) lcd.print ("0");
    lcd.print(Alarm_3_Minute);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Alarm 3 Status : "));
    Serial.println(Alarm_3_ON);
#endif
    lcd.setCursor(0, 1);
    if (Alarm_3_ON == 1) lcd.print("Active");
    if (Alarm_3_ON == 0) lcd.print("OFF");
    Menu_Complete = false;
    lawn_delay(500);

    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_3 :"));
        lcd.print(Alarm_3_Hour);
        lcd.print(F(":"));
        if (Alarm_3_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_3_Minute);
        lcd.setCursor(0, 1);
        lcd.print(F("ON  SAVED"));
        Alarm_3_ON = 1;
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
        NeedSaveEEPROM = true;
        //        EEPROM.write(10, 1);
        //        EEPROM.write(11, Alarm_3_Hour);
        //        EEPROM.write(12, Alarm_3_Minute);
        //        EEPROM.write(13, Alarm_3_ON);
      }
      if (!Plus_Key_X)
      {
        Alarm_3_Minute++;
        if (Alarm_3_Minute > 59)
        {
          Alarm_3_Minute = 0;
          Alarm_3_Hour++;
        }
        if (Alarm_3_Hour > 23) Alarm_3_Hour = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_3 : "));
        lcd.print(Alarm_3_Hour);
        lcd.print(F(":"));
        if (Alarm_3_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_3_Minute);
      }

      if (!Minus_Key_X)
      {
        Alarm_3_Minute--;
        if (Alarm_3_Minute < 0)
        {
          Alarm_3_Minute = 59;
          Alarm_3_Hour--;
          if (Alarm_3_Hour < 0) Alarm_3_Hour = 23;
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_3 : "));
        lcd.print(Alarm_3_Hour);
        lcd.print(F(":"));
        if (Alarm_3_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_3_Minute);
      }

      if (!Stop_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Alarm Cancelled"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Alarm_3 :"));
        lcd.print(Alarm_3_Hour);
        lcd.print(F(":"));
        if (Alarm_3_Minute < 10) lcd.print ("0");
        lcd.print(Alarm_3_Minute);
        lcd.setCursor(0, 1);
        lcd.print(F("Alarm OFF"));
        Alarm_3_ON = 0;
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
        NeedSaveEEPROM = true;
        //        EEPROM.write(10, 1);
        //        EEPROM.write(13, Alarm_3_ON);
      }
    }
#if (DEBUG_LEVEL >= 3)
    Serial.print("Alarm 3 Status : ");
    Serial.println(Alarm_3_ON);
#endif
    lawn_delay(1000);
  }

  if (Menu_Mode_Selection == 4)
  {
    // Wheel PWM Settings
    Menu_Mode_Selection = 0;
    lcd.clear();
    lawn_delay(100);
    lcd.setCursor(0, 0);
    lcd.print(F("Wheel Speed LH:"));
    lcd.setCursor(0, 1);
    lcd.print(PWM_MaxSpeed_LH);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("PWM_L:"));
    Serial.println(PWM_MaxSpeed_LH);
#endif
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PWM_L:");
        lcd.print(PWM_MaxSpeed_LH);
        lcd.setCursor(0, 1);
        lcd.print("SAVED");
        lawn_delay(2000);
        lcd.clear();

        NeedSaveEEPROM = true;
        //        EEPROM.write(14, 1);
        //        EEPROM.write(15, PWM_MaxSpeed_LH);

        Menu_Mode_Selection = 0;
      }
      if (!Plus_Key_X)
      {
        PWM_MaxSpeed_LH = PWM_MaxSpeed_LH + 1;
        if (PWM_MaxSpeed_LH > 255) PWM_MaxSpeed_LH = 255;
        lcd.setCursor(0, 1);
        lcd.print("      ");    // Fully clear the number to stop reminants of a previous number from being left behind
        lcd.setCursor(0, 1);
        lcd.print(PWM_MaxSpeed_LH);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("PWM_L:"));
        Serial.println(PWM_MaxSpeed_LH);
#endif
      }
      if (!Minus_Key_X)
      {
        PWM_MaxSpeed_LH = PWM_MaxSpeed_LH - 1;
        if (PWM_MaxSpeed_LH < 0) PWM_MaxSpeed_LH = 0;
        lcd.setCursor(0, 1);
        lcd.print("      ");   // Fully clear the number to stop reminants of a previous number from being left behind
        lcd.setCursor(0, 1);
        lcd.print(PWM_MaxSpeed_LH);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("PWM_L:"));
        Serial.println(PWM_MaxSpeed_LH);
#endif
      }
    }
  }

  if (Menu_Mode_Selection == 5)
  {
    // Wheel RH PWM Settings
    Menu_Mode_Selection = 0;
    lcd.clear();
    lawn_delay(100);
    lcd.setCursor(0, 0);
    lcd.print(F("Wheel Speed RH:"));
    lcd.setCursor(0, 1);
    lcd.print(PWM_MaxSpeed_RH);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("PWM_R:"));
    Serial.println(PWM_MaxSpeed_RH);
#endif
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PWM_R:");
        lcd.print(PWM_MaxSpeed_RH);
        lcd.setCursor(0, 1);
        lcd.print("SAVED");
        lawn_delay(2000);
        lcd.clear();

        NeedSaveEEPROM = true;
        //        EEPROM.write(16, 1);
        //        EEPROM.write(17, PWM_MaxSpeed_RH);
        Menu_Mode_Selection = 0;
      }
      if (!Plus_Key_X)
      {
        PWM_MaxSpeed_RH = PWM_MaxSpeed_RH + 1;
        if (PWM_MaxSpeed_RH > 255) PWM_MaxSpeed_RH = 255;
        lcd.setCursor(0, 1);
        lcd.print("      ");    // Fully clear the number to stop reminants of a previous number from being left behind
        lcd.setCursor(0, 1);
        lcd.print(PWM_MaxSpeed_RH);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("PWM_R:"));
        Serial.println(PWM_MaxSpeed_RH);
#endif
      }
      if (!Minus_Key_X)
      {
        PWM_MaxSpeed_RH = PWM_MaxSpeed_RH - 1;
        if (PWM_MaxSpeed_RH < 0) PWM_MaxSpeed_RH = 0;
        lcd.setCursor(0, 1);
        lcd.print("      ");   // Fully clear the number to stop reminants of a previous number from being left behind
        lcd.setCursor(0, 1);
        lcd.print(PWM_MaxSpeed_RH);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("PWM_R:"));
        Serial.println(PWM_MaxSpeed_RH);
#endif
      }
    }
  }

#ifdef MowMotorDriver_BTS7960
  if (Menu_Mode_Selection == 6)
  {
    // Blade PWM Settings
    Menu_Mode_Selection = 0;
    lcd.clear();
    lawn_delay(500);
    lcd.setCursor(0, 0);
    lcd.print(F("Blade Speed:"));
    lcd.setCursor(0, 1);
    lcd.print(PWM_Blade_Speed);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Blade PWM:"));
    Serial.println(PWM_Blade_Speed);
#endif
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Blade PWM:"));
        lcd.print(PWM_Blade_Speed);
        lcd.setCursor(0, 1);
        lcd.print("SAVED");
        lawn_delay(2000);
        lcd.clear();

        NeedSaveEEPROM = true;
        //        EEPROM.write(18, 1);
        //        EEPROM.write(19, PWM_Blade_Speed);
        Menu_Mode_Selection = 0;
      }
      if (!Plus_Key_X)
      {
        PWM_Blade_Speed = PWM_Blade_Speed + 1;
        if (PWM_Blade_Speed > 255) PWM_Blade_Speed = 255;
        lcd.setCursor(0, 1);
        lcd.print("      ");    // Fully clear the number to stop reminants of a previous number from being left behind
        lcd.setCursor(0, 1);
        lcd.print(PWM_Blade_Speed);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("Blade PWM:"));
        Serial.println(PWM_Blade_Speed);
#endif
      }
      if (!Minus_Key_X)
      {
        PWM_Blade_Speed = PWM_Blade_Speed - 1;
        if (PWM_Blade_Speed < 0) PWM_Blade_Speed = 0;
        lcd.setCursor(0, 1);
        lcd.print("      ");   // Fully clear the number to stop reminants of a previous number from being left behind
        lcd.setCursor(0, 1);
        lcd.print(PWM_Blade_Speed);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("Blade PWM:"));
        Serial.println(PWM_Blade_Speed);
#endif
      }
    }
  }
#endif //Motor type

  if (Menu_Mode_Selection == 7)
  {
    // Compass setup
    lcd.clear();
    lcd.print(F("Compass Setup"));
    lawn_delay(1000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Compass ON/OFF"));
    lcd.setCursor(0, 1);
    lcd.print("Status : ");
    if (Compass_Activate == 1) lcd.print("ON ");
    if (Compass_Activate == 0) lcd.print("OFF");

    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Compass Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Compass Saved"));
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("Compass:"));
        Serial.println(Compass_Activate);
#endif
        lawn_delay(2000);
        lcd.clear();

        NeedSaveEEPROM = true;
        //        EEPROM.write(20 , 1);
        //        EEPROM.write(21 , Compass_Activate);
        if (Compass_Activate == 1) Setup_Compass();
        Menu_Mode_Selection = 0;

      }
      if (!Plus_Key_X)
      {
        lcd.setCursor(0, 1);
        lcd.print("Status : ");
        Compass_Activate = 1;
        lcd.print("ON ");
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("Compass:"));
        Serial.println(Compass_Activate);
#endif
        lawn_delay(100);
      }
      if (!Minus_Key_X)
      {
        lcd.setCursor(0, 1);
        lcd.print("Status : ");
        Compass_Activate = 0;
        lcd.print("OFF");
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("Compass:"));
        Serial.println(Compass_Activate);
#endif
        lawn_delay(100);
      }
    }
  }

  if (Menu_Mode_Selection == 8)
  {
    // Tracking PID Settings
    Menu_Mode_Selection = 0;
    lcd.clear();
    lawn_delay(500);
    lcd.setCursor(0, 0);
    lcd.print(F("Tracking PID:"));
    lcd.setCursor(0, 1);
    lcd.print("P = ");
    lcd.print(PID_P);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Tracking PID P = :"));
    Serial.println(PID_P);
#endif
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("P : ");
        lcd.print(PID_P);
        lcd.setCursor(0, 1);
        lcd.print("SAVED");
        lawn_delay(2000);
        lcd.clear();

        NeedSaveEEPROM = true;
        //        EEPROM.write(22, 1);
        //        EEPROM.write(23, (P*100));
        Menu_Mode_Selection = 0;
      }
      if (!Plus_Key_X)
      {
        PID_P = PID_P + 0.01;
        if (PID_P > 10) PID_P = 10;
        lcd.setCursor(0, 1);
        lcd.print("      ");    // Fully clear the number to stop reminants of a previous number from being left behind
        lcd.setCursor(0, 1);
        lcd.print("P : ");
        lcd.print(PID_P);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("Tracking PID P = :"));
        Serial.println(PID_P);
#endif
      }
      if (!Minus_Key_X)
      {
        PID_P = PID_P - 0.01;
        if (PID_P < 0) PID_P = 0;
        lcd.setCursor(0, 1);
        lcd.print("      ");   // Fully clear the number to stop reminants of a previous number from being left behind
        lcd.setCursor(0, 1);
        lcd.print("P : ");
        lcd.print(PID_P);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("Tracking PID P = :"));
        Serial.println(PID_P);
#endif
      }
    }
  }

  // Set Mower CLock
  if (Menu_Mode_Selection == 9)
  {
    // Set Mower Clock
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.setCursor(0, 0);           // Spaces to the right & down
    
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
//    int set_hour = t.hr;
//    int set_min = t.min;

    lcd.print(F("Time : "));
    lcd.print(hour);
    lcd.print(":");
    if (minute < 10) lcd.print ("0");
    lcd.print(minute);
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Time : "));
    Serial.print(hour);
    Serial.print(":");
    if (minute < 10) Serial.print ("0");
    Serial.println(minute);
#endif

    Menu_Complete = false;
    lawn_delay(500);
    while (Menu_Complete == false)
    {
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Settings Saved"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.print(F("Time : "));
        lcd.print(hour);
        lcd.print(":");
        if (minute < 10) lcd.print ("0");
        lcd.print(minute);
        lcd.setCursor(0, 1);
#if (DEBUG_LEVEL >= 3)
        Serial.print(F("Clock : "));
        Serial.print(hour);
        Serial.print(":");
        if (minute < 10) Serial.print("0");
        Serial.println(minute);
#endif
        
#ifdef UseClockDS1302
        rtc.writeProtect(false);
        rtc.halt(false);
        Time t(RTCSketchYear, RTCSketchMonth, RTCSketchDay, hour, minute, 00, Time::kFriday);            // Year XXXX, Month XX, Day XX, Hour XX, Minute XX, Second, kXYZday
        rtc.time(t);
        lawn_delay(200);
        rtc.writeProtect(true);
        rtc.halt(true);
        rtc.time(t);
#endif
#ifdef UseClockDS1307
        rtc.adjust(DateTime(RTCSketchYear, RTCSketchMonth, RTCSketchDay, hour, minute, 0));
#endif
        lcd.print(F("TIME SAVED"));
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
      if (!Plus_Key_X)
      {
        minute++;
        if (minute > 59)
        {
          minute = 0;
          hour++;
        }
        if (hour > 23) hour = 0;
        lcd.clear();
        lcd.print(F("Time : "));
        lcd.print(hour);
        lcd.print(":");
        if (minute < 10) lcd.print ("0");
        lcd.print(minute);
      }
      if (!Minus_Key_X)
      {
        minute--;
        if (minute < 0)
        {
          minute = 59;
          hour--;
        }
        if (hour < 0) hour = 23;
        lcd.clear();
        lcd.print("Time : ");
        lcd.print(hour);
        lcd.print(":");
        if (minute < 10) lcd.print ("0");
        lcd.print(minute);
      }
      if (!Stop_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Time Set Cancelled"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Time Set"));
        lcd.setCursor(0, 1);
        lcd.print(F("Cancelled"));
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
    }
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Time : "));
#endif
    lawn_delay(1000);
  }

  if (Menu_Mode_Selection == 10)
  {
    // Enter Settings Here
    lcd.clear();
    lcd.print(F(".........."));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("...................."));
#endif
    lawn_delay(5000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Stop_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Stop key is pressed"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Test Stopped"));
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
    }
  }

  // Menu Option to CLEAR EEPROM
  //-----------------------------

  if (Menu_Mode_Selection == 11)
  {
    // EEPORM Clear MENU
    lcd.clear();
    lcd.print(F("EEPROM Clear ?"));
    lcd.setCursor(0, 1);
    lcd.print(F("Yes/No"));
#if (DEBUG_LEVEL >= 2)
    Serial.println(F("Clear EEPROM Yes/No   - Press Up for Yes and Down for No.  Start to Confirm"));
#endif
    Menu_Mode_Selection = 0;
    Menu_Complete = false;
    int Answer = 0;

    while (Menu_Complete == false)
    {
      //Enter Code Here to Cycle until stop key is pressed.
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      if (!Start_Key_X)
      {
#if (DEBUG_LEVEL >= 3)
        Serial.println(F("Stop key is pressed"));
#endif
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        if (Answer == 1)
        {
          Clear_EERPOM();
          lcd.clear();
          lcd.print(F("EEPROM Cleared"));
          lcd.setCursor(0, 1);
          lcd.print(F("Power OFF Mower"));
        }
        if (Answer == 0)
        {
          lcd.clear();
          lcd.print(F("Cancelled"));
          lcd.setCursor(0, 1);
        }
        lawn_delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
      if (!Plus_Key_X)
      {
        Answer = 1;
        lcd.setCursor(0, 1);
        lcd.print(F("Yes    "));
#if (DEBUG_LEVEL >= 2)
        Serial.println(F("Clear EEPROM = YES....  Press Start to Confirm"));
#endif
        lawn_delay(100);
      }
      if (!Minus_Key_X)
      {
        Answer = 0;
        lcd.setCursor(0, 1);
        lcd.print(F("No     "));
#if (DEBUG_LEVEL >= 2)
        Serial.println(F("Clear EEPROM = No....Press Start to Confirm"));
#endif
        lawn_delay(100);
      }
    }
  }

  if (NeedSaveEEPROM)
    Save_EEPROM_Data();

  Print_Membrane_Switch_Input("Settings", (const char *)&SettingsMenuItems, Activate_Menu_Option_Settings); 
  //Print_Membrane_Switch_Input_Settings();
}
//-----------------------------------------------------------------------------------------------------------------------------------
