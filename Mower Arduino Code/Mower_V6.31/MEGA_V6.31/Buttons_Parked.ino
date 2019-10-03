//-----------------------------------------------------------------------------------------------------------------------------------
// BUTTONS PARKED MENU

//-----------------------------------------------------------------------------------------------------------------------------------

void Check_Membrane_Switch_Input_Parked()
{
  //Menu Options if the Mower is Parked
  Read_Membrane_Keys();
  Menu_Complete       = 1;
  Menu_Mode_Selection = 0;
  Menu_View           = 0;

  if (!Start_Key_X)                                      // If the start key is pressed
  {
    lawn_delay(ButtonPressDelay);
#if (DEBUG_LEVEL >= 3)
    Serial.println();
    Serial.println(F("Start Key Pressed"));
#endif
    Print_Membrane_Switch_Input("Parked", (const char *)&ParkedMenuItems, Activate_Menu_Option_Parked); 
    //Print_Membrane_Switch_Input_Parked();
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

// Defines the actions when that option is selected with the keypad.
void Activate_Menu_Option_Parked()
{
  if (Menu_Mode_Selection == 1)
  {
    Menu_Mode_Selection = 0;
    lcd.clear();
    Print_Membrane_Switch_Input_Timing();
    lcd.clear();
    lcd.print(F("Mow Re-Starting"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Mower Starting"));
#endif
    lawn_delay(1000);
    Manouver_Start_Mower();                                             // Restarts the mower again from standing position
    lcd.clear();
  }

  if (Menu_Mode_Selection == 2)
  {
    if (Use_Charging_Station == 1)
    {
      lcd.clear();
      lcd.print(F("Returning Home"));
#if (DEBUG_LEVEL >= 3)
      Serial.println(F("Sending Mower Home"));
#endif
      lawn_delay(100);
      Menu_Mode_Selection = 0;                                      // Releases the loop in the membrane button section.
      lawn_delay(1000);
      lcd.clear();
      Manouver_Go_To_Charging_Station();
    }
    if (Use_Charging_Station == 0)
    {
      lcd.clear();
      lcd.print(F("No Dock Active"));
#if (DEBUG_LEVEL >= 3)
      Serial.println(F("Activate Docking Station in Settings"));
#endif
      lawn_delay(100);
      Menu_Mode_Selection = 0;                                      // Releases the loop in the membrane button section.
      lawn_delay(1000);
      lcd.clear();
    }
  }

  if (Menu_Mode_Selection == 3)
  {
    lcd.clear();
    lcd.print(F("Trampoline Cut!"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Mower Set to Cut under Trampoline"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(3000);
    lcd.clear();
    Special_Cut_Under_Trampoline_Function();
  }

  if (Menu_Mode_Selection == 4)
  {
    lcd.clear();
    lcd.print(F("Test Mower Menu"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Test Menu Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    Print_Membrane_Switch_Input("Testing", (const char *)&TestingMenuItems, Activate_Menu_Option_Testing); 
    //Print_Membrane_Switch_Input_Testing();
  }

  if (Menu_Mode_Selection == 5)
  {
    lcd.clear();
    lcd.print(F("Setup Mower"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Mower Setup Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    Print_Membrane_Switch_Input("Settings", (const char *)&SettingsMenuItems, Activate_Menu_Option_Settings); 
    //Print_Membrane_Switch_Input_Settings();
  }
/*
  if (Menu_Mode_Selection == 6)
  {
    lcd.clear();
    lcd.print(F("Slot 6 - Empty"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Slot 6 Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    // Insert Function Here();
  }
*/  
}
//-----------------------------------------------------------------------------------------------------------------------------------
