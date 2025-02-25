//-----------------------------------------------------------------------------------------------------------------------------------

void Special_Cut_Under_Trampoline_Function()
{
  //Enter Code Here
}
//-----------------------------------------------------------------------------------------------------------------------------------

// after wire tracking the code to bering the mower to a sensible position away from the wire
// This poart of the code should eb modified to fit your garden needs
void Special_Move_Into_Garden_Zone_X()
{
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("start mower code here"));
#endif
  Motor_Action_Stop_Motors();                                     // Stop the wheel motors
  SetPins_ToTurnLeft();                                           // Turn left
  Motor_Action_Go_Full_Speed();
  lawn_delay(1400);                                                     // Turn left for 0.7seconds
  Motor_Action_Stop_Motors();                                     // Stop the wheel motors.
  SetPins_ToGoForwards();                                         // Get ready to move off
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Special_Exit_From_Docking_Station()
{
  lcd.clear();
  lcd.print(F("Exiting Garage"));
  lawn_delay(2000);
  SetPins_ToGoBackwards();                                        // Prepare motors pins to go Backwards
  Motor_Action_GoFullSpeed_Out_Garage();                          // Turn the wheels
#if (DEBUG_LEVEL >= 3)
  Serial.print(F("Left Wheel PWM:"));
  Serial.print(PWM_MaxSpeed_LH);
  Serial.print("|");
  Serial.print(F("Right Wheel PWM:"));
  Serial.println(PWM_MaxSpeed_RH);
#endif
  lawn_delay(4000);                                                   // Backwards time
  Motor_Action_Stop_Motors;                                       // Stop
  SetPins_ToTurnLeft();                                           // Prepare motors to turn left
  Motor_Action_Turn_Speed();                                      // Turn the wheels
  lawn_delay(500);                                                     // Turn time
  Motor_Action_Stop_Motors;                                       // Stop
  SetPins_ToGoBackwards();                                        // Set again to go backwards
  Motor_Action_Go_Full_Speed();                                   // Turn the wheels
  lawn_delay(500);                                                    // Backwards Time
  SetPins_ToTurnLeft();                                           // Set to go left
  Motor_Action_Turn_Speed();                                      // Turn the wheels
  lawn_delay(200);                                                     // Turning time
  Motor_Action_Stop_Motors();                                     // Stop
  SetPins_ToGoForwards();                                         // Set to go wheel motor pins to go forwards
  Motor_Action_Stop_Motors();                                     // Stop / Park the mower here
  lcd.clear();                                                    // Clears the LCD display
  lcd.print(F("Garage Clear"));                                      // Prints to the LCD screen
  lawn_delay(500);
  lcd.clear();
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Specials_Find_Wire_Track()
{
#if (DEBUG_LEVEL >= 2)
  Serial.println("");
  Serial.println(F("Lost Mower - find wire Track"));
#endif
  lcd.clear();
  lcd.print(F("Finding Wire...  "));
  Motor_Action_Stop_Spin_Blades();
  delay(5);
  Abort_Wire_Find   = 0;
//6.4  No_Wire_Found_Fwd = 0;
  TestforBoundaryWire();                                                                    // Check to see that the wire is on.

  for (int i = 0; i <= 1; i++)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Position Try = "));
    Serial.println(i);
#endif
    ADCMan.run();
    UpdateWireSensor();
    delay(20);
    ADCMan.run();
    UpdateWireSensor();
    delay(20);
    PrintBoundaryWireStatus();

    //No_Wire_Found = 0;
    Wire_Find_Attempt = 0;  

    // First go backwards if the mower is outside the wire
    if (inside == false)                                     // If the mower is outside the wire then run the following code.
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println("Reversing to find the wire");
#endif      
      ADCMan.run();
      UpdateWireSensor();
      PrintBoundaryWireStatus();
      Motor_Action_Stop_Motors();                                                           // Stop all wheel motion
      lawn_delay(1000);
      SetPins_ToGoBackwards();                                                              // Set the mower to back up
      lawn_delay(100);
      lcd.clear();
      lcd.print(F("Backwards Try...  "));
      lcd.setCursor(0, 1);
      lcd.print(F("Finding Wire  "));
      lawn_delay(100);

      
      // While the mower is still outside the boundary wire run this code unless and abort signal from the APP comes or it runs out of tries.
      while (inside != true && Wire_Find_Attempt < 100)
 //     while (( inside != true) && (Abort_Wire_Find == 0) )                                 // While the mower is still outside the fence run this code
      {
        Motor_Action_Go_Full_Speed();                                                       // Go full speed (in this case backwards)
        UpdateWireSensor();                                                                 // Read the wire sensor and see of the mower is now  or outside the wire
        ADCMan.run();
        PrintBoundaryWireStatus();                                                          // Prints of the status of the wire sensor readings.

        Wire_Find_Attempt++;                                                                // Counts how many loops have passed to find the wire.
#if (DEBUG_LEVEL >= 3)
        Serial.println();
        Serial.print("No Wire Count Backwards:");
        Serial.print(Wire_Find_Attempt);
        Serial.print("|");
#endif
      }
    }

    Motor_Action_Stop_Motors();
    delay(5);
  }

  Wire_Find_Attempt = 0; 
  // Code to go forwards until the mower is outside/ON the wire
  if (inside == true)              // If the Mower is situated  the wire then run the following code.
  {
    ADCMan.run();
    UpdateWireSensor();
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Moving Forwards to find the wire"));
    Serial.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
#endif
    PrintBoundaryWireStatus();
    Motor_Action_Stop_Motors();
    lawn_delay(1000);
    SetPins_ToGoForwards();                                                             // Set the motors to move the mower forwards
    lawn_delay(100);
    lcd.clear();
    lcd.print(F("Forward Try...  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Finding Wire  "));
    lawn_delay(100);
    while (inside != false && Wire_Find_Attempt < 100)                                // Move the mower forward until mower is outisde/ON the wire fence or 500 cycles have passed
    {
      Motor_Action_Go_Full_Speed();                                                     // Go full speed (in this case forwards)
      UpdateWireSensor();                                                               // Read the wire sensor and see of the mower is now  or outside the wire
      ADCMan.run();
      PrintBoundaryWireStatus();                                                        // Prints of the status of the wire sensor readings.
    
      Wire_Find_Attempt++;                                                              // Counts how many loops have passed to find the wire.
#if (DEBUG_LEVEL >= 2)
      Serial.println("");
      Serial.print("No Wire Count Forwards:");
      Serial.print(Wire_Find_Attempt);
      Serial.print("|");      
#endif      
    }
  }
  Motor_Action_Stop_Motors();
  delay(5);
}
//-----------------------------------------------------------------------------------------------------------------------------------
