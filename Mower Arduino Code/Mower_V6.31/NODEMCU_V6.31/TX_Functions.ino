//----------------------------------------------------------------------------------------------
// All the Transmitted informtion functions

//----------------------------------------------------------------------------------------------

void Transmit_Blynk_Data_to_Mega(int code)
{
  if (code < 0)
    return;
    
  SerialToAtmega.print(code + 10);
  SerialToAtmega.println("p");

  Serial.print("TX to MEGA:");
  Serial.println(code);
}
//----------------------------------------------------------------------------------------------

void Transmit_Blynk_Joystick_Data_to_Mega(int code, int JoystickX, int JoystickY) 
{
  if (code < 0)
    return;
    
  SerialToAtmega.print(code + 10);
  SerialToAtmega.print("|");
  SerialToAtmega.print(JoystickX);
  SerialToAtmega.print("|");
  SerialToAtmega.print(JoystickY);
  SerialToAtmega.println("j");

  Serial.print("TX to MEGA:");
  Serial.print(code + 10);
  Serial.print("|");
  Serial.print(JoystickX);
  Serial.print("|");
  Serial.println(JoystickY);
}
//----------------------------------------------------------------------------------------------
