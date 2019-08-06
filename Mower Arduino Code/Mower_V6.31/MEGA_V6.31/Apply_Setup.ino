//---------------------------------------------------------------------------------------

void Print_Mower_Status()
{
  //Serial.print(F("Mower Stat...."));
#if (DEBUG_LEVEL >= 3)
  if (Mower_Docked == 1)          Serial.print(F("Docked:1|"));
  if (Mower_Parked == 1)          Serial.print(F("Parked:1|"));
  if (Mower_Running == 1)         Serial.print(F("Running:1|"));
  if (Manuel_Mode == 1)           Serial.print(F("Manuel Mode:1|"));
  if (Mower_Parked_Low_Batt == 1) Serial.print(F("Park_Low_Batt:1|"));
  if (Mower_Error == 1)           Serial.print(F("Mower Error:1|"));
  if (Mower_SaftyWarning == 1)    Serial.print(F("Mower Warning:1|"));
  if (Mower_Charging == 1)        Serial.print(F("Charging:1|"));
#endif
}
//---------------------------------------------------------------------------------------

void Prepare_Mower_from_Settings()
{
  Mower_Charging = false;

  if (Use_Charging_Station == 1)
  {
    Mower_Docked  = 1;
    Mower_Parked  = false;
    Mower_Running = false;
  }

  if (Use_Charging_Station == 0)
  {
    Mower_Docked  = 0;
    Mower_Parked  = 1;
    Mower_Running = false;
  }
}
//---------------------------------------------------------------------------------------

void Setup_Compass()
{
  if (Compass_Activate == 1)
  {
    /*Setup Compass
    *************************************************************************/
    lcd.clear();
    lcd.print(F("Compass  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Setup"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Setup Compass"));
#endif

    while (!compass.begin())
    {
#if (DEBUG_LEVEL >= 1)
      Serial.println(F("Could not find a valid QMC5883 sensor, check wiring!"));
      delay(500);
#endif
    }

    if (compass.isHMC())
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println(F("Initialize HMC5883"));
#endif
      compass.setRange(HMC5883L_RANGE_1_3GA);
      compass.setMeasurementMode(HMC5883L_CONTINOUS);
      compass.setDataRate(HMC5883L_DATARATE_15HZ);
      compass.setSamples(HMC5883L_SAMPLES_8);
    }
    else if (compass.isQMC())
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println(F("Initialize QMC5883"));
#endif
      compass.setRange(QMC5883_RANGE_2GA);
      compass.setMeasurementMode(QMC5883_CONTINOUS);
      compass.setDataRate(QMC5883_DATARATE_50HZ);
      compass.setSamples(QMC5883_SAMPLES_8);
    }
    lcd.clear();
    lcd.print(F("Compass Setup "));
    lcd.setCursor(0, 1);
    lcd.print(F("Done!             "));
    delay(500);
    lcd.clear();
  }

#if (DEBUG_LEVEL >= 2)
  if (Compass_Activate == 0)
    Serial.println(F("Compass Switched off - Select 1 in setup to switch on."));
#endif
}
//---------------------------------------------------------------------------------------

void Setup_Relays()
{
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Setup Relays"));
#endif
  pinMode(Relay_Motors, OUTPUT);
  delay(5);
  Turn_Off_Relay();
  delay(5);
}
//---------------------------------------------------------------------------------------

void Setup_Motor_Pins()
{
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Setup Motor Pins"));
#endif

#ifdef MowMotorDriver_BTS7960
  pinMode(L_EN, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(RPWM, OUTPUT);
#endif

#ifdef MowMotorDriver_RELAY
  pinMode(MowMotorRelayPin, OUTPUT);
#endif
}
//---------------------------------------------------------------------------------------

void  Turn_On_Relay()
{
#if (DEBUG_LEVEL >= 3)
  Serial.print(F("Relay:ON|"));
#endif
  digitalWrite(Relay_Motors, LOW);                         // Turn of the relay for the main battery power
}
//---------------------------------------------------------------------------------------

void  Turn_Off_Relay()
{
#if (DEBUG_LEVEL >= 3)
  Serial.print(F("Relay:Off|"));
#endif
  digitalWrite(Relay_Motors, HIGH);                         // Turn of the relay for the main battery power
}
//---------------------------------------------------------------------------------------

void Setup_Membrane_Buttons()
{
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("Setup Membrane Keys"));
#endif
  pinMode(StartKeyPin, INPUT_PULLUP);           // set pin as input
  pinMode(PlusKeyPin, INPUT_PULLUP);            // set pin as input
  pinMode(MinusKeyPin, INPUT_PULLUP);           // set pin as input
  pinMode(StopKeyPin, INPUT_PULLUP);            // set pin as input
}
//---------------------------------------------------------------------------------------

void Setup_ADCMan()
{
#if (DEBUG_LEVEL >= 3)
  Serial.println(F("ADCMAN"));
#endif
  ADCMan.init();
  perimeter.setPins(pinPerimeterLeft, pinPerimeterRight);
  perimeter.useDifferentialPerimeterSignal = true;
  perimeter.speedTest();
  ADCMan.run();
}
//---------------------------------------------------------------------------------------
