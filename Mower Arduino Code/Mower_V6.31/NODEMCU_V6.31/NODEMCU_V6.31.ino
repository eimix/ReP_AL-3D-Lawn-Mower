//TODO:
// 1. WiFi manager for multiple SSIDs

//DONE:
// 1. BUG: LCD virtual pin V6 used same as virtual "Parked led", changed to V17
// 2. BUG: LED pin (D4) initialization (OUTPUT) bofore setting HIGH
// 3. Personal Blynk server use (hostname & port)
// 4. Commented unused variables, delete afer review
// 5. Report Voltage in any case (if out of range), if it is out of range - need remove BUG that causes such problem
// 6. BUG If serial read is started in middle of "packet" - part of value is received: TX 12.6V\g RX 2.6V\g  -> VoltNow = 2.6V
// 7. Removed unrelated comments (probably from other project)
// 8. Strange use of escaped characters (\g \c \d \z \y \o \m),
//    https://en.wikipedia.org/wiki/Escape_sequences_in_C does not list any of them
//    escaping these characters does nothing, and standard chars should be use (g c d z y o m)
// 9. BUG: Manual mode comparison used "=" instead of "==" and always enabled manual mode if any movement button was pressed
//10. Pressed button repeat sending movement code (FW, RV, LF, RH)
//11. Why filtering values? Strange behaviour with startup syncALL.
//12. Do not flood with TX commands, transmit with some delay


/* Comment this out to disable prints and save space */
#define Version "1.0_20190905"

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

//#define SWSerialRX D5    //Wemos D1 mini pro
//#define SWSerialTX D6    //Wemos D1 mini pro
#define SWSerialRX D2    //NodeMCU 1.0
#define SWSerialTX D3    //NodeMCU 1.0
SoftwareSerial SerialToAtmega(SWSerialRX, SWSerialTX);  //RXD2 TXD3
String RX_Buffer = "_";

#define LED D4                        // on various NODEMCU boards the LED Pin is different

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "0asdasd5as46d54as6d54as6da54sd";

// If using personal BLYNK server (free for personal use)
// set blynk_host to your hostname and enjoy unlimited and any type of widgets, gauges etc.
// set blynk_host to "" for default blynk.cc cloud server
char blynk_host[] = "192.168.1.253"; // "my.blynk.host.name.ok"
int blynk_port    = 9080;                           // default 8080

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid1[] = "XXX";          //"WLAN Name";
char pass1[] = "AAA";         //"WLAN Password";
char ssid2[] = "XXX";          //"WLAN Name";
char pass2[] = "AAA";         //"WLAN Password";

BlynkTimer Timer;
int TimerID;                          // Used for disable/enable Timer if connection is lost
byte ReconnectCount  = 0;

WidgetLCD lcd(V17);

int StatisticsCycle = 0;

#define TXDelay 300                  // Use same on MEGA to get continuous manual controll - each message is direction for same duaration
unsigned long LastTXToAtmega = 0;    // Last time manual command was transmitted to Atmega

//----------------------------------------------------------------------------------------------
// REQUESTED ACTIONS
//----------------------------------------------------------------------------------------------

byte ForwardPinState  = 0;
byte ReversePinState  = 0;
byte LeftPinState     = 0;
byte RightPinState    = 0;

byte JoystickPinState = 0;
int JoystickX         = 256;
int JoystickY         = 256;

bool ManualMode       = 0;
bool AutomaticMode    = 0;

//----------------------------------------------------------------------------------------------
// ACTUAL DATA
//----------------------------------------------------------------------------------------------

float RX_BatteryVoltage;               // Voltage read last RX
float RX_LoadCurrent;
float RX_ChargeCurrent;
int   RX_LoopCycleMowing;              // Current Loop Cycle from MEGA

//----------------------------------------------------------------------------------------------
// ACTUAL STATES
//----------------------------------------------------------------------------------------------

//int Mower_Parked_Low_Batt;
//int Mower_Docked_Filter;
//int Mower_Running_Filter;
//int Mower_Lost;
//int Compass_Heading_Lock;

//bool Mower_Already_Started = 0;
//bool Going_Home_Already = 0;

int RX_MowerParked       = 0;
int RX_MowerDocked       = 0;
int RX_MowerRunning      = 0;
int RX_MowerCharging     = 0;
int RX_MowerTrackingWire = 0;

//----------------------------------------------------------------------------------------------

unsigned long CurrentTime; 
unsigned long LoopLength; 
unsigned long LastTime;

//----------------------------------------------------------------------------------------------
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

