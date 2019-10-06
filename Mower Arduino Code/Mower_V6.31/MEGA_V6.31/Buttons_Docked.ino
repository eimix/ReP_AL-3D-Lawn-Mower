// BUTTONS DOCKED MENU

//TODO:
//1. Buttons signal debounce

//DONE
//1. Refactoring

//-----------------------------------------------------------------------------------------------------------------------------------

// Reads each of the membrane keys and detects if a key is pressed.
void Read_Membrane_Keys()
{
  Start_Key_X = digitalRead(StartKeyPin);
  Plus_Key_X  = digitalRead(PlusKeyPin);
  Minus_Key_X = digitalRead(MinusKeyPin);
  Stop_Key_X  = digitalRead(StopKeyPin);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Check_Membrane_Keys_Running()
{
  Read_Membrane_Keys();
  if (!Stop_Key_X)
  {
    Manouver_Park_The_Mower();
    Check_Membrane_Switch_Input_Parked();
    Menu_Mode_Selection = 0;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

// Print a string from Program Memory directly to save RAM 
void LCDPrintProgStr(const char *str)
{
  char c;
  if (!str) 
    return;
  while (c = pgm_read_byte(str++))
    lcd.print(c);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Print_LCD_Menu(const char *MenuItems, byte Item)
{
  byte i = max(Item, 1);

  lcd.clear();
  lcd.setCursor(2, 0);
  LCDPrintProgStr(MenuItems + (i - 1) * MAX_SIZE);
  //If not the last element was printed, print second row
  if (i < NUMBER_OF_ELEMENTS)
  {
    lcd.setCursor(2, 1);
    LCDPrintProgStr(MenuItems + i * MAX_SIZE);
  }
  if (Item > 0)
  {
    lcd.setCursor(0, 0);
    lcd.print(">");
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Run_Menu_Order(const char *MenuItems)
{
  Print_LCD_Menu(MenuItems, Menu_View);

  Menu_Mode_Selection = Menu_View;
#if (DEBUG_LEVEL >= 3)
  Serial.print(F("Menu View : "));
  Serial.print(Menu_View);
  Serial.print(F("| Menu Selection :"));
  Serial.println(Menu_Mode_Selection);
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------

//Global menu function that calls two custom menu functions
void Print_Membrane_Switch_Input(const char *Name, const char *MenuItems, void (*Activate_Menu_Option)())
{
  //  Read_Membrane_Keys();
  //  Menu_Complete       = 1;
  Menu_Mode_Selection = 0; //? why this?
  Menu_View           = 0; // Menu items are 1-11, but 0 - none selected

#if (DEBUG_LEVEL >= 3)
  Serial.println();
  Serial.print(Name);
  Serial.println(F(" Menu Activated"));
#endif
  Menu_Complete = false;                                // Menu complete will return to the normal loop

  Run_Menu_Order(MenuItems);

  while (Menu_Complete == false)                       // holds the program in a loop until a selection has been made in the membrane button menu
  {
    // Gets the values again from the keys
    Read_Membrane_Keys();
    lawn_delay(ButtonPressDelay);

    if (!Start_Key_X)
    {
#if (DEBUG_LEVEL >= 4)
      Serial.println(F("Start key is pressed"));
#endif
      Menu_Complete = true;
      lcd.clear();
      (*Activate_Menu_Option)();
    }
    if (!Plus_Key_X)
    {
#if (DEBUG_LEVEL >= 4)
      Serial.println(F("+ key is pressed"));
#endif
      Menu_View--;
      Menu_View = max(Menu_View, 1);
      Run_Menu_Order(MenuItems);
    }
    if (!Minus_Key_X)
    {
#if (DEBUG_LEVEL >= 4)
      Serial.print(F("- key is pressed "));
#endif
      Menu_View++;
      Menu_View = min(Menu_View, NUMBER_OF_ELEMENTS);  
      Run_Menu_Order(MenuItems);
    }
    if (!Stop_Key_X)
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println(F("Stop key is pressed"));
#endif
      Menu_Complete = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Menu Cancelled"));
      lawn_delay(1000);
      lcd.clear();
      Menu_Mode_Selection = 0;
    }
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Check_Membrane_Switch_Input_Docked()
{
  //Menu Options if the Mower is Docked
  Read_Membrane_Keys();
  Menu_Complete       = 1;
  Menu_Mode_Selection = 0;
  Menu_View           = 0;

  if (!Start_Key_X)                                      // If the start key is pressed
  {
    lawn_delay(ButtonPressDelay);
#if (DEBUG_LEVEL >= 3)
    Serial.println();
    Serial.println(F("Start Key Pressed"));
#endif
    Print_Membrane_Switch_Input("Docked", (const char *)&DockedMenuItems, Activate_Menu_Option_Docked);  
//    Print_Membrane_Switch_Input_Docked();
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------

// Defines the actions when that option is selected with the keypad.
void Activate_Menu_Option_Docked()
{
  if (Menu_Mode_Selection == 1)
  {
    // Exit the mower from the Garage and go to Zone 1;
    lcd.clear();
    lcd.print(F("Manual Start"));
    lcd.setCursor(0, 1);
    lcd.print(F("Exit Dock Z1"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Exit to Zone 1 - Free Mow"));
#endif
    lawn_delay(1000);
    lcd.clear();
    Print_Membrane_Switch_Input_Timing();
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    if (Mow_Time_Set == 1)
    {
      //Enter here the code to go to zone 1 from dock
      Exit_Zone = 1;
      Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
      Manouver_Exit_To_Zone_X();
    }
  }

  if (Menu_Mode_Selection == 2)
  {
    // Exit the mower from the Garage and go to Zone 2;
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.print(F("Manual Start"));
    lcd.setCursor(0, 1);
    lcd.print(F("Exit Dock Z2"));
    lawn_delay(1000);
    lcd.clear();
    Print_Membrane_Switch_Input_Timing();                            // Changes the menu to select the mow time
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    if (Mow_Time_Set == 1)
    {
      Exit_Zone = 2;
      Track_Wire_Itterations = Track_Wire_Zone_2_Cycles;
      Manouver_Exit_To_Zone_X();
    }
  }

  if (Menu_Mode_Selection == 3)
  {
    // Quick Start the Mower in the middle of the Garden;
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Quick Start Selected"));
#endif
    Print_Membrane_Switch_Input_Timing();                             // Changes the menu to select the mow time
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    if (Mow_Time_Set == 1)
      Manouver_Start_Mower();
    lcd.clear();
  }

  if (Menu_Mode_Selection == 4)
  {
    lcd.clear();
    lcd.print(F("Trampoline Cut!"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Mower Set to Cut under Trampoline"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    Special_Cut_Under_Trampoline_Function();
  }

  if (Menu_Mode_Selection == 5)
  {
    lcd.clear();
    lcd.print(F("Blade will spin"));
    lcd.setCursor(0, 1);
    lcd.print(F("Mow the Line"));
    lawn_delay(1000);
    lcd.clear();
    Print_Membrane_Switch_Input_Timing();                             // Changes the menu to select the mow time
    if (Mow_Time_Set == 1)
    {
#if (DEBUG_LEVEL >= 3)
      Serial.println(F("Cutting the grass on the boundary wire"));
#endif
      Blade_Override = 1;
      Track_Wire_Itterations = 6000;
      Exit_Zone = 3;
      Manouver_Exit_To_Zone_X();
      Manouver_Start_Mower();                                  // Sets up the mower to go.
    }
  }

  if (Menu_Mode_Selection == 6)
  {
    lcd.clear();
    lcd.print(F("Test Mower Menu"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Test Menu Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    Print_Membrane_Switch_Input("Testing", (const char *)&TestingMenuItems, Activate_Menu_Option_Testing); 
    //Print_Membrane_Switch_Input_Testing();
  }

  if (Menu_Mode_Selection == 7)
  {
    lcd.clear();
    lcd.print(F("Mower Setup"));
#if (DEBUG_LEVEL >= 3)
    Serial.println(F("Mower Setup Selected"));
#endif
    Menu_Mode_Selection = 0;
    lawn_delay(1000);
    lcd.clear();
    Print_Membrane_Switch_Input("Settings", (const char *)&SettingsMenuItems, Activate_Menu_Option_Settings); 
    //Print_Membrane_Switch_Input_Settings();
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------
