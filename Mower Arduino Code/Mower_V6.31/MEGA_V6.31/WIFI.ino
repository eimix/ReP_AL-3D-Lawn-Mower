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
//-----------------------------------------------------------------------------------------------------------------------------------
String Serial2_RX = "_";

void Get_WIFI_Commands()
{
  Receive_Data_From_NODEMCU(false);
  delay(5);
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
      case 'p': val_WIFI = Serial2_RX.toInt();
                if (ManualMode)
                {
                  delay(5); //Why this?
                  Execute_Manuel_Blynk_Command_To_Mower();
                }
                else
                {
                  delay(1000); //Why this?
                  Execute_Blynk_Command_To_Mower();
                }
                break;
    }

    //Always clear buffer if "end of packet" received
    Serial2_RX = "";
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Receive_WIFI_Manuel_Commands()
{
  Receive_Data_From_NODEMCU(true);  
  delay(5);
  Transmit_All_To_NODEMCU();
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Transmit_All_To_NODEMCU()
{
  delay(5);
  Serial2.print(BatteryVoltage);
  Serial2.println("g");
  delay(5);
  Serial2.print(Loop_Cycle_Mowing);
  Serial2.println("c");
  delay(5);
  Serial2.print(Mower_Docked);
  Serial2.println("d");
  delay(5);
  Serial2.print(Mower_Running);
  Serial2.println("z");
  delay(5);
  Serial2.print(Mower_Parked);
  Serial2.println("y");
  delay(5);
  Serial2.println(Mower_Charging);
  Serial2.println("o");
  delay(5);
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
      delay(500);
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
    delay(1000);
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

  // Manuel Button in Blynk App
  if (val_WIFI == 15)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
    Serial.print(F("WIFI Command: "));
    Serial.print(val_WIFI);
    Serial.println(F("|Manuel Mode"));
#endif
    Manouver_Manuel_Mode();
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

void Execute_Manuel_Blynk_Command_To_Mower()
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
    Serial.print("WIFI");
    Serial.print(val_WIFI);
    Serial.print(F("|Wheel Forward"));
#endif
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
    delay(300);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  if (val_WIFI == 18)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.print("WIFI:");
    Serial.print(val_WIFI);
    Serial.print(F("|Wheel Reverse"));
#endif
    SetPins_ToGoBackwards();
    Motor_Action_Go_Full_Speed();
    delay(300);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  if (val_WIFI == 19)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.print("WIFI:");
    Serial.print(val_WIFI);
    Serial.print(F("|Wheel Left"));
#endif
    SetPins_ToTurnLeft();
    Motor_Action_Go_Full_Speed();
    delay(200);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }

  if (val_WIFI == 20)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.print("WIFI:");
    Serial.print(val_WIFI);
    Serial.print(F("|Wheel Right"));
#endif
    SetPins_ToTurnRight();
    Motor_Action_Go_Full_Speed();
    delay(200);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // resets val2 to zero so the command is only executed once
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

#endif
