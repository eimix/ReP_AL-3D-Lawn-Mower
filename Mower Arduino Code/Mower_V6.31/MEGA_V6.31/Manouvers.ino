// Manouvers are a set of motion functions or motor actions that are regulary called
// e.g. Turn the mower around

//TODO:
//1. One manouver should not call other manouver (ex.: Manouver_Mow_The_Grass -> Manouver_Turn_Around)
//recursive call could happen, and states/errors/notes should be used - "State: Mowing; Error: max cycle length reached"

//---------------------------------------------------------------------------------------

// Moves the Mower Forwards in the garden and activates compass features if enabled
void Manouver_Mow_The_Grass()
{
  Motor_Action_Spin_Blades();
  SetPins_ToGoForwards();

  Loop_Cycle_Mowing = (Loop_Cycle_Mowing + 1);
  Serial.print(F("Loop:"));
  Serial.print(Loop_Cycle_Mowing);
  Serial.print("|");
  lcd.setCursor(13, 1);
  lcd.print(Loop_Cycle_Mowing);
  delay(1);

  if (Loop_Cycle_Mowing < 5)
  {
    Serial.print(F("C-Lock:OFF"));
    Serial.print("|");
    Print_LCD_Mowing();
    if (Compass_Activate == 1)
      Get_Compass_Reading();
    Motor_Action_Go_Full_Speed();
    Compass_Heading_Locked = 0;                           // Turn off the compass heading lock for the new cycles
  }

  if (Loop_Cycle_Mowing == 5)
  {
    if (Compass_Heading_Hold_Enabled == 0)
    {
      Serial.print(F("C-Lock:OFF"));
      Serial.print("|");
      Print_LCD_Compass_Mowing();
      Motor_Action_Go_Full_Speed();
      Compass_Heading_Locked = 0;
    }
    if ((Compass_Heading_Hold_Enabled == 1) && (Compass_Activate == 1))
    {
      Get_Compass_Reading();                                                      // Gets the latest compass reading
      if (Pattern_Mow == 1)  Heading_Lock = Compass_Target;                       // If pattern mow is enabled follow that heading
      if (Pattern_Mow == 0)  Heading_Lock = Compass_Heading_Degrees;              // Otherwise use the current heading as the lock
      Compass_Heading_Locked = 1;                                                 // One more cycle of normal movement
    }
  }

  if (Loop_Cycle_Mowing > 5)
  {
    if (Compass_Heading_Locked == 0)
    {
      lcd.setCursor(0, 1);
      lcd.print("Mowing..        ");
      Serial.print(F("C-Lock:OFF"));
      Serial.print("|");
      Motor_Action_Go_Full_Speed();
    }
    if ((Compass_Heading_Hold_Enabled == 1) && (Compass_Activate == 1)) {            // if the Mower is tracking using the compass steer here
      Get_Compass_Reading();
      Calculate_Compass_Wheel_Compensation();
      Motor_Action_Dynamic_PWM_Steering();              // Removes the full speed function if the mower is trying to hold to the compass heading.
      Print_LCD_Compass_Mowing();
      Serial.print(F("C-Lock:ON_"));
      Serial.print("|");
    }
  }

  if (Loop_Cycle_Mowing > Max_Cycles) {                    // 150 the max length for my garden. Adjust accordingly
    Serial.println("");
    Serial.println("Loop Cycle at Max");
    Serial.println("");
    Motor_Action_Stop_Spin_Blades();                    // Stop the blades from spinning
    Manouver_Turn_Around();                             // Turn around the mower
    Loop_Cycle_Mowing = 0;                              // Restes the loop cycle to start again.
    lcd.clear();
  }
}
//---------------------------------------------------------------------------------------