void TimerEvent()
{
  StatisticsCycle++;

  Print_RX_Values();

  if (StatisticsCycle % 3 == 0)
  {
    Blynk.virtualWrite(V3, RX_BatteryVoltage);
    // TEST
    //  Blynk.virtualWrite(V3, (millis() % 9990) / 1000.0);
  }
  if (StatisticsCycle % 3 == 1)
    Blynk.virtualWrite(V20, RX_LoadCurrent);
  if (StatisticsCycle % 3 == 2)
    Blynk.virtualWrite(V21, RX_ChargeCurrent);

  if (StatisticsCycle % 3 == 0)
    Blynk.virtualWrite(V5, RX_LoopCycleMowing);

  if (StatisticsCycle % 5 == 0)
  {
    if (RX_MowerCharging == 4)
      Blynk.virtualWrite(V12, 1023);        // Charging LED ON
    else
      Blynk.virtualWrite(V12, 0);           // Charging LED OFF
  }

  if (StatisticsCycle % 5 == 1)
  {
    if (RX_MowerParked == 1)
      Blynk.virtualWrite(V6, 1023);         // Parked LED
    else
      Blynk.virtualWrite(V6, 0);            // Parked LED
  }

  if (StatisticsCycle % 5 == 2)
  {
    if (RX_MowerDocked == 1)
      Blynk.virtualWrite(V8, 1023);         // Parked LED
    else
      Blynk.virtualWrite(V8, 0);            // Parked LED
  }

  if (StatisticsCycle % 5 == 3)
  {
    if (RX_MowerRunning == 1)
      Blynk.virtualWrite(V7, 1023);         // Parked LED
    else
      Blynk.virtualWrite(V7, 0);            // Parked LED
  }

  if (StatisticsCycle % 5 == 4)
  {
    if (RX_MowerTrackingWire == 1)
      Blynk.virtualWrite(V9, 1023);         // Tracking LED ON
    else
      Blynk.virtualWrite(V9, 0);            // Tracking LED OFF
  }
}
//----------------------------------------------------------------------------------------------

void setup()
{
  pinMode(SWSerialRX, INPUT);
  pinMode(SWSerialTX, OUTPUT);
  pinMode(LED,        OUTPUT);

  Serial.begin(115200);
  SerialToAtmega.begin(9600);

  Serial.println(F("Setting up ESP8266"));
  Serial.println("");

  TimerID = Timer.setInterval(1000L, TimerEvent);

  digitalWrite(LED, HIGH);                            // Turn off LED Light

  do
  {
    WIFI_Connect();                                   // Connect to the WIFI
  } while (!Blynk.connected());

  Clear_APP();
  lcd.clear();
}
//----------------------------------------------------------------------------------------------

void WIFI_Connect()
{
  ReconnectCount++;
  Timer.disable(TimerID);

  unsigned long TimeOut = millis();
  Serial.println();
  Serial.println("ESP8266 Disconnected");
  Serial.println("Reconnecting WiFi . . .");

  if (blynk_host == "")
    Blynk.begin(auth, ssid1, pass1);
  else
    Blynk.begin(auth, ssid1, pass1, blynk_host, blynk_port);

  while (Blynk.connected() != WL_CONNECTED)
  {
    Serial.println("Waiting . . .");
    delay(500);
    if (millis() > TimeOut + 3000)                     // try for less than 4 seconds to connect to WiFi router
      break;
  }

  if (!Blynk.connected())
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
    Serial.println("Connected.");
    // Setup a function to be called every second
    Timer.enable(TimerID);
  }
}
//----------------------------------------------------------------------------------------------

// Calculate the time difference, and account for roll over too
unsigned long calcTimeDiff(unsigned long start, unsigned long end)
{
  // inspired by http://www.arduino.cc/playground/Code/TimingRollover saves 200bytes
  return (long)(end - start);
}
//-----------------------------------------------------------------------------------------------------------------------------------

void loop()
{
  CurrentTime = millis(); 
  LoopLength  = calcTimeDiff(LastTime, CurrentTime); 
  LastTime    = CurrentTime; 

  Blynk.run();
  Timer.run();

  if (!Blynk.connected())
  {
    if (ManualMode)
      Set_To_AutomaticMode();      //Set Automatic mode if connection to WiFi or Blynk server is lost

    digitalWrite(LED, HIGH);
    WIFI_Connect();
  }
  else
  {
    digitalWrite(LED, LOW);         //LED is inverted on a MODEMCU
    Receive_All_From_MEGA();
//    Update_Blynk_App_With_Status();
    Process_ManualMode();
  }
}
//----------------------------------------------------------------------------------------------
