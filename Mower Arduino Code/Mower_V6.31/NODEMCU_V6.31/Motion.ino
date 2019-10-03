//----------------------------------------------------------------------------------------------
// Send the various WIFI codes to the MEGA

//----------------------------------------------------------------------------------------------

void Pause_Mower()
{
  Serial.println("Pause/Stop");
  Transmit_Blynk_Data_to_Mega(1);
}
//----------------------------------------------------------------------------------------------

void Going_Home()
{
  Serial.println("Go to Dock");
  Transmit_Blynk_Data_to_Mega(2);
}
//----------------------------------------------------------------------------------------------

void StartMower()
{
  Serial.println("Quick Start");
//???  Mower_Parked = 0;
//???  Update_Blynk_App_With_Status();
  Transmit_Blynk_Data_to_Mega(3);              // Transmits that code to the MEGA
}
//----------------------------------------------------------------------------------------------

void Exit_Dock() 
{
  Serial.println("Exit Dock");
//???  Mower_Docked = 0;                           // Forces the APP to show docked as false.
//???  // Otherwise the APP needs to wait for the next update round
//???  Update_Blynk_App_With_Status();
  Transmit_Blynk_Data_to_Mega(4);
}
//----------------------------------------------------------------------------------------------

void Set_To_ManualMode() 
{
  ManualMode    = 1;
  AutomaticMode = 0;
  Serial.println("Manual Mode Selected");
//???  Update_Blynk_App_With_Status();
  Transmit_Blynk_Data_to_Mega(5);

/* TODO XXX These values should be received from Mower
  Mower_Parked   = 0;
  Mower_Docked   = 0;
  Mower_Running  = 0;
*/
}
//----------------------------------------------------------------------------------------------

void Set_To_AutomaticMode() 
{
  AutomaticMode = 1;
  ManualMode    = 0;
  Serial.println("Automatic Mode Selected");
//???  Update_Blynk_App_With_Status();
  Transmit_Blynk_Data_to_Mega(6);
}
//----------------------------------------------------------------------------------------------

void Process_ManualMode() 
{
  if (!ManualMode)
    return;

  long Delay = (long)(millis() - LastTXToAtmega);
  if (Delay < TXDelay)
    return;
  
  if (ForwardPinState)
    Transmit_Blynk_Data_to_Mega(7);  
  if (ReversePinState)
    Transmit_Blynk_Data_to_Mega(8);  
  if (LeftPinState)
    Transmit_Blynk_Data_to_Mega(9);  
  if (RightPinState)
    Transmit_Blynk_Data_to_Mega(10); 

  if (JoystickPinState)   
    Transmit_Blynk_Joystick_Data_to_Mega(11, JoystickX, JoystickY); 

  LastTXToAtmega = millis();  
}
//----------------------------------------------------------------------------------------------
