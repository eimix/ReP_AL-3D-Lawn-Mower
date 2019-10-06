#ifdef UseCompassQMC5883
//-----------------------------------------------------------------------------------------------------------------------------------
// COMPASS FUNCTIONS
//*****************************************************************************

//-----------------------------------------------------------------------------------------------------------------------------------

/* Calculates the compass heading as heading & degrees of the onboard compass */
void Get_Compass_Reading(void)
{
  Vector norm = compass.readNormalize();
  delay(5);
  Compass_Heading = atan2(norm.YAxis, norm.XAxis);                        // Calculate heading
  Compass_Heading += Declination_Angle;

/* Compass_Heading is used in output only, so no need normalize, degree version of this value will be normalized
  if (Compass_Heading < 0)                                                // Correct for heading < 0deg and heading > 360deg
    Compass_Heading += 2 * PI;

  if (Compass_Heading > 2 * PI)
    Compass_Heading -= 2 * PI;
*/

  Compass_Heading_Degrees = Compass_Heading * 180 / M_PI;                 // Convert to degrees
  Compass_Heading_Degrees = (Compass_Heading_Degrees + 360) % 360;
  //Serial.print(F("Comp H:"));
  //Serial.print(Compass_Heading);
#if (DEBUG_LEVEL >= 3)
  Serial.print(F("Comp°:"));
  Serial.print(Compass_Heading_Degrees);
  Serial.print("|");
#endif
#if (DEBUG_LEVEL >= 4)
  Serial.print(F("X:"));
  Serial.print(norm.XAxis);
  Serial.print("|");
  Serial.print(F("Y:"));
  Serial.print(norm.YAxis);
  Serial.print("|");
  Serial.print(F("Z:"));
  Serial.print(norm.ZAxis);
  Serial.print("|");
#endif
  delay(5);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Get_Compass_Reading_Multiple(byte Count, int Delay)
{
  while (Count > 0)
  {
    Get_Compass_Reading();
    if (Count > 1)
      delay(Delay);

    Count--;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

// Turns the Mower to the correct orientation for the optimum home wire track
// Avoiding tracking around the whole wire to get back to the docking station
void Compass_Turn_Mower_To_Home_Direction()
{
  //Stop the motors
  Motor_Action_Stop_Motors;
  lawn_delay(2000);
  Print_LCD_Compass_Home();
  lawn_delay(1000);
  Compass_Target = Home_Wire_Compass_Heading;
  Print_LCD_Heading_for_Home();
  lawn_delay(2000);
  lcd.clear();

  // Reverse the mower a little
  SetPins_ToGoBackwards();
  Motor_Action_Go_Full_Speed();
  lawn_delay(800);
  Motor_Action_Stop_Motors;  
      
  Get_Compass_Reading();
  SetPins_ToTurnLeft();
#if (DEBUG_LEVEL >= 3)
  Serial.print(F("Compass Heading Now : "));
  Serial.println(Compass_Heading_Degrees);
  Serial.println(F("********************************"));
#endif
  lawn_delay(100);
  lcd.print(Compass_Heading_Degrees);
  // This spins the mower a little to ensure a true compass reading is being read (calibration).
  SetPins_ToTurnLeft();                                            // Calls the motor function turn Left
  Motor_Action_Turn_Speed();                                       // Sets the speed of the turning motion
  lawn_delay(500);
  Motor_Action_Stop_Motors();
  Get_Compass_Reading();
  lcd.clear();
  lcd.print(Compass_Heading_Degrees);
  SetPins_ToTurnLeft();                                            // Calls the motor function turn Left
  lawn_delay(100);
  Motor_Action_Turn_Speed();                                       // Sets the speed of the turning motion
  lawn_delay(2000);
  Motor_Action_Stop_Motors();
  Get_Compass_Reading();
  lawn_delay(500);
  lcd.clear();
  lcd.print(F("Compass Set"));
  Motor_Action_Stop_Motors();
  lawn_delay(2000);
  Turn_To_Compass_Heading();
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Turn_To_Compass_Heading()
{
  // Step turns the mower to the left while the heading is outside the home tolerance
  // Once the heading is found. the mower stops and can then activate the find wire function

  lcd.setCursor(0, 0);
  lcd.print(F("Target: "));
  lcd.print(Compass_Target);
  lcd.setCursor(0, 1);
  lcd.print("Now:");
  lcd.setCursor(0, 4);

/*  
  int Heading_Lower_Limit_Compass = Compass_Target - 5;
  int Heading_Upper_Limit_Compass = Compass_Target + 5;

  if (Heading_Lower_Limit_Compass < 0)
    Heading_Lower_Limit_Compass += 360;
  if (Heading_Lower_Limit_Compass > 360)
    Heading_Lower_Limit_Compass -= 360;
*/
    
  lawn_delay(500);
  int Cancel = 0;
  int Compass_Error = 10; //Any value >5 to start cycle

//  while ((Compass_Heading_Degrees < Heading_Lower_Limit_Compass) || (Compass_Heading_Degrees > Heading_Upper_Limit_Compass) && (Cancel < 40))
  while (abs(Compass_Error) > 5  && (Cancel < 40))
  {
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Turning to Target:"));
    Serial.print(Compass_Target);
    Serial.print(F("|"));
#endif
    Get_Compass_Reading();
//    delay(50); //Why this?
    lcd.setCursor(0, 0);
    lcd.print(F("Degrees: "));
    lcd.print(Compass_Heading_Degrees);
#if (DEBUG_LEVEL >= 3)
    Serial.println("");
#endif

    int Compass_Error = Compass_Heading_Degrees - Compass_Target;
    //Normalize error in range [-180;180]
    Compass_Error = (Compass_Error + 180) % 360 - 180; // % = mod
    
    lcd.setCursor(0, 1);
    lcd.print("Error:");
    lcd.print(Compass_Error);
#if (DEBUG_LEVEL >= 3)
    Serial.print("Er:");
    Serial.print(Compass_Error);
    Serial.print(F("|"));
#endif

    if (Compass_Error < 0)
    {
      SetPins_ToTurnRight();
#if (DEBUG_LEVEL >= 3)
      Serial.print(F("Spin Right"));
      Serial.print(F("|"));
#endif
    }
    if (Compass_Error > 0)
    {
      SetPins_ToTurnLeft();
#if (DEBUG_LEVEL >= 3)
      Serial.print(F("Spin Left"));
      Serial.print(F("|"));
#endif
    }
//    delay(100); //Why this?

    //Linear turn speed adjustmen function
    byte Turn_Adjust = abs(Compass_Error) / 3 + 30; //0
/*    
    Compass_Error = abs(Compass_Error);
    if (Compass_Error < 10)  Turn_Adjust = 40; //120;
    if (Compass_Error < 20)  Turn_Adjust = 50; //100;
    if (Compass_Error < 50)  Turn_Adjust = 60; //80;
    if (Compass_Error < 180) Turn_Adjust = 90; //20;
*/
    Motor_Action_Turn_Speed_Adjust(Turn_Adjust);                                       // Sets the speed of the turning motion
    lawn_delay(100); //This one could be increased, because 150ms was deducted in upper code
    Cancel++;
    lcd.setCursor(12, 1);
    lcd.print(Cancel);
  }
  Get_Compass_Reading_Multiple(2, 5);
/*
  Get_Compass_Reading();
  delay(5);
  Get_Compass_Reading();
*/
  // Once the while loop is satisfied (compass measures a degree between Lower and Upper, stop the mower
  Motor_Action_Stop_Motors();
  SetPins_ToGoForwards();
  lcd.print("Found");
  lawn_delay(1000);
  lcd.clear();
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Display_Compass_Current_Heading_on_LCD()
{
  lcd.setCursor(5, 1);
  lcd.print("    ");
  lcd.print(Compass_Heading_Degrees);
  delay(10);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Calculate_Compass_Wheel_Compensation()
{
/*
  float Compass_Error = Compass_Heading_Degrees - Heading_Lock;        // Calculates the error in compass heading from the saved lock heading

  if (Compass_Error > 180)
    Compass_Error = Compass_Error * -1;
  if (Compass_Error < -180)
    Compass_Error = Compass_Error * -1;
*/
  int Compass_Error = Compass_Heading_Degrees - Compass_Heading_Lock_Degrees;
  //Normalize error in range [-180;180]
  Compass_Error = (Compass_Error + 180) % 360 - 180; // % = mod
  
#if (DEBUG_LEVEL >= 3)
  Serial.print(F("C_Err:"));
  Serial.print(Compass_Error);
  Serial.print("|");
#endif

  if (Compass_Error < 0)                                               // Steer left
  {
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Steer_Right|"));
#endif
    PWM_Right = PWM_MaxSpeed_RH + (CompassCorectionPower * Compass_Error);            // Multiply the difference by D to increase the power then subtract from the PWM
    if (PWM_Right < 0)
      PWM_Right = 0; //PWM_MaxSpeed_RH - 50;
    PWM_Left = PWM_MaxSpeed_LH;                                        // Keep the Right wheel at full power calibrated to go straight
  }
  if (Compass_Error >= 0)
  {
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("Steer_Left|"));
#endif
    PWM_Right = PWM_MaxSpeed_RH;                                       // Keep the Left wheel at full power calibrated to go straight
    PWM_Left = PWM_MaxSpeed_LH - (CompassCorectionPower * Compass_Error);             // Multiply the difference by D to increase the power then subtract from the PWM
    if (PWM_Left < 0)
      PWM_Left = 0; //PWM_MaxSpeed_LH - 50;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------
#endif
