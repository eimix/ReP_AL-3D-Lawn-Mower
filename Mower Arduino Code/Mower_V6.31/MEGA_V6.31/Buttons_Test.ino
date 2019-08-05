//-----------------------------------------------------------------------------------------------------------------------------------
// BUTTONS TEST MENU

//-----------------------------------------------------------------------------------------------------------------------------------

// Test to displyed on the LCD screen when using the membrane key menus
void Print_LCD_Menu_Tests(byte LCD_Menu_Tests)
{
  if (LCD_Menu_Tests == 1) lcd.print(F("Wire Test"));
  if (LCD_Menu_Tests == 2) lcd.print(F("Relay Test"));
  if (LCD_Menu_Tests == 3) lcd.print(F("Wheel Test"));
  if (LCD_Menu_Tests == 4) lcd.print(F("Blade Test"));
  if (LCD_Menu_Tests == 5) lcd.print(F("Sonar Test"));
  if (LCD_Menu_Tests == 6) lcd.print(F("Turn Test"));
  if (LCD_Menu_Tests == 7) lcd.print(F("Volt Amp Test"));
  if (LCD_Menu_Tests == 8) lcd.print(F("Compass Test"));
  if (LCD_Menu_Tests == 9) lcd.print(F("Spare 9"));
  if (LCD_Menu_Tests == 10) lcd.print(F("Spare 10"));
  if (LCD_Menu_Tests == 11) lcd.print(F("Spare 11"));
  if (LCD_Menu_Tests >= 12) lcd.print("");   // Leave Blank
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Print_Membrane_Switch_Input_Tests()
{
  //Menu Options if the Mower is Tests.
  Read_Membrane_Keys();
  Menu_Complete = 1;
  Menu_Mode_Selection = 0;
  Menu_View = 0;

  Serial.println();
  Serial.println(F("Test Menu Activated"));
  Menu_Complete = false;                                // Menu complete will return to the normal loop
  lcd.clear();
  delay(5);

  while (Menu_Complete == false)                        // holds the program in a loop until a selection has been made in the membrane button menu
  {
    if (Menu_View == 0)
    {
      lcd.setCursor(2, 0);
      Print_LCD_Menu_Tests(1);
      lcd.setCursor(2, 1);
      Print_LCD_Menu_Tests(2);
    }
    // Gets the values again from the keys
    Read_Membrane_Keys();
    delay(100);

    if (!Start_Key_X)
    {
      Menu_Complete = true;
      Serial.println(F("Start key is pressed"));
      lcd.clear();
    }
    if (!Plus_Key_X)
    {
      Serial.println(F("+ key is pressed"));
      Menu_View = Menu_View - 1;
      Run_Menu_Order_Testing();
    }
    if (!Minus_Key_X)
    {
      Menu_View = Menu_View + 1;
      Run_Menu_Order_Testing();
    }
    if (!Stop_Key_X)
    {
      Serial.println(F("Stop key is pressed"));
      Menu_Complete = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Menu Cancelled"));
      delay(1000);
      lcd.clear();
      Menu_Mode_Selection = 0;
    }
  }
  Activate_Menu_Option_Testing();
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Run_Menu_Order_Testing()
{
  Serial.print(F("- key is pressed "));
  lcd.clear();
  lcd.setCursor(2, 0);
  Print_LCD_Menu_Tests(Menu_View);
  lcd.setCursor(2, 1);
  Print_LCD_Menu_Tests(Menu_View + 1);
  lcd.setCursor(0, 0);
  lcd.print(">");
  Menu_Mode_Selection = Menu_View;
  Serial.print(F("Menu View : "));
  Serial.print(Menu_View);
  Serial.print(F("| Menu Selection"));
  Serial.println(Menu_Mode_Selection);
  delay(100);

  /*
            if (Menu_View == 1) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(1);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(2);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 1;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 2) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(2);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(3);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 2;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 3) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(3);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(4);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 3;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 4) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(4);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(5);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 4;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 5) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(5);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(6);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 5;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 6) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(6);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(7);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 6;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 7) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(7);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(8);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 7;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 8) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(8);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(9);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 8;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 9) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(9);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(10);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 9;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 10) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(10);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(11);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 10;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }
            if (Menu_View == 11) {
               Serial.print(F("- key is pressed "));
               lcd.clear();
               lcd.setCursor(2,0);
               Print_LCD_Menu_Tests(11);
               lcd.setCursor(2,1);
               Print_LCD_Menu_Tests(12);
               lcd.setCursor(0,0);
               lcd.print(">");
               Menu_Mode_Selection = 10;
               Serial.print(F("Menu View : "));
               Serial.print(Menu_View);
               Serial.print(F("| Menu Selection"));
               Serial.println(Menu_Mode_Selection);
               delay(100);
               }


        delay(100);
  */
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Activate_Menu_Option_Testing()
{
  if (Menu_Mode_Selection == 1)
  {
    // Perimeter Wire Test
    lcd.clear();
    lcd.print(F("Wire Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    Serial.println(F("Perimeter Wire Test Started"));
    delay(5000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Test_Mower_Check_Wire();
      Read_Membrane_Keys();
      if (!Stop_Key_X)
      {
        Serial.println(F("Stop key is pressed"));
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Test Stopped"));
        delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
    }
  }

  if (Menu_Mode_Selection == 2)
  {
    // Relay Test
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Relay Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    delay(1000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Test_Relay();
      Read_Membrane_Keys();
      if (!Stop_Key_X)
      {
        Serial.println(F("Stop key is pressed"));
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Test Stopped"));
        delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
    }
  }

  if (Menu_Mode_Selection == 3)
  {
    // Mower Motor Test
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Wheel Motor Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    delay(1000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Perimeter_I = 1;                       // sets the itteration for the motor test
    Test_Wheel_Motors();         // starts the mowe motor test 1x
  }

  if (Menu_Mode_Selection == 4)
  {
    // Blade Motor Test
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Blade Motor Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    delay(1000);
    lcd.clear();
    Menu_Mode_Selection = 0;
    lcd.clear();
    Test_Mower_Blade_Motor();
  }

  if (Menu_Mode_Selection == 5)
  {
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Sonar Test"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activated"));
    delay(1000);
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      Test_Sonar_Array();
      Read_Membrane_Keys();
      if (!Stop_Key_X)
      {
        Serial.println(F("Stop key is pressed"));
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Test Stopped"));
        delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
    }
  }

  if (Menu_Mode_Selection == 6)
  {
    lcd.clear();
    lcd.print(F("Pattern Mow"));
    Serial.println(F("Slot 6 Selected"));
    Menu_Mode_Selection = 0;
    delay(3000);
    lcd.clear();
    Test_Compass_Turn_Function();
  }

  if (Menu_Mode_Selection == 7)
  {
    lcd.clear();
    lcd.print(F("Volt Amps Test"));
    Serial.println(F("Volts and Amps Test"));
    Menu_Mode_Selection = 0;
    delay(1000);
    lcd.clear();
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
#ifdef UseRemoteSensors
      Read_Serial1_Nano();                                                                    // Read the Serial data from the nano
#endif
#ifdef UseLocalSensors
      Loop_Sensors();                                                                         // Read sensors data
#endif

      delay(100);
      lcd.setCursor(0, 0);
      lcd.print("Volt:");
      lcd.print(BatteryVoltage);
      lcd.setCursor(0, 1);
      lcd.print("Amps:");
      lcd.print(ChargeCurrent);
      Serial.println("");

      if (!Stop_Key_X)
      {
        Serial.println(F("Stop key is pressed"));
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Test Stopped"));
        delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
    }
  }

  if (Menu_Mode_Selection == 8)
  {
    lcd.clear();
    lcd.print(F("Compass Test"));
    Serial.println(F("Compass Test Selected"));
    Menu_Mode_Selection = 0;
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Heading:"));
    lcd.setCursor(0, 1);
    lcd.print(F("Degrees:"));
    Menu_Complete = false;
    while (Menu_Complete == false)
    {
      // insert Test Code Here
      Get_Compass_Reading();
      Serial.print(F("Comp H:"));
      Serial.print(Heading);
      Serial.print("|");
      Serial.println("");
      lcd.setCursor(9, 0);
      lcd.print(Heading);
      lcd.setCursor(9, 1);
      lcd.print(Compass_Heading_Degrees);
      delay(5);

      if (!Stop_Key_X)
      {
        Serial.println(F("Stop key is pressed"));
        Menu_Complete = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Test Stopped"));
        delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
    }
  }

  if (Menu_Mode_Selection == 9)
  {
    lcd.clear();
    lcd.print(F("Spare 9"));
    Serial.println(F("Slot 9 Selected"));
    Menu_Mode_Selection = 0;
    delay(3000);
    lcd.clear();
  }

  if (Menu_Mode_Selection == 10)
  {
    lcd.clear();
    lcd.print(F("Spare 10"));
    Serial.println(F("Slot 10 Selected"));
    Menu_Mode_Selection = 0;
    delay(3000);
    lcd.clear();
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------