void Manouver_Find_Wire_Track()
{
  Serial.println(F("Find Wire Track Function Activated"));
  lcd.clear();
  lcd.print("Finding Wire...  ");
  Motor_Action_Stop_Spin_Blades();
  delay(5);
  Abort_Wire_Find = 0;
  No_Wire_Found = 0;
  TestforBoundaryWire();                                                                    // Check to see that the wire is on.


  for (int i = 0; i <= 1; i++)
  {
    if (Mower_Parked == 0)
    {
#ifdef UseWiFi
      Get_WIFI_Commands();
#endif
      Serial.print(F("Position Try = "));
      Serial.println(i);
      ADCMan.run();
      UpdateWireSensor();
      delay(20);
      ADCMan.run();
      UpdateWireSensor();
      delay(20);
      Serial.println(F("before if loops start"));
      PrintBoundaryWireStatus();

      // First go backwards if the mower is outside the wire
      if (( inside == false) && (Abort_Wire_Find == 0) ) {                                    // If the mower is outside the wire then run the following code.
        ADCMan.run();
        UpdateWireSensor();
        PrintBoundaryWireStatus();
        Motor_Action_Stop_Motors();                                                           // Stop all wheel motion
        delay(1000);
        SetPins_ToGoBackwards();                                                              // Set the mower to back up
        delay(100);
        lcd.clear();
        lcd.print("Backwards Try...  ");
        lcd.setCursor(0, 1);
        lcd.print("Finding Wire  ");
        delay(100);
        while (( inside != true) && (Abort_Wire_Find == 0) ) {                                // While the mower is still outside the fence run this code
          Motor_Action_Go_Full_Speed();                                                       // Go full speed (in this case backwards)
          UpdateWireSensor();                                                                 // Read the wire sensor and see of the mower is now  or outside the wire
          ADCMan.run();
          PrintBoundaryWireStatus();                                                          // Prints of the status of the wire sensor readings.
          Serial.println("");
#ifdef UseWiFi
          if (Manuel_Mode == 0) Get_WIFI_Commands();
#endif
          if (Mower_Parked == 1) {
            Serial.println("Abort Wire Find");
            Abort_Wire_Find = 1;
          }
        }

        Motor_Action_Stop_Motors();
        delay(5);
      }

      // Code to go forwards until the mower is outside/ON the wire
      if (( inside == true) && (Abort_Wire_Find == 0) && (No_Wire_Found == 0) ) {             // If the Mower is situated  the wire then run the following code.
        ADCMan.run();
        UpdateWireSensor();
        Serial.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
        PrintBoundaryWireStatus();
        Motor_Action_Stop_Motors();
        delay(1000);
        SetPins_ToGoForwards();                                                             // Set the motors to move the mower forwards
        delay(100);
        lcd.clear();
        lcd.print("Forward Try...  ");
        lcd.setCursor(0, 1);
        lcd.print("Finding Wire  ");
        delay(100);
        int cycle = 0;                                                                      // resets the cycles
        while (( inside != false) && (No_Wire_Found == 0) && (Mower_Parked == 0) ) {                              // Move the mower forward until mower is outisde/ON the wire fence or 500 cycles have passed
          cycle = cycle + 1;
          lcd.setCursor(0, 1);
          lcd.print("Track -> Charge");
          Motor_Action_Go_Full_Speed();                                                     // Go full speed (in this case forwards)
          UpdateWireSensor();                                                               // Read the wire sensor and see of the mower is now  or outside the wire
          ADCMan.run();
          PrintBoundaryWireStatus();                                                        // Prints of the status of the wire sensor readings.
          Serial.println("");
#ifdef UseWiFi
          if (Manuel_Mode == 0)
            Get_WIFI_Commands();
#endif
          if (Mower_Parked == 1) {
            Serial.println("Abort Wire Find");
            Abort_Wire_Find = 1;
          }
          if (cycle > Max_Cycle_Wire_Find) {                                                                // Track forwards for 500 cycles
            No_Wire_Found = 1;                                                              // if mower is still tracking after 500 cycles then cancel the find.
            Serial.println("Max Forward Cycles reached");
          }
        }
        Motor_Action_Stop_Motors();
        delay(5);
      }
      Motor_Action_Stop_Motors();
      delay(1000);
    }
  }

  // Position the mower further over the wire so it has space to turn 90° onto the wire.
  if ( (Abort_Wire_Find == 0) && (No_Wire_Found == 0) && (Mower_Parked == 0) ) {
    SetPins_ToGoForwards();
    delay(100);
    Motor_Action_Go_Full_Speed();
    delay(500);
    Motor_Action_Stop_Motors();
  }

  // Sets the firection of spin depensing on if the mower is eciting or tracking home

  // Set pins to track home to charge.
  if ((Mower_Track_To_Charge == 1) && (Mower_Parked == 0)) {
    lcd.setCursor(0, 1);
    lcd.print("Track -> Charge");
    delay(1000);
    if (CW_Tracking_To_Charge == 1) {
      SetPins_ToTurnRight();                                                                // Track perimeter wire in a Clockwise Direction to the charging station
      Serial.println(F("CW Tracking to Charger"));
    }
    if (CCW_Tracking_To_Charge == 1) {
      SetPins_ToTurnLeft();
      Serial.println(F("CCW Tracking toCharger"));
    }
  }

  // Set pins to track to exit.
  if ((Mower_Track_To_Exit == 1) && (Mower_Parked == 0)) {
    lcd.setCursor(0, 1);
    lcd.print("Track -> Exit");
    delay(1000);
    if (CW_Tracking_To_Start == 1) {
      SetPins_ToTurnRight();                                                                // Track perimeter wire in a Clockwise Direction to the charging station
      Serial.println(F("CW Tracking to Exit"));
    }
    if (CCW_Tracking_To_Start == 1) {
      SetPins_ToTurnLeft();
      Serial.println(F("CCW Tracking to Exit"));
    }
  }


  delay(20);
  // Pins are now set to turn from the above logic which way to turn onto the wire

  // Update the mowers position to the wire.
  ADCMan.run();
  UpdateWireSensor();
  ADCMan.run();
  PrintBoundaryWireStatus();                                                                  // Prints of the status of the wire sensor readings
  delay(20);


  // Spins the mower over the wire in the driection of tracking
  while (( inside == false)  && (Abort_Wire_Find == 0) && (No_Wire_Found == 0) && (Mower_Parked == 0) ) {
    while ( inside != true) {                                                             // Do this loop until mower is back  the wire fence
      Motor_Action_Go_Full_Speed();                                                         // Go full speed (in the case turning as set by the previous logic)
      UpdateWireSensor();                                                                   // Read the wire sensor and see of the mower is now  or outside the wire
      ADCMan.run();
      PrintBoundaryWireStatus();                                                            // Prints of the status of the wire sensor readings.
    }
    Motor_Action_Stop_Motors();                                                                 // Stop the mower on the wire facing the correct direction.
  }

  Motor_Action_Stop_Motors();
  if (Abort_Wire_Find == 0) {
    Serial.println(F("Track Wire Function Complete - ON WIRE??"));
    lcd.clear();
    lcd.print("Wire Found...");
    delay(2000);                                                                                // 2 second pause to check result
    SetPins_ToGoForwards();
    delay(100);
  }

  if (Abort_Wire_Find == 1) {
    Serial.println("Wire Find Aborted");
    Abort_Wire_Find = 0;
    SetPins_ToGoForwards();
    Manouver_Park_The_Mower();
  }

  if (No_Wire_Found == 1) {
    Serial.println("Re-starting wire find");
    SetPins_ToGoForwards();
  }

}
//---------------------------------------------------------------------------------------

