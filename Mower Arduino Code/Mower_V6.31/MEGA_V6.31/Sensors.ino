
//---------------------------------------------------------------------------------------
//Config

//Variables

int RawBatteryVoltage;
int RawChargeCurrent;
int RawLoadCurrent;
int RawRainSensor;

//---------------------------------------------------------------------------------------

void Setup_Sensors(void)
{
#if (DEBUG_LEVEL >= 3)
  Serial.println("Setup Sensor pins");
#endif

#ifdef UseLocalSensors
  ADCMan.setCapture(pinBatteryVoltage,    1, false); // no Ofs calibrate
  ADCMan.setCapture(pinChargeCurrent,     1, false);
  ADCMan.setCapture(pinLoadCurrent,       1, false);
  ADCMan.setCapture(pinRainSensor,        1, false);
#endif

#ifdef UseLiftSensors
  pinMode(LiftFrontLeftPin, INPUT_PULLUP);            // set pin as input
  pinMode(LiftFrontRightPin, INPUT_PULLUP);           // set pin as input

  LiftFrontLeft  = HIGH;
  LiftFrontRight = HIGH;
#endif

#ifdef UseBumperSensors
  pinMode(BumperFrontLeftPin, INPUT_PULLUP);            // set pin as input
  pinMode(BumperFrontRightPin, INPUT_PULLUP);           // set pin as input

  BumperFrontLeft  = HIGH;
  BumperFrontRight = HIGH;
#endif

  BatteryVoltage = -1;
  ChargeCurrent  = -1;
  LoadCurrent    = -1;
}
//---------------------------------------------------------------------------------------

void Loop_Sensors(void)
{
#ifdef UseLocalSensors
  ADCMan.run();

  if (ADCMan.isCaptureComplete(pinBatteryVoltage))
  {
    RawBatteryVoltage =  ADCMan.read(pinBatteryVoltage);
    BatteryVoltage = (RawBatteryVoltage * 5.0) / 1024.0 * (VoltageDeviderR1 + VoltageDeviderR2) / VoltageDeviderR1;
  }

  if (ADCMan.isCaptureComplete(pinChargeCurrent))
  {
    RawChargeCurrent =  ADCMan.read(pinChargeCurrent);
    ChargeCurrent = ((RawChargeCurrent * 5000) / 1024.0 - CurrentOffset) / CurrentmVperA;
  }

  if (ADCMan.isCaptureComplete(pinLoadCurrent))
  {
    RawLoadCurrent =  ADCMan.read(pinLoadCurrent);
    LoadCurrent = ((RawLoadCurrent * 5000) / 1024.0 - CurrentOffset) / CurrentmVperA;
  }

  if (ADCMan.isCaptureComplete(pinRainSensor))
    RawRainSensor =  ADCMan.read(pinRainSensor);
#endif

#ifdef UseLiftSensors
  LiftFrontLeft  = digitalRead(LiftFrontLeftPin);
  LiftFrontRight = digitalRead(LiftFrontRightPin);
#endif

#ifdef UseLiftSensors
  BumperFrontLeft  = digitalRead(BumperFrontLeftPin);
  BumperFrontRight = digitalRead(BumperFrontRightPin);
#endif

  Action_Sensors();
}
//---------------------------------------------------------------------------------------

void Action_Sensors(void)
{
#ifdef UseLiftSensors
  // If both wheels are lifted - warning state
  if (LiftFrontLeft == LOW && LiftFrontRight == LOW)
    Mower_SaftyWarning = 1;

  // If both wheels touch ground - cancel warning state
  if (LiftFrontLeft == HIGH && LiftFrontRight == HIGH)
    Mower_SaftyWarning = 0;
#endif

  Motor_SaftyWarning_Pause();
}
//---------------------------------------------------------------------------------------
