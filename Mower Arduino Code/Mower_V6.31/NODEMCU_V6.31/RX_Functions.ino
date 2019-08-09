//----------------------------------------------------------------------------------------------
// All the recieved information functions

void Receive_All_From_MEGA () 
{
  //Using global variable NodeMCU_RX_Value as receive buffer
  
  while (Serial.available() > 0)  //NodeMCU
  {
    char r = Serial.read(); //NodeMCU

    //Ignore end of line characters (received if other side does "println")
    if (r == '\n' || r == '\r')
      continue;

    //If any other (not special character) received - add it to buffer
    if (r < 'a' || r > 'z') 
    {
      NodeMCU_RX_Value = NodeMCU_RX_Value + (char)r;
      continue;
    }

    //Ignore first "packet", it is not sure that it is received from begining, 
    //only reading first "end of packet" byte confirms that new "packet" will start
    if (NodeMCU_RX_Value[0] == '_') 
    {
      NodeMCU_RX_Value = "";
      continue;
    }

    switch (r)
    {
      case 'g': BatteryVoltage    = NodeMCU_RX_Value.toFloat();
                break;
      case 'c': Loop_Cycle_Mowing = NodeMCU_RX_Value.toInt();
                break;
      case 'd': Mower_Docked      = NodeMCU_RX_Value.toInt();
                break;
      case 'z': Mower_Running     = NodeMCU_RX_Value.toInt();
                break;
      case 'y': Mower_Parked      = NodeMCU_RX_Value.toInt();
                break;
      case 'o': Charge_Detected   = NodeMCU_RX_Value.toInt();
                break;
      case 'm': Tracking_Wire     = NodeMCU_RX_Value.toInt();
                break;
    }

    //Always clear buffer if "end of packet" received
    NodeMCU_RX_Value = "";
  }
  Print_RX_Values();
  Calculate_Filtered_Mower_Status();
}
//----------------------------------------------------------------------------------------------

void Print_RX_Values() 
{
  Serial.print("Volt:");
  Serial.print(BatteryVoltage);
  Serial.print("|");

  Serial.print("Loop:");
  Serial.print(Loop_Cycle_Mowing);
  Serial.print("|");

  Serial.print("Docked:");
  Serial.print(Mower_Docked);
  Serial.print("|");

  Serial.print("Running:");
  Serial.print(Mower_Running);
  Serial.print("|");

  Serial.print("Parked:");
  Serial.print(Mower_Parked);
  Serial.print("|");

  Serial.print("Charging:");
  Serial.print(Charge_Detected);
  Serial.print("|");

  Serial.print("Tracking:");
  Serial.print(Tracking_Wire);
  Serial.print("|");
}
//----------------------------------------------------------------------------------------------

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