void Manouver_Turn_Around()
{
  if (Pattern_Mow == 0)
  {
    Motor_Action_Stop_Motors();
    delay(500);
    SetPins_ToGoBackwards();
    Motor_Action_Go_Full_Speed();
    delay(Mower_Reverse_Delay);
    Motor_Action_Stop_Motors();

    if ((Loop_Cycle_Mowing % 2) == 0)
      SetPins_ToTurnRight();
    else
      SetPins_ToTurnLeft();

    Motor_Action_Turn_Speed();
    delay (random(Mower_Turn_Delay_Min, Mower_Turn_Delay_Max));
  }

  if ((Pattern_Mow == 1) && (Compass_Activate == 1))
  {
    Motor_Action_Stop_Motors();
    Motor_Action_Stop_Spin_Blades();
    delay(500);
    SetPins_ToGoBackwards();
    Motor_Action_Go_Full_Speed();
    delay(1000);
    Motor_Action_Stop_Motors();
    delay(500);

    if (Compass_Leg == 0)
    {
      SetPins_ToTurnRight();
      Motor_Action_Go_Full_Speed();
      delay(200);
      Get_Compass_Reading();
      delay(200);
      Get_Compass_Reading();
      // Turn the Mower 90° to the corner
      Serial.println("");
      Serial.print("Compas_Leg = 0 | ");
      Serial.print("Turning 90°|");
      Compass_Target = Compass_Mow_Direction + 90;
      if (Compass_Target > 360) (Compass_Target = Compass_Target - 360);
      Serial.print("|Target Degree Heading = ");
      Serial.print(Compass_Target);
      Serial.print("|");
      Turn_To_Compass_Heading();

      // Move Forwards at 90° and stop
      SetPins_ToGoForwards();
      Motor_Action_Go_Full_Speed();
      delay(1500);
      Motor_Action_Stop_Motors();
      SetPins_ToTurnRight();
      Motor_Action_Go_Full_Speed();
      delay(200);
      Get_Compass_Reading();
      delay(200);
      Get_Compass_Reading();

      // Turn 180° to the initial mowing direction.
      Serial.println("");
      Serial.print("|Return Leg 1°");
      Compass_Target = Compass_Mow_Direction;
      if (Compass_Target > 360) (Compass_Target = Compass_Target - 360);
      Serial.print("|Target Degree Heading = ");
      Serial.print(Compass_Target);
      Serial.println("|");
      Turn_To_Compass_Heading();
    }

    if (Compass_Leg == 1)
    {
      // Turn the Mower 90° to the corner
      Serial.println("");
      Serial.print("Compass_Leg = 1|");
      Serial.print("Turning 90°");
      SetPins_ToTurnLeft();
      Motor_Action_Go_Full_Speed();
      delay(200);
      Get_Compass_Reading();
      delay(200);
      Get_Compass_Reading();
      Compass_Target = Compass_Mow_Direction + 90;
      if (Compass_Target < 0) (Compass_Target = 360 + Compass_Target);
      Serial.print("|Target Degree Heading = ");
      Serial.print(Compass_Target);
      Turn_To_Compass_Heading();

      // Move Forwards at 90° and stop
      SetPins_ToGoForwards();
      Motor_Action_Go_Full_Speed();
      delay(1500);
      Motor_Action_Stop_Motors();
      SetPins_ToTurnLeft;
      Motor_Action_Go_Full_Speed();
      delay(200);
      Get_Compass_Reading();
      delay(200);
      Get_Compass_Reading();

      // Turn 180° to the initial mowing direction.
      Serial.println("");
      Serial.print("|Return Leg 0°");
      Compass_Target = Compass_Mow_Direction + 180;
      if (Compass_Target > 360) (Compass_Target = Compass_Target - 360);
      Serial.print("|Target Degree Heading = ");
      Serial.print(Compass_Target);
      Serial.println("|");
      Turn_To_Compass_Heading();
    }

    //Set the compass leg to the next stage.
    Compass_Leg = Compass_Leg + 1;
    if (Compass_Leg > 1) Compass_Leg = 0;

  }

  Motor_Action_Stop_Motors();
  TestforBoundaryWire();
  Check_Wire_In_Out();
  delay(200);
  TestforBoundaryWire();
  Check_Wire_In_Out();
  Loop_Cycle_Mowing = 1;
  Sonar_Hit = 0;
  Compass_Heading_Locked = 0;
  lcd.clear();
}
//---------------------------------------------------------------------------------------

