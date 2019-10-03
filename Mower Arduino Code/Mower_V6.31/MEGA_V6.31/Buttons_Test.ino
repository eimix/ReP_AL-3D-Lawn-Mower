//-----------------------------------------------------------------------------------------------------------------------------------
// BUTTONS TEST MENU

//-----------------------------------------------------------------------------------------------------------------------------------

void Activate_Menu_Option_Testing()
{
  if (Menu_Mode_Selection == 1)
  {
    // Perimeter Wire Test
    lcd.clear();
    lcd.print(F("Wire Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Perimeter Wire Test Started"));
#endif
    lawn_delay(5000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Test_Mower_Check_Wire();
      
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      
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

  if (Menu_Mode_Selection == 2)
  {
    // Relay Test
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Relay Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    lawn_delay(1000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Test_Relay();
      
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      
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

  if (Menu_Mode_Selection == 3)
  {
    // Mower Motor Test
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Wheel Motor Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    lawn_delay(1000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Perimeter_I = 1;                       // sets the itteration for the motor test
    Test_Wheel_Motors();                   // starts the mowe motor test 1x
  }

  if (Menu_Mode_Selection == 4)
  {
    // Blade Motor Test
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Blade Motor Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    lawn_delay(1000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Test_Mower_Blade_Motor();
  }

  if (Menu_Mode_Selection == 5)
  {
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Sonar Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    lawn_delay(1000);
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Test_Sonar_Array();
      
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      
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

  if (Menu_Mode_Selection == 6)
  {
    lcd.clear();
    lcd.print(F("Pattern Mow"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Slot 6 Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(3000);
    lcd.clear();
    Test_Compass_Turn_Function();
  }

  if (Menu_Mode_Selection == 7)
  {
    lcd.clear();
    lcd.print(F("Volt Amps Test"));
#if (DEBUG_LEVEL >= 2)
    Serial.println(F("Volts and Amps Test"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
#ifdef UseRemoteSensors
      Read_Serial1_Nano();                                                                    // Read the Serial data from the nano
#if (DEBUG_LEVEL >= 3)
      Serial.println("");
#endif
#endif
#ifdef UseLocalSensors
      Loop_Sensors();                                                                         // Read sensors data
#endif

/*
      lcd.setCursor(0, 0);
      lcd.print("V:");
      lcd.print(BatteryVoltage);
*/
      Print_LCD_Volt_Info();      
      
      lcd.setCursor(0, 1);
      lcd.print("CC:");
      lcd.print(ChargeCurrent, 2);
      
      lcd.setCursor(8, 1);
      lcd.print("LC:");
      lcd.print(LoadCurrent, 2);
      
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      
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

  if (Menu_Mode_Selection == 8)
  {
    lcd.clear();
    lcd.print(F("Compass Test"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Compass Test Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Heading:"));
    lcd.setCursor(0, 1);
    lcd.print(F("Degrees:"));
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      // insert Test Code Here
      Get_Compass_Reading();
#if (DEBUG_LEVEL >= 3)
      Serial.print(F("Comp H:"));
      Serial.print(Compass_Heading);
      Serial.print("|");
      Serial.println("");
#endif
      lcd.setCursor(9, 0);
      lcd.print(Compass_Heading);
      lcd.setCursor(9, 1);
      if (Compass_Heading_Degrees < 100)
        lcd.print(0);
      lcd.print(Compass_Heading_Degrees);
      
      Read_Membrane_Keys();
      lawn_delay(ButtonPressDelay);
      
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

  if (Menu_Mode_Selection == 9)
  {
    lcd.clear();
    lcd.print(F("Spare 9"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Slot 9 Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(3000);
    lcd.clear();
  }

  if (Menu_Mode_Selection == 10)
  {
    lcd.clear();
    lcd.print(F("Spare 10"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Slot 10 Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(3000);
    lcd.clear();
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------
