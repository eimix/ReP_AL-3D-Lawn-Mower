//---------------------------------------------------------------------------------------

#ifdef UseRemoteSensors
String Serial1_RX = "_";

void Read_Serial1_Nano()
{
  while (Serial1.available() > 0)
  {
    char r = Serial1.read();

    //Ignore end of line characters (received if other side does "println")
    if (r == '\n' || r == '\r')
      continue;

    //If any other (not special character) received - add it to buffer
    if (r < 'a' || r > 'z') 
    {
      Serial1_RX = Serial1_RX + (char)r;
      continue;
    }

    //Ignore first "packet", it is not sure that it is received from begining, 
    //only reading first "end of packet" byte confirms that new "packet" will start
    if (Serial1_RX[0] == '_') 
    {
      Serial1_RX = "";
      continue;
    }

    switch (r)
    {
      case 'q': RawValueAmp   = Serial1_RX.toInt();
                break;
      case 'j': RawValueVolt  = Serial1_RX.toInt();
                break;
      case 'w': Rain_Detected = Serial1_RX.toInt();
                break;
    }
    
    Serial1_RX = "";
  }

  Calculate_Volt_Amp_Charge();
}
#endif
//---------------------------------------------------------------------------------------