void Manouver_Turn_Around_Sonar()
{
  Motor_Action_Stop_Motors();
  delay(500);
  SetPins_ToGoBackwards();
  Motor_Action_Go_Full_Speed();
  delay(Mower_Reverse_Delay);
  Motor_Action_Stop_Motors();

  if (distance1 < maxdistancesonar || distance2 < maxdistancesonar)
  {
    lcd.setCursor(0, 8);
    lcd.print("Go Right -->       ");
    SetPins_ToTurnRight();
    Motor_Action_Turn_Speed();
    delay(Mower_Turn_Delay_Max);
  }

  if (distance3 < maxdistancesonar)
  {
    lcd.setCursor(0, 8);
    lcd.print("<-- Go Left      ");
    SetPins_ToTurnLeft();
    Motor_Action_Turn_Speed();
    delay(Mower_Turn_Delay_Max);
  }

  Motor_Action_Stop_Motors();
  Compass_Heading_Locked = 0;
  Sonar_Hit = 0;
  Loop_Cycle_Mowing = 0;
}
//---------------------------------------------------------------------------------------

void Manouver_Manuel_Mode()
{
  Mower_Docked          = 0;
  Mower_Parked          = 0;
  Mower_Running         = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Exit   = 0;
  Mower_Track_To_Charge = 0;
  Mower_Error           = 0;
  Mower_SaftyWarning    = 0;
  Manuel_Mode           = 1;
  Loop_Cycle_Mowing     = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
}
//---------------------------------------------------------------------------------------

