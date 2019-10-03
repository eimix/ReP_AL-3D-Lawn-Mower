//----------------------------------------------------------------------------------------------
// All the recieved information functions

void Receive_All_From_MEGA () 
{
  bool DataReceived = false;
  
  while (SerialToAtmega.available() > 0)
  {
    char r = SerialToAtmega.read();

    //Ignore end of line characters (received if other side does "println")
    if (r == '\n' || r == '\r')
      continue;

    //Using global variable RX_Buffer as receive buffer
    //If any other (not special character) received - add it to buffer
    if (r < 'a' || r > 'z') 
    {
      RX_Buffer = RX_Buffer + (char)r;
      continue;
    }

    //Ignore first "packet", it is not sure that it is received from begining, 
    //only reading first "end of packet" byte confirms that new "packet" will start
    if (RX_Buffer[0] == '_') 
    {
      RX_Buffer = "";
      continue;
    }

    switch (r)
    {
      case 'g': RX_BatteryVoltage    = RX_Buffer.toFloat();
                break;
      case 'a': RX_LoadCurrent       = RX_Buffer.toFloat();
                break;
      case 'b': RX_ChargeCurrent     = RX_Buffer.toFloat();
                break;
      case 'c': RX_LoopCycleMowing   = RX_Buffer.toInt();
                break;
      case 'd': RX_MowerDocked       = RX_Buffer.toInt();
                break;
      case 'z': RX_MowerRunning      = RX_Buffer.toInt();
                break;
      case 'y': RX_MowerParked       = RX_Buffer.toInt();
                break;
      case 'o': RX_MowerCharging     = RX_Buffer.toInt();
                break;
      case 'm': RX_MowerTrackingWire = RX_Buffer.toInt();
                break;
    }

    //Always clear buffer if "end of packet" received
    RX_Buffer = "";
    DataReceived = true;
  }
  
  //Calculate_Filtered_Mower_Status();

  //Print every time data is received
  if (DataReceived)
    Print_RX_Values();
}
//----------------------------------------------------------------------------------------------

void Print_RX_Values() 
{
  Serial.print("Loop:");
  Serial.print(LoopLength);
  Serial.print("|");
   
  Serial.print("Volt:");
  Serial.print(RX_BatteryVoltage);
  Serial.print("|");

  Serial.print("LoadC:");
  Serial.print(RX_LoadCurrent);
  Serial.print("|");

  Serial.print("ChargeC:");
  Serial.print(RX_ChargeCurrent);
  Serial.print("|");

  Serial.print("Loop:");
  Serial.print(RX_LoopCycleMowing);
  Serial.print("|");

  Serial.print("Docked:");
  Serial.print(RX_MowerDocked);
  Serial.print("|");

  Serial.print("Running:");
  Serial.print(RX_MowerRunning);
  Serial.print("|");

  Serial.print("Parked:");
  Serial.print(RX_MowerParked);
  Serial.print("|");

  Serial.print("Charging:");
  Serial.print(RX_MowerCharging);
  Serial.print("|");

  Serial.print("Tracking:");
  Serial.print(RX_MowerTrackingWire);
  Serial.print("|");

  Serial.println("");
}
//----------------------------------------------------------------------------------------------

/*
void Calculate_Filtered_Mower_Status() 
{
  if (Mower_Docked == 1) 
  {
    if (Mower_Docked_Filter < 2) 
      Mower_Docked_Filter++;
  }
  else 
    Mower_Docked_Filter = 0;

  if (Mower_Running == 1) 
  {
    if (Mower_Running_Filter < 2) 
      Mower_Running_Filter++;
  }
  else 
    Mower_Running_Filter = 0;
}
//----------------------------------------------------------------------------------------------
*/
