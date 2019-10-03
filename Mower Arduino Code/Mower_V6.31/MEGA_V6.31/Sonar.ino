//TODO:
// 1. Multiple delays - are they nessesary? It looks that 15ms for reflection problems should be enough
// 2. Code optimization - use array[3] for Sonars

//DONE
// 1. Timeout for pulseIn - 6000uS (6ms) (1m distance)
// 2. PingSonarX parameters durationX, distanceX are not used
// 3. BUG: LCD is 16x2, but PingSonarX is used with lines 0, 1 and 2 (2 is out of range)
//    Rows and Columns was mixed up
// 4. BUG: Posible use of trigPin1 trigPin2 trigPin3 and echoPin1 echoPin2 echoPin3 before setup them as INPUT/OUTPUT
// 5. BUG: delayMicroseconds(10) should be between HIGH/LOW, not after LOW

//-----------------------------------------------------------------------------------------------------------------------------------

void Setup_Sonar()
{
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Setup Sonar pins"));
#endif

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Check_Sonar_Sensors()
{
  // Ping Sonar sensors

  //Clears the Trig Pin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5); //Why this?! Slows down detection
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5); //Why this?! Slows down detection
  digitalWrite(trigPin3, LOW);

  // Pings each sonar at a 15ms interval

  if (Sonar_2_Activate == 1) distance2 = PingSonarX(trigPin2, echoPin2, 2, 0, 0, false);         //SONAR2
  if (Sonar_1_Activate == 1) distance1 = PingSonarX(trigPin1, echoPin1, 1, 0, 1, false);         //SONAR1
  if (Sonar_3_Activate == 1) distance3 = PingSonarX(trigPin3, echoPin3, 3, 0, 2, false);         //SONAR3
}
//-----------------------------------------------------------------------------------------------------------------------------------

/* SONAR Function
************************************************************************************/
// Function to Ping the Sonar calculate the distance from Object to the Sonars.
// Distance calculated is printed to serial printer and displays X or _ on the LCD Screen
// Distance calculated is then used for the object avoidance logic
// Sonars used can be activated in the settings.

int PingSonarX(int trigPinX, int echoPinX, int sonarX, int LCDRow, int LCDColumn, bool Test)
{
  //Sets the trigPin at High state for 10 micro secs sending a sound wave
  digitalWrite(trigPinX, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinX, LOW);
  delayMicroseconds(10);

  /*Reads the echoPin for the bounced wave and records the time in microseconds*/
  long durationX = pulseIn(echoPinX, HIGH, EchoTimeout);

  /*Calculates the distance in cm based on the measured time*/
  int distanceX = durationX * 0.034 / 2;
  delay(5); //Why this?! Slows down detection

  /* If a 0 distance is measured normally the Sonar ping has not been received.
    distance is then set to 999cm so the missed ping is not seen as an object detected.*/
  if (distanceX == 0)
  {
    distanceX = 999;
#if (DEBUG_LEVEL >= 3)
    Serial.print(F("S"));
    Serial.print(sonarX);
    Serial.print(F(":"));
    Serial.print(F("NP!"));
    Serial.print(F("|"));
#endif
  }

  /*Prints the Sonar letter and distance measured on the serial Monitor*/
#if (DEBUG_LEVEL >= 3)
  Serial.print(F("S"));
  Serial.print(sonarX);
  Serial.print(F(":"));
  Serial.print(distanceX);
  Serial.print(F("cm"));
  Serial.print(F("/"));
#endif

  /*If sonar distance is less than maximum distance then an object is registered to avoid*/
  if (distanceX <= MaxDistanceSonar)
  {
    //Prints that Sonar X has detected an object to the Mower LCD.
    lcd.setCursor(LCDColumn, LCDRow);                //sets location for text to be written
    lcd.print("X");
    delay(10); //Why this?! Slows down detection
    if (sonarX == 1)
    {
      Sonar_Hit_1_Total = (Sonar_Hit_1_Total + 1);
#if (DEBUG_LEVEL >= 2)
      Serial.print(Sonar_Hit_1_Total);
#endif
    }
    if (sonarX == 2)
    {
      Sonar_Hit_2_Total = (Sonar_Hit_2_Total + 1);
#if (DEBUG_LEVEL >= 2)
      Serial.print(Sonar_Hit_2_Total);
#endif
    }
    if (sonarX == 3)
    {
      Sonar_Hit_3_Total = (Sonar_Hit_3_Total + 1);
#if (DEBUG_LEVEL >= 2)
      Serial.print(Sonar_Hit_3_Total);
#endif
    }
    if ((Sonar_Hit_1_Total >= Max_Sonar_Hit) || (Sonar_Hit_2_Total >= Max_Sonar_Hit) || (Sonar_Hit_3_Total >= Max_Sonar_Hit))
    {
      Sonar_Hit = 1;
      Print_Sonar_Hit();
#if (DEBUG_LEVEL >= 2)
      Serial.println("");
      Serial.println(F("Sonar Hit Detected"));
#endif
    }
  }

  /*If sonar distance is greater than maximum distance then no object is registered to avoid*/
  if (distanceX > MaxDistanceSonar)
  {
    //Prints that the path of Sonar X is open.
    lcd.setCursor(LCDColumn, LCDRow);                 //sets location for text to be written
    lcd.print("_");
    delay(10); //Why this?! Slows down detection
    if (sonarX == 1)
    {
      Sonar_Hit_1_Total = 0;
#if (DEBUG_LEVEL >= 2)
      Serial.print(Sonar_Hit_1_Total);
#endif
    }
    if (sonarX == 2)
    {
      Sonar_Hit_2_Total = 0;
#if (DEBUG_LEVEL >= 2)
      Serial.print(Sonar_Hit_2_Total);
#endif
    }
    if (sonarX == 3)
    {
      Sonar_Hit_3_Total = 0;
#if (DEBUG_LEVEL >= 2)
      Serial.print(Sonar_Hit_3_Total);
#endif
    }
  }

  return distanceX;
  //return sonarX;  //WTF?
}
//-----------------------------------------------------------------------------------------------------------------------------------