void Manouver_Start_Mower()
{
  Mower_Docked          = 0;
  Mower_Parked          = 0;
  Mower_Running         = 1;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 0;
  Rain_Hit_Detected     = 0;
  Mower_Error           = 0;
  Mower_SaftyWarning    = 0;
  Loop_Cycle_Mowing     = 0;
  Manuel_Mode           = 0;
  Turn_On_Relay();
}
//---------------------------------------------------------------------------------------

void Manouver_Mower_Exit_Dock()
{
  Mower_Docked          = 0;
  Mower_Parked          = 0;
  Mower_Running         = 0;
  Mower_Parked_Low_Batt = 0;
  Rain_Hit_Detected     = 0;
  Mower_Error           = 0;
  Mower_SaftyWarning    = 0;
  Manuel_Mode           = 0;
  Tracking_Wire         = 0;
#ifdef UseWiFi
  Get_WIFI_Commands();
#endif
}
//---------------------------------------------------------------------------------------

void Manouver_Dock_The_Mower()
{
  Mower_Docked          = 1;
  Mower_Parked          = 0;
  Mower_Running         = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Exit   = 0;
  Mower_Track_To_Charge = 0;
  Mower_Error           = 0;
  Mower_SaftyWarning    = 0;
  Loop_Cycle_Mowing     = 0;
  Manuel_Mode           = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
  Turn_Off_Relay();
  Print_LCD_Info_Docked();
  Mower_Charging        = 0;

  //Setup Alarms
  Alarm_Timed_Mow_ON = 0;                                           // Turns off the 1 hr Alarm
}
//---------------------------------------------------------------------------------------

// Mower is a parked position and needs manual charging
void Manouver_Park_The_Mower_Low_Batt()
{
  Mower_Docked          = 0;
  Mower_Parked          = 0;
  Mower_Running         = 0;
  Mower_Parked_Low_Batt = 1;
  Mower_Track_To_Charge = 0;
  Mower_Error           = 0;
  Mower_SaftyWarning    = 0;
  Loop_Cycle_Mowing     = 0;
  Manuel_Mode           = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
}
//---------------------------------------------------------------------------------------

