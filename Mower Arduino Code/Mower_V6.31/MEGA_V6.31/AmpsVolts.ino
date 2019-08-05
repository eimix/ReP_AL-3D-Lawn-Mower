//---------------------------------------------------------------------------------------

//Serial Communication
#ifdef UseRemoteSensors
int   RawValueAmp;
int   RawValueVolt;
int   Zero_Volts;
float Volts_Last;
#endif
int   Rain_Detected;
int   Rain_Hit_Detected = 0;

//---------------------------------------------------------------------------------------

void Process_Volt_Information()
{
  //  Logic for how the battery readings should be handled

  if (BatteryVoltage < Battery_Min)
  {
    LowBatteryCount++;
    Serial.print("VLow:");
    Serial.print(LowBatteryCount);
    Serial.print("|");
    if (LowBatteryCount > MaxLowBatteryCount)
    {
      Serial.println("Low Battery Detected");
      if (Use_Charging_Station == 1) Manouver_Go_To_Charging_Station();                       // Stops the mowing and sends the mower back to the charging station via the permieter wire
      if (Use_Charging_Station == 0) Manouver_Park_The_Mower_Low_Batt();                      // Parks the mower with a low battery warning
    }
  }

  if (BatteryVoltage >= Battery_Min)
  {
    Serial.print("VLow:");
    Serial.print(LowBatteryCount);
    Serial.print("|");
    LowBatteryCount = 0;
  }
}
//---------------------------------------------------------------------------------------

// checks to see if the mower is on the charging station
void Check_if_Charging()
{
  if (ChargeCurrent < 0.4)
    Mower_Charging = 0;
  if (ChargeCurrent > 0.4)
    Mower_Charging = 1;

  Serial.print("Charging:");
  Serial.print(Mower_Charging);
  Serial.print("|");
  Print_Charging_LCD();
}
//---------------------------------------------------------------------------------------

void Check_if_Docked()
{
  if (Mower_Charging == 1)                                     // if Amps are between this there is a charge detected.  Amps above 4 are discounted as a miscommunication
  {
    Motor_Action_Stop_Motors();
    Serial.println(F("Charging Current detected"));
    Serial.println(F("Mower Docked"));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Docked in");
    lcd.setCursor(0, 1);
    lcd.print("Charging Station");                                // Prints info to LCD display
    delay(2000);
    Mower_Docked = 1;
    Manouver_Dock_The_Mower();                                    // Shuts down the Mower ready for charging and mowing again.
    lcd.clear();
  }
  Serial.println("");
}
//---------------------------------------------------------------------------------------

#ifdef UseRemoteSensors
void Calculate_Volt_Amp_Charge()
{
  if (Show_TX_Data == 1)
  {
    Serial.print("Amp:");
    Serial.print(RawValueAmp);
    Serial.print("|");
    Serial.print("Volt:");
    Serial.print(RawValueVolt);
    Serial.print("|");
    Serial.print("Rain:");
    Serial.print(Rain_Detected);
    Serial.print("|");
  }

  // Calculate Amps from NANO RX Data
  int mVperAmp  = 185;
  int ACSoffset = 2500;
  double VoltageAmp = 0;
  VoltageAmp = (RawValueAmp / 1024.0) * 5000; // Gets you mV
  ChargerCurrent =  ((VoltageAmp - ACSoffset) / mVperAmp);

  Serial.print(F("A:"));
  Serial.print(ChargerCurrent);
  Serial.print(F("|"));

  // Calculate Voltage from NANO RX Data

  if (RawValueVolt > 100)
  {
    float vout = 0.0;
    vout = (RawValueVolt * 5.0) / 1024.0; // see text
    BatteryVoltage = vout / (VoltageDeviderR1 / (VoltageDeviderR2 + VoltageDeviderR1));
    Volts_Last = BatteryVoltage;
    Zero_Volts = 0;
  }
  else
  {
    BatteryVoltage = Volts_Last;
    Zero_Volts = Zero_Volts + 1;
    if (Zero_Volts > 5)
      BatteryVoltage = 0;
  }

  Serial.print(F("V:"));
  Serial.print(BatteryVoltage);
  Serial.print(F("|"));

  if (ChargerCurrent < 0.4) Mower_Charging = 0;
  if (ChargerCurrent > 0.4) Mower_Charging = 1;
}
#endif
//---------------------------------------------------------------------------------------
