//----------------------------------------------------------------------------------------------

void Update_Blynk_App_With_Status()
{
/*
  if (Mower_Docked_Filter == 2)
  {
    //App Buttons
    Blynk.virtualWrite(V10, LOW);       // Exit Dock Button
    Blynk.virtualWrite(V0,  LOW);       // Quick Start Button
    Blynk.virtualWrite(V2,  LOW);       // Pause/Stop Button
    Blynk.virtualWrite(V1,  LOW);       // Go To Dock Button
  }

  if (Mower_Running_Filter == 2)
  {
    //App Buttons
    Blynk.virtualWrite(V10, LOW);       // Exit Dock Button
    Blynk.virtualWrite(V0,  LOW);       // Quick Start Button
    Blynk.virtualWrite(V2,  LOW);       // Pause/Stop Button
    Blynk.virtualWrite(V1,  LOW);       // Go To Dock Button
  }

  if (ManualMode == 1)
  {
    //App Buttons
    Blynk.virtualWrite(V10, LOW);       // Exit Dock Button
    Blynk.virtualWrite(V0,  LOW);       // Quick Start Button
    Blynk.virtualWrite(V2,  LOW);       // Pause/Stop Button
    Blynk.virtualWrite(V1,  LOW);       // Go To Dock Button

    Blynk.virtualWrite(V6, 0);
    Blynk.virtualWrite(V7, 0);
    Blynk.virtualWrite(V9, 0);
    Blynk.virtualWrite(V8, 0);
  }

  if (Mower_Parked == 1)
  {
    //App Buttons
    Blynk.virtualWrite(V10, LOW);       // Exit Dock Button
    Blynk.virtualWrite(V0,  LOW);       // Quick Start Button
    Blynk.virtualWrite(V2,  LOW);       // Pause/Stop Button
    Blynk.virtualWrite(V1,  LOW);       // Go To Dock Button
  }

>  if (Charge_Detected == 4)          Blynk.virtualWrite(V12, 1023);        // Charging LED ON
>  if (Charge_Detected == 0)          Blynk.virtualWrite(V12, 0);           // Charging LED OFF
>  if (Tracking_Wire == 1)            Blynk.virtualWrite(V9, 1023);         // Tracking LED ON
>  if (Tracking_Wire == 0)            Blynk.virtualWrite(V9, 0);            // Tracking LED OFF
>  if (Mower_Parked == 1)             Blynk.virtualWrite(V6, 1023);         // Parked LED
>  if (Mower_Parked == 0)             Blynk.virtualWrite(V6, 0);            // Parked LED
>  if (Mower_Running_Filter == 2)     Blynk.virtualWrite(V7, 1023);         // Mowing LED
>  if (Mower_Running_Filter != 2)     Blynk.virtualWrite(V7, 0);            // Mowing LED
>  if (Mower_Docked_Filter == 2)      Blynk.virtualWrite(V8, 1023);         // Docked LED
>  if (Mower_Docked_Filter != 2)      Blynk.virtualWrite(V8, 0);            // Docked LED
*/  
}
//----------------------------------------------------------------------------------------------

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V0)                           // Quick Start Mowing Function
{
//  Clear_APP();
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    StartMower();
    //Clear_APP();
    lcd.clear();
    lcd.print(0, 0, "Starting . .");        // Print to APP LCD
  }
//??? delay(100);
//??? Blynk.virtualWrite(V0, HIGH);           // Start Mowing Buton ON
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V1)                           // Go to Docking Station
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    Going_Home();
    //Clear_APP();
    lcd.clear();
    lcd.print(0, 0, "Docking . .");         // Print to APP LCD
  }  
//???  delay(100);
//???  Blynk.virtualWrite(V1, HIGH);           // Docking Button ON
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V2)                           // Pause Mower Function
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    Pause_Mower();
    //Clear_APP();
    lcd.clear();
    lcd.print(0, 0, "Stopping . .");        // Print to APP LCD
  }  
//???  delay(100);
//???  Blynk.virtualWrite(V2, HIGH);           // Pause Button ON
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V10)                          // Exit Dock Function
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    Exit_Dock();
    //Clear_APP();
    lcd.clear();
    lcd.print(0, 0, "Exiting . .");         // Print to APP LCD
  }  
//???  delay(100);
//???  Blynk.virtualWrite(V10, HIGH);          // Dock Button ON
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V13)                          // Manual Forward Motion
{
  ForwardPinState = param.asInt(); 
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V14)                          // Manual Reverse Motion
{
  ReversePinState = param.asInt(); 
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V15)                          // Manual Left Turn
{
  LeftPinState = param.asInt(); 
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V16)                          // Manual Right Turn
{
  RightPinState = param.asInt(); 
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V18)                          // Manual Right Turn
{
  JoystickX = param[0].asInt(); 
  JoystickY = param[1].asInt(); 

  JoystickPinState = 0;
  if (JoystickX != 256 || JoystickY != 256)
    JoystickPinState = 1;
}
//----------------------------------------------------------------------------------------------

BLYNK_WRITE(V4)
{
  switch (param.asInt())
  {
    case 1:
      { 
        Set_To_AutomaticMode();
        break;
      }
    case 2:
      { 
        Set_To_ManualMode();
        break;
      }
  }
  Update_Blynk_App_With_Status();
}
//----------------------------------------------------------------------------------------------

void Clear_APP()
{
  lcd.clear();

  Blynk.virtualWrite(V0, LOW);  // Start Mowing Buton OFF
  Blynk.virtualWrite(V1, LOW);  // Go-Home Button OFF
  Blynk.virtualWrite(V2, LOW);  // Stop Button OFF
  Blynk.virtualWrite(V10, LOW); // Dock Button OFF

  Blynk.virtualWrite(V3, 0);    // Reset Voltage
  Blynk.virtualWrite(V20, 0);    // Reset Load current
  Blynk.virtualWrite(V21, 0);    // Reset Charge current
  Blynk.virtualWrite(V5, 0);    // Loops
  
  Blynk.virtualWrite(V7, 0);    // Mow LED OFF
  Blynk.virtualWrite(V8, 0);    // Dock LED OFF
  Blynk.virtualWrite(V9, 0);    // TrackingLED OFF
  Blynk.virtualWrite(V11, 0);   // Compass LED OFF
  Blynk.virtualWrite(V12, 0);   // Charging LED OFF
}
//----------------------------------------------------------------------------------------------
