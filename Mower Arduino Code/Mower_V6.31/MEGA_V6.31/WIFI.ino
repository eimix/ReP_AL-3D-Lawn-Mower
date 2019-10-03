//-----------------------------------------------------------------------------------------------------------------------------------
// Blynk TX and RX functions to communicate with the MEGA and NODEMCU
// Each transmission has a label n or p or q or

// USE V5 Blynk  12th Feb 2019

//TODO:

//DONE:
//1. Serial communication with buffer - dont miss commands from NodeMCU (ESP8266)
//2. In manual mode - report data to ESP8266
//3. Skip real special EOL characters '\r' or '\n', and do not use escaped characters that are not escaped
//4. val_WIFI is not global, just in this file, and int is enougth

#ifdef UseWiFi
int val_WIFI;

#define ManualCommandDelay 300
unsigned long LastTX = 0;

//-----------------------------------------------------------------------------------------------------------------------------------
String Serial2_RX = "_";

void Get_WIFI_Commands()
{
  Receive_Data_From_NODEMCU(false);
  //delay(5); //WHY!?!
  Transmit_All_To_NODEMCU();
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Receive_Data_From_NODEMCU(bool ManualMode)
{
  //Using global variable NodeMCU_RX_Value as receive buffer

  while (Serial2.available() > 0)
  {
    char r = Serial2.read();

    //Ignore end of line characters (received if other side does "println")
    if (r == '\n' || r == '\r')
      continue;

    //If any other (not special character) received - add it to buffer
    if (r < 'a' || r > 'z')
    {
      Serial2_RX = Serial2_RX + (char)r;
      continue;
    }

    //Ignore first "packet", it is not sure that it is received from begining,
    //only reading first "end of packet" byte confirms that new "packet" will start
    if (Serial2_RX[0] == '_')
    {
      Serial2_RX = "";
      continue;
    }

    switch (r)
    {
      case 'p':
//        {
          val_WIFI = Serial2_RX.toInt();
          if (ManualMode)
          {
            delay(5); //Why this?
            Execute_Manual_Blynk_Command_To_Mower();
          }
          else
          {
            lawn_delay(1000); //Why this? -
            //sthis makes delay to enabling Manual mode
            Execute_Blynk_Command_To_Mower();
          }
          break;
//        }
      case 'j':  //Incoming Joystick command Code|JoystickX|JoystickY
        {
          val_WIFI      = Serial2_RX.substring(0, Serial2_RX.indexOf("|")).toInt();
          int JoystickX = Serial2_RX.substring(Serial2_RX.indexOf("|") + 1, Serial2_RX.lastIndexOf("|")).toInt(); 
          int JoystickY = Serial2_RX.substring(Serial2_RX.lastIndexOf("|") + 1).toInt(); 

#if (DEBUG_LEVEL >= 4)
          Serial.println();
          Serial.print("WiFi:");
          Serial.print(val_WIFI);
          Serial.print("|");
          Serial.print(JoystickX);
          Serial.print("|");
          Serial.print(JoystickY);
          Serial.println();
#endif

          Execute_Manual_Joystick_Blynk_Command_To_Mower(val_WIFI, JoystickX, JoystickY);

          break;
        }
    }
    //Always clear buffer if "end of packet" received
    Serial2_RX = "";
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Receive_WIFI_Manual_Commands()
{
  Receive_Data_From_NODEMCU(true);
  Transmit_All_To_NODEMCU();
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Transmit_All_To_NODEMCU()
{
  //Send "driver" controls data sending in 250ms intervals
  if (calcTimeDiff(LastTX, millis()) < 250)
    return;
  LastTX = millis();

  Serial2.print(BatteryVoltage, 2);            //2 decimal digits
  Serial2.println("g");
  Serial2.print(LoadCurrent, 2);               //2 decimal digits
  Serial2.println("a");
  Serial2.print(ChargeCurrent, 2);             //2 decimal digits
  Serial2.println("b");

  Serial2.print(Loop_Cycle_Mowing);
  Serial2.println("c");
  Serial2.print(Mower_Docked);
  Serial2.println("d");
  Serial2.print(Mower_Running);
  Serial2.println("z");
  Serial2.print(Mower_Parked);
  Serial2.println("y");
  Serial2.println(Mower_Charging);
  Serial2.println("o");
  Serial2.println(Tracking_Wire);
  Serial2.println("m");

  //*************

  //Serial2.print(Mower_Parked_Low_Batt);
  //Serial2.println("b");

  //Serial2.print(Mower_Error);
  //Serial2.println("l");

  //Serial2.println(Compass_Heading_Locked);
  //Serial2.println("w");
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Execute_Blynk_Command_To_Mower()
{
  // Updates the Serial Monitor with the latest Blynk Commands and can be used to start
  // functions on the mower when the command is recieved.
  delay(30);

  // Exit Dock to Zone 1
  if (val_WIFI == 14)
  {
    if (Mower_Docked == 1)
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println("");
      Serial.print(F("WIFI Command: "));
      Serial.print(val_WIFI);
      Serial.print(F("Exit Dock| "));
      Serial.println(F("Zone 1"));
#endif
      lcd.clear();
      lcd.print(F("WIFI Start"));
      lcd.setCursor(0, 1);
      lcd.print(F("Exit Dock Z1"));
      lawn_delay(500);
      lcd.clear();
      Exit_Zone = 1;
      Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
      Manouver_Exit_To_Zone_X();
    }
  }

  // Quick Start Button in Blynk App
  if (val_WIFI == 13)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print(F("WIFI Command:"));
    Serial.print(val_WIFI);
    Serial.println(F("|Quick Start"));
#endif
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("WIFI Start"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Quick Start"));
#endif
    if (Mower_Docked == 0)
    {
      Manouver_Start_Mower();
      lcd.clear();
    }
    else
    {
#if (DEBUG_LEVEL >= 2)
      Serial.println(F("Mower Docked - Quick Start not possible"));
#endif
    }
  }

  // Go To Dock Button in Blynk App
  if (val_WIFI == 12)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print(F("WIFI Command: "));
    Serial.print(val_WIFI);
    Serial.println(F("|Go To Dock"));
#endif
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("WIFI Go To Dock"));
    Menu_Mode_Selection = 0;                                      // Releases the loop in the membrane button section.
    lawn_delay(1000);
    lcd.clear();

    Manouver_Go_To_Charging_Station();
  }

  // STOP / Cancel Button in Blynk App
  if (val_WIFI == 11)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print(F("WIFI Command: "));
    Serial.print(val_WIFI);
    Serial.println(F("|Pause/Stop"));
#endif
    Manouver_Park_The_Mower();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  // Manual Button in Blynk App
  if (val_WIFI == 15)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print(F("WIFI Command: "));
    Serial.print(val_WIFI);
    Serial.println(F("|Manual Mode"));
#endif
    Manouver_Manual_Mode();
    Turn_On_Relay();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  // Automatic Button in Blynk App
  if (val_WIFI == 16)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print(F("WIFI Command: "));
    Serial.print(val_WIFI);
    Serial.println(F("|Automatic Mode"));
#endif
    Manouver_Park_The_Mower();
    Turn_On_Relay();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Execute_Manual_Blynk_Command_To_Mower()
{
  // insert wheel motions here.
  if (val_WIFI == 16)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print(F("WIFI Command: "));
    Serial.print(val_WIFI);
    Serial.println(F("|Automatic Mode"));
#endif
    Manouver_Park_The_Mower();
    Turn_On_Relay();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  if (val_WIFI == 17)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print("WIFI");
    Serial.print(val_WIFI);
    Serial.println(F("|Wheel Forward"));
#endif
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
    lawn_delay(ManualCommandDelay);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  if (val_WIFI == 18)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print("WIFI:");
    Serial.print(val_WIFI);
    Serial.println(F("|Wheel Reverse"));
#endif
    SetPins_ToGoBackwards();
    Motor_Action_Go_Full_Speed();
    lawn_delay(ManualCommandDelay);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  if (val_WIFI == 19)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print("WIFI:");
    Serial.print(val_WIFI);
    Serial.println(F("|Wheel Left"));
#endif
    SetPins_ToTurnLeft();
    Motor_Action_Go_Full_Speed();
    lawn_delay(ManualCommandDelay);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  if (val_WIFI == 20)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");

    Serial.print("WIFI:");
    Serial.print(val_WIFI);
    Serial.println(F("|Wheel Right"));
#endif
    SetPins_ToTurnRight();
    Motor_Action_Go_Full_Speed();
    lawn_delay(ManualCommandDelay);
    Motor_Action_Stop_Motors();
    
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Execute_Manual_Joystick_Blynk_Command_To_Mower(byte Code, int JoystickX, int JoystickY)
{
  if (Code == 21)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");

    Serial.print("WIFI:");
    Serial.print(val_WIFI);
    Serial.println(F("|Joystick stearing"));
#endif 
    Motor_Action_Joystick_Steering(JoystickX - 256, JoystickY - 256);
    lawn_delay(ManualCommandDelay);
    Motor_Action_Stop_Motors();

    val_WIFI = 0;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

#endif