// Mower is in a parked or paused potion ready to restart
void Manouver_Park_The_Mower()
{
  if (Mower_Parked == 0)
    lcd.clear();
  Mower_Docked          = 0;
  Mower_Parked          = 1;
  Mower_Running         = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 0;
  Tracking_Wire         = 0;
  Mower_Track_To_Exit   = 0;
  Mower_Error           = 0;
  Mower_SaftyWarning    = 0;
  Loop_Cycle_Mowing     = 0;
  Manuel_Mode           = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
  Turn_Off_Relay();

  Alarm_Timed_Mow_ON = 0;                                           // Turns off the 1 hr Alarm
  //if (Alarm_1_Repeat == 0) Alarm_1_ON = 0;
  //if (Alarm_2_Repeat == 0) Alarm_2_ON = 0;
  //if (Alarm_3_Repeat == 0) Alarm_3_ON = 0;
}
//---------------------------------------------------------------------------------------

void Manouver_Hibernate_Mower()
{
  Mower_Docked          = 0;
  Mower_Parked          = 0;
  Mower_Running         = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 0;
  Tracking_Wire         = 0;
  Mower_Track_To_Exit   = 0;
  Mower_Error           = 1;
  Mower_SaftyWarning    = 0;
  Loop_Cycle_Mowing     = 0;
  Manuel_Mode           = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
  Turn_Off_Relay();
}
//---------------------------------------------------------------------------------------

// Mower is sent to the charging station after low volts are detected or mebrane key input.
void Manouver_Go_To_Charging_Station()
{
  Mower_Docked          = 0;
  Mower_Parked          = 0;
  Mower_Running         = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 1;
  Tracking_Wire         = 1;
  Mower_Track_To_Exit   = 0;
  Mower_Error           = 0;
  Mower_SaftyWarning    = 0;
  Loop_Cycle_Mowing     = 0;
  Manuel_Mode           = 0;
  No_Wire_Found         = 0;
  Manage_Alarms();                                              // Switches on or off the Alarms depending on the setup
#ifdef UseWiFi
  Get_WIFI_Commands();
#endif
  delay(5);
  Motor_Action_Stop_Spin_Blades();
  Motor_Action_Stop_Motors();
  delay(2000);
  Turn_On_Relay();
  delay(500);
#ifdef UseWiFi
  Get_WIFI_Commands();
#endif
  if ((Compass_Activate == 1) && (Mower_Parked == 0))    Compass_Turn_Mower_To_Home_Direction();
#ifdef UseWiFi
  Get_WIFI_Commands();
#endif
  if (Mower_Parked == 0)                                Manouver_Find_Wire_Track();
#ifdef UseWiFi
  Get_WIFI_Commands();
#endif
  if ((Mower_Parked == 0) && (No_Wire_Found == 0))      Track_Perimeter_Wire_To_Dock();
#ifdef UseWiFi
  Get_WIFI_Commands();
#endif
  if (No_Wire_Found == 1)                               Manouver_Go_To_Charging_Station();

#ifdef UseWiFi
  Get_WIFI_Commands();
#endif
}
//---------------------------------------------------------------------------------------

