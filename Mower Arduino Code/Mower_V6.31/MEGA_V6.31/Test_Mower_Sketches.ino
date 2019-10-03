//TODO
// 1. Why to use seperate function for Sonar detection?
// 2. Why use Perimeter_I in WHEEL test?
// 3. Optimize WHEEL test - use command array

//DONE
//

//-----------------------------------------------------------------------------------------------------------------------------------
/* Perimieter Wire Collision Motion
  ************************************************************************************/
void Test_Mower_Check_Wire()
{
  ADCMan.run();
  // ADCMan.setCapture(pinPerimeterLeft, 1, 0);

  UpdateWireSensor();

  //No need to delay - test cycle has own 200ms button press delay
  /*
    if (millis() >= nextTime)
    {
      nextTime = millis() + 50;
      if (perimeter.isInside(0) != inside)
      {
        inside = perimeter.isInside(0);
        counter++;
      }
    }
  */
  
#if (DEBUG_LEVEL >= 3)
  /* Prints Values to the Serial Monitor of mag, smag and signal quality.  */
  Serial.print(F("Inside (1) or Outside (0):  "));
  Serial.print((perimeter.isInside(0)));
  Serial.print(F("     MAG: "));
  Serial.print((int)perimeter.getMagnitude(0));
  Serial.print(F("    smag: "));
  Serial.print((int)perimeter.getSmoothMagnitude(0));
  Serial.print(F("     quality: "));
  Serial.println(perimeter.getFilterQuality(0));
#endif

  lcd.setCursor(0, 0);
  lcd.print(F("IN/Out:"));
  lcd.setCursor(8, 0);
  lcd.print(perimeter.isInside(0));
  lcd.setCursor(0, 1);
  lcd.print("MAG:");
  lcd.setCursor(8, 1);
  //Clean part of screen
  lcd.print(F("       "));
  lcd.setCursor(8, 1);
  lcd.print(perimeter.getMagnitude(0));
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Test_Relay()
{
  Turn_Off_Relay();
/*
  digitalWrite(Relay_Motors, HIGH);
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Relay OFF"));
#endif
*/
  lcd.print(F("Relay OFF"));
  lawn_delay(1000);
  lcd.clear();
  Turn_On_Relay();
/*  
  digitalWrite(Relay_Motors, LOW);
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Relay ON"));
#endif
*/
  lcd.print(F("Relay ON"));
  lawn_delay(1000);
  lcd.clear();
  //After test - leave relay OFF
  Turn_Off_Relay();
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Test_Wheel_Motors()
{
  Turn_On_Relay();
  //digitalWrite(Relay_Motors, LOW);
  lawn_delay(200);
  if (Perimeter_I == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Drive Wheel"));
    lcd.setCursor(0, 1);
    lcd.print("Test");
    lawn_delay(1000);

    lcd.clear();
    lcd.print(F("Remove ALL"));
    lcd.setCursor(0, 1);
    lcd.print(F("Blades!!!"));
    lawn_delay(1000);
    lcd.clear();

    lcd.print(F("<-- Turn Left"));
    lawn_delay(500);
    SetPins_ToTurnLeft();
    Motor_Action_Go_Full_Speed();
    lawn_delay(1000);
    Motor_Action_Stop_Motors();
    lawn_delay(1000);
    lcd.clear();

    lcd.print(F("Turn Right -->"));
    lawn_delay(500);
    SetPins_ToTurnRight();
    Motor_Action_Go_Full_Speed();
    lawn_delay(2000);
    Motor_Action_Stop_Motors();
    lawn_delay(1000);
    lcd.clear();

    lcd.print(F("Forwards"));
    lawn_delay(500);
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
    lawn_delay(1000);
    Motor_Action_Stop_Motors();
    lawn_delay(1000);
    lcd.clear();

    lcd.print(F("Backwards"));
    lawn_delay(500);
    SetPins_ToGoBackwards();
    Motor_Action_Go_Full_Speed();
    lawn_delay(1000);
    Motor_Action_Stop_Motors();
    lawn_delay(1000);
    lcd.clear();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dynamic");
    lcd.setCursor(0, 1);
    PWM_Left  = 120;
    PWM_Right = 120;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();
    lawn_delay(2000);

    lcd.clear();
    lcd.setCursor(0, 1);
    PWM_Left  = 255;
    PWM_Right = 0;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();

    lcd.clear();
    lcd.setCursor(0, 1);
    PWM_Left  = 255;
    PWM_Right = 0;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();
    lawn_delay(2000);

    lcd.clear();
    lcd.setCursor(0, 1);
    PWM_Left  = 150;
    PWM_Right = 0;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();
    lawn_delay(2000);

    lcd.clear();
    lcd.setCursor(0, 1);
    PWM_Left  = 255;
    PWM_Right = 0;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();
    lawn_delay(2000);

    lcd.clear();
    lcd.setCursor(0, 1);
    PWM_Left  = 255;
    PWM_Right = 0;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();

    lcd.clear();
    lcd.setCursor(0, 1);
    PWM_Left  = 0;
    PWM_Right = 255;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();
    lawn_delay(2000);

    lcd.clear();
    lcd.setCursor(0, 1);
    PWM_Left  = 0;
    PWM_Right = 150;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();
    lawn_delay(2000);

    lcd.clear();
    lcd.setCursor(0, 1);
    PWM_Left  = 0;
    PWM_Right = 255;
    lcd.print("L:");
    lcd.print(PWM_Left);
    lcd.print("  R:");
    lcd.print(PWM_Right);
    SetPins_ToGoForwards();
    Motor_Action_Dynamic_PWM_Steering();
    lawn_delay(2000);

    Motor_Action_Stop_Motors();
    lawn_delay(1000);

    lcd.clear();
    lcd.print(F("Test Finished"));
    lawn_delay(1000);
    lcd.clear();

    Perimeter_I = 2;
  }
  Turn_Off_Relay();
  //digitalWrite(Relay_Motors, HIGH);
  lawn_delay(200);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Test_Mower_Blade_Motor()
{
  // Spin the blade motor for 7 seconds
  Turn_On_Relay();
  //digitalWrite(Relay_Motors, LOW);
  lawn_delay(200);
  lcd.print(F("Blade Motor"));
  lcd.setCursor(0, 1);
  lcd.print(F("Test..!!"));
  lawn_delay(1000);
  lcd.clear();
  lcd.print(F("Remove ALL"));
  lcd.setCursor(0, 1);
  lcd.print(F("Blades!!!"));
  lawn_delay(4000);
  lcd.clear();
  lawn_delay(500);

#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Blades ON"));
#endif
  lcd.print(F("BLADE"));
  lcd.setCursor(0, 1);
  lcd.print("ON ");
#ifdef MowMotorDriver_BTS7960
  lcd.setCursor(6, 1);
  lcd.print("PWM =");
  lcd.print(PWM_Blade_Speed);
#endif

  Motor_Action_Spin_Blades();
  for (byte i = 0; i < 10; i++)
  {
    lawn_delay(1000);
    
    Print_LCD_Volt_Info();
    
    lcd.setCursor(8, 1);
    lcd.print("LC:");
    lcd.print(LoadCurrent, 2);
  }
  // Stop the blade motor spinning for 2 seconds
  lcd.clear();
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Blades OFF"));
#endif
  lcd.print(F("BLADE"));
  lcd.setCursor(0, 1);
  lcd.print(F("OFF"));
  Motor_Action_Stop_Spin_Blades();
  
  for (byte i = 0; i < 3; i++)
  {
    lawn_delay(1000);
    
    Print_LCD_Volt_Info();

    lcd.setCursor(8, 1);
    lcd.print("LC:");
    lcd.print(LoadCurrent, 2);
  }  
  
  lcd.clear();
  lawn_delay(500);

  Turn_Off_Relay();
  //digitalWrite(Relay_Motors, HIGH);
  lawn_delay(200);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Test_Sonar_Array()
{
  //Clears the Trig Pin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin3, LOW);

  /*Fires all Sonars to detect objects ahead...
     Sonars are not fired in order to avoid reflections of sonar in the next sensor.
     distance# reurned (trigpin#, echopin#, distance#, duration#, Sonar#, LCD Row#, LCDColumn#)
   *********************************************************************************************/
  if (Sonar_1_Activate) distance1 = PingSonarY(trigPin1, echoPin1, 1, 0, 5);       //SONAR1
  lawn_delay(15);
  if (Sonar_2_Activate) distance2 = PingSonarY(trigPin2, echoPin2, 2, 0, 0);       //SONAR2
  lawn_delay(15);
  if (Sonar_3_Activate) distance3 = PingSonarY(trigPin3, echoPin3, 3, 0, 10);      //SONAR3
  lawn_delay(15);
}
//-----------------------------------------------------------------------------------------------------------------------------------

/* SONAR Function
************************************************************************************/
// Function to Ping the Sonar calculate the distance from Object to the Sonars.
// Distance calculated is printed to serial printer and displays X or _ on the LCD Screen
// Distance calculated is then used for the object avoidance logic
// Sonars used can be activated in the settings.

int PingSonarY(int trigPinY, int echoPinY, int sonarY, int LCDRow, int LCDColumn)
{
  //Sets the trigPin at High state for 10 micro secs sending a sound wave
  digitalWrite(trigPinY, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPinY, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinY, LOW);
  delayMicroseconds(10);

  /*Reads the echoPin for the bounced wave and records the time in microseconds*/
  long durationY = pulseIn(echoPinY, HIGH, EchoTimeout);

  /*Calculates the distance in cm based on the measured time*/
  int distanceY = durationY * 0.0343 / 2;

  /* If a 0 distance is measured normally the Sonar ping has not been received.
    distance is then set to 999cm so the missed ping is not seen as an object detected.*/
  if (distanceY == 0)
  {
    distanceY = 999;
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("SONAR "));
    Serial.print(sonarY);
    Serial.print(": ");
    Serial.println(F("NO PING ERROR REMOVED"));
#endif
  }
  else
  {
#if (DEBUG_LEVEL >= 2)
    /*Prints the Sonar letter and distance measured on the serial Monitor*/
    Serial.print("SONAR ");
    Serial.print(sonarY);
    Serial.print(": ");
    Serial.print(distanceY);
    Serial.println(" cm");
#endif
  }
  
  lcd.setCursor(LCDColumn, LCDRow);
  lcd.print("    ");
  lcd.setCursor(LCDColumn, LCDRow);                 //sets location for text to be written
  
  /*If sonar distance is less than maximum distance then an object is registered to avoid*/
  //Prints that Sonar X has detected an object to the Mower LCD.
  if (distanceY <= MaxDistanceSonar)
    lcd.print("X");

  /*If sonar distance is greater than maximum distance then no object is registered to avoid*/
  //Prints that the path of Sonar X is open.
  if (distanceY > MaxDistanceSonar)
    lcd.print("_");

  lcd.print(distanceY);
  delayMicroseconds(EchoTimeout * 2);               //delay 2 times more then timeout 24000us * 2 = ~50ms (8m round trip)

  return distanceY;
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Test_Compass_Turn_Function()
{
  Turn_On_Relay();
  //digitalWrite(Relay_Motors, LOW);
  lawn_delay(200);
  SetPins_ToGoForwards();
  Motor_Action_Go_Full_Speed();
  lawn_delay(2000);
  Manouver_Turn_Around();
  Turn_To_Compass_Heading();
  SetPins_ToGoForwards();
  Motor_Action_Go_Full_Speed();
  lawn_delay(2000);
  Turn_Off_Relay();
  //digitalWrite(Relay_Motors, HIGH);
}
//-----------------------------------------------------------------------------------------------------------------------------------
