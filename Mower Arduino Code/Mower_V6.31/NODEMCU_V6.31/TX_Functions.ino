//----------------------------------------------------------------------------------------------
// All the Transmitted informtion functions

//----------------------------------------------------------------------------------------------

void Transmit_Blynk_Data_to_Mega()
{
  int code = transmit_blynk_code + 10;           // Transmits the code and adds 10 to the number
  NodeMCU.print(code);
  NodeMCU.println("p");
  delay(30);
  Serial.print("TX_Code_to_MEGA:");
  Serial.print(code);
  Serial.print("");
  delay(20);
}
//----------------------------------------------------------------------------------------------