void Manouver_Exit_To_Zone_X()
{
  // Zone 1 or Zone 2 and the Wire itterations are set on the Membrane Buttons.
  // These values are then crried into the following functions.
  Turn_On_Relay();
  delay(1000);
  Manouver_Mower_Exit_Dock();
  Special_Exit_From_Docking_Station();                         // Move the Mower into position backing out of the docking station
  if (Perimeter_Wire_Enabled == 1) {
    Mower_Track_To_Exit = 1;
    TestforBoundaryWire();
    delay(50);
    if (Wire_Detected == 1)
    {
#ifdef UseWiFi
      Get_WIFI_Commands();
#endif
      Manouver_Find_Wire_Track();                                   // Located the boundary wire
#ifdef UseWiFi
      Get_WIFI_Commands();
#endif
      if (Mower_Parked == 0) Track_Wire_From_Dock_to_Zone_X();
#ifdef UseWiFi
      Get_WIFI_Commands();
#endif
      if (Mower_Parked == 0) Special_Move_Into_Garden_Zone_X();
#ifdef UseWiFi
      Get_WIFI_Commands();
#endif
      if (Mower_Parked == 0) Manouver_Start_Mower();
#ifdef UseWiFi
      Get_WIFI_Commands();
#endif
      if (Mower_Parked == 1) Manouver_Park_The_Mower();
    }
    if (Wire_Detected == 0) {
      TestforBoundaryWire();                                      // Test again for the boundary wire
      if (Wire_Detected == 0) {                                 // if its still saying the wire is off then park the mower.
        Serial.println("");
        Serial.println("Perimeter Wire not detected");
        Manouver_Park_The_Mower();
      }
    }
  }
  if (Perimeter_Wire_Enabled == 0) {
    Serial.println("");
    Serial.println("Perimeter Wire not activated in settings");
    Manouver_Park_The_Mower();
  }
}
//---------------------------------------------------------------------------------------

// Function to re-find the wire if the mower looses the wire while mowing
// 3 outside the wires ativates this function. Sonar and wire function is then used to re-find the wire.
void Manouver_Outside_Wire_ReFind_Function()
{
  Motor_Action_Stop_Spin_Blades();
  lcd.clear();
  lcd.print("Trying to find");
  lcd.setCursor(0, 1);
  lcd.print("Wire Again...");
  ADCMan.run();
  UpdateWireSensor();
  delay(20);
  PrintBoundaryWireStatus();
  ADCMan.run();
  UpdateWireSensor();
  delay(20);
  PrintBoundaryWireStatus();
  while (inside == false)                                               // If the mower is outside the wire then run the following code.
  {
    ADCMan.run();
    UpdateWireSensor();
    PrintBoundaryWireStatus();
    //Check_Wire_In_Out();
    delay(500);
    distance_blockage = PingSonarX(trigPin1, echoPin1, 1, 0, 4, false);
    delay(500);
    Serial.print("Distance measured from sonar :");
    Serial.println(distance_blockage);

    // if the sonar is measuring an opening as the distance is greater than 300cm then move forward in that direction.
    if (distance_blockage > 400)
    {
      while ((inside == false) && (distance_blockage > 400))
      {
        SetPins_ToGoForwards();
        Motor_Action_Go_Full_Speed();
        delay(500);
        ADCMan.run();
        UpdateWireSensor();
        PrintBoundaryWireStatus();
        distance_blockage = PingSonarX(trigPin1, echoPin1, 1, 0, 4, false);
        delay(10);
        //Check_Wire_In_Out();
      }
      Motor_Action_Stop_Motors;
      ADCMan.run();
      UpdateWireSensor();
      PrintBoundaryWireStatus();
    }

    // if the sonar is measuring something less than 300cm then turn to the left and measure again
    // keep turning 'while function' until a path is open.
    while (distance_blockage < 300)
    {
      SetPins_ToTurnLeft();
      delay(200);
      Motor_Action_Turn_Speed();
      delay(100);
      distance_blockage = PingSonarX(trigPin1, echoPin1, 1, 0, 4, false);
      delay(10);
    }
    Motor_Action_Stop_Motors;
    ADCMan.run();
    UpdateWireSensor();
    PrintBoundaryWireStatus();
  }

  Serial.println("Mower is now back inside the wire......?");
  lcd.clear();
  lcd.print("Mower now");
  lcd.setCursor(0, 1);
  lcd.print("Inside Wire?");
  Outside_Wire_Count = 0;
  //FindWireTrack();
}
