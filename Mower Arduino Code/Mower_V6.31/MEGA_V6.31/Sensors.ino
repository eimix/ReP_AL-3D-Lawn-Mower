
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
  Serial.println(F("Setup Sensor pins"));
#endif

#ifdef UseLocalSensors
  ADCMan.setCapture(pinBatteryVoltage,    1, false); // no Ofs calibrate
  ADCMan.setCapture(pinChargeCurrent,     1, false);
  ADCMan.setCapture(pinLoadCurrent,       1, false);
  ADCMan.setCapture(pinRainSensor,        1, false);
#endif

#ifdef UseBeeper
  pinMode(BeeperPin, OUTPUT);
#endif

#ifdef UseLiftSensors
  pinMode(LiftFrontLeftPin,  INPUT_PULLUP);           // set pin as input
  pinMode(LiftFrontRightPin, INPUT_PULLUP);           // set pin as input

  LiftFrontLeft  = LiftSensorBAD;
  LiftFrontRight = LiftSensorBAD;
#endif

#ifdef UseBumperSensors
  pinMode(BumperFrontLeftPin,  INPUT_PULLUP);           // set pin as input
  pinMode(BumperFrontRightPin, INPUT_PULLUP);           // set pin as input

  BumperFrontLeft  = HIGH;
  BumperFrontRight = HIGH;
#endif

  BatteryVoltage = -1;
  ChargeCurrent  = 0;
  LoadCurrent    = 0;
}
//---------------------------------------------------------------------------------------

void Loop_Sensors(void)
{
#ifdef UseLocalSensors
  ADCMan.run();

  if (ADCMan.isCaptureComplete(pinBatteryVoltage))
  {
    RawBatteryVoltage = ADCMan.read(pinBatteryVoltage);
    BatteryVoltage    = RawBatteryVoltage * 5.0 / 1023.0 * (VoltageDeviderR1 + VoltageDeviderR2) / VoltageDeviderR1;
  }

  if (ADCMan.isCaptureComplete(pinChargeCurrent))
  {
    RawChargeCurrent = ADCMan.read(pinChargeCurrent);
    ChargeCurrent    = fabs(RawChargeCurrent * 5000.0 / 1023.0 - CurrentOffset) / CurrentmVperA;
  }

  // DC motors gerates high startup current spikes, average them to get clean value
  if (ADCMan.isCaptureComplete(pinLoadCurrent))
  {
    RawLoadCurrent = ADCMan.read(pinLoadCurrent);
    LoadCurrent    = LoadCurrent * 0.9 + fabs(RawLoadCurrent * 5000.0 / 1023.0 - CurrentOffset) / CurrentmVperA * 0.1;
  }

  if (ADCMan.isCaptureComplete(pinRainSensor))
    RawRainSensor = ADCMan.read(pinRainSensor);

#if (DEBUG_LEVEL >= 4)
#if (Show_TX_Data == 1)
  Serial.print("AmpL:");
  Serial.print(RawLoadCurrent);
  Serial.print("|");
  Serial.print("AmpC:");
  Serial.print(RawChargeCurrent);
  Serial.print("|");
  Serial.print("Volt:");
  Serial.print(RawBatteryVoltage);
  Serial.print("|");
  Serial.print("Rain:");
  Serial.print(RawRainSensor);
  Serial.print("|");
#endif
#endif

#endif

#ifdef UseLiftSensors
  LiftFrontLeft  = digitalRead(LiftFrontLeftPin);
  LiftFrontRight = digitalRead(LiftFrontRightPin);
#endif

#ifdef UseBumperSensors
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
  if (LiftFrontLeft == LiftSensorBAD && LiftFrontRight == LiftSensorBAD)
  {
#if (DEBUG_LEVEL >= 2)
    if ((Mower_SaftyWarning & 1) == 0)
      Serial.println(F("Lift warning"));
#endif;
    Mower_SaftyWarning = Mower_SaftyWarning | 1;
  }

  // If both wheels touch ground - cancel warning state
  if (LiftFrontLeft != LiftSensorBAD && LiftFrontRight != LiftSensorBAD)
    Mower_SaftyWarning = Mower_SaftyWarning & 30; //.....11110
#endif

#ifdef UseLocalSensors
  // If max load current is exceeded - warning, not resetible automaticaly, only manual reset
  if (LoadCurrent > MaxLoadCurrent)
  {
#if (DEBUG_LEVEL >= 2)
    if ((Mower_SaftyWarning & 2) == 0)
    {
      Serial.print(F("Overload warning: "));
      Serial.print(LoadCurrent, 2);
      Serial.println("A");
    }
#endif
    Mower_SaftyWarning = Mower_SaftyWarning | 2;
  }
#endif

  Motor_SaftyWarning_Pause();
}
//---------------------------------------------------------------------------------------

void lawn_delay(unsigned long d)
{
  unsigned long start = millis();
  unsigned long t = 0;
  while (t < d)
  {
#ifdef UseLocalSensors
    //If 250ms already passed (delay motor startup)
    //and if remaining delay time >32ms - read sensors data
    if (t > 250 && d - t > 32)
      Loop_Sensors();

    //If remaining delay >250ms - send data to WiFi
    //Send "driver" controls data sending in 250ms intervals
#ifdef UseWiFi
    if (d - t > 250)
      Transmit_All_To_NODEMCU();
#endif
#endif
    delay(1);
    yield();
    t = calcTimeDiff(start, millis());
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------
