//TODO:
//1. Why filtering values?

//DONE:
//1. BUG: LCD virtual pin V6 used same as virtual "Parked led", changed to V17
//2. BUG: LED pin (D4) initialization (OUTPUT) bofore setting HIGH
//3. Personal Blynk server use (hostname & port)
//4. Commented unused variables, delete afer review
//5. Report Voltage in any case (if out of range), if it is out of range - need remove BUG that causes such problem
//6. BUG If serial read is started in middle of "packet" - part of value is received: TX 12.6V\g RX 2.6V\g  -> VoltNow = 2.6V 
//7. Removed unrelated comments (probably from other project)
//8. Strange use of escaped characters (\g \c \d \z \y \o \m),
//   https://en.wikipedia.org/wiki/Escape_sequences_in_C does not list any of them
//   escaping these characters does nothing, and standard chars should be use (g c d z y o m)

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeMCU(D2, D3);  //RXD2 TXD3
String NodeMCU_RX_Value = "_";

#define LED D4      // on various NODEMCU boards the LED Pin is different

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Blynk Auth Code";

// If using personal BLYNK server (free for personal use)
// set blynk_host to your hostname and enjoy unlimited and any type of widgets, gauges etc.
// set blynk_host to "" for default blynk.cc cloud server
char blynk_host[] = "";            // "my.blynk.host.name.ok"
int blynk_port = 8080;             // default 8080

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WLAN Name";
char pass[] = "WLAN Password";

//int buttonState = 0;
//char  Version[16];

// RX Variables
//float val_VoltNow;                  // Battery Voltage from MEGA
float BatteryVoltage;               // Voltage read last RX
int Loop_Cycle_Mowing;              // Current Loop Cycle from MEGA

int Mower_Parked;
//int Mower_Parked_Low_Batt;
int Mower_Docked;
int Mower_Docked_Filter;
int Mower_Running;
int Mower_Running_Filter;
//int Mower_Lost;
int Charge_Detected;
int Tracking_Wire;
//int Compass_Heading_Lock;

//int i;                              // Transmit blank code

//bool Mower_Already_Started = 0;
//bool Going_Home_Already = 0;

//int j;
//int k;
int transmit_blynk_code = 0;

bool Manuel_Mode;
bool Automatic_Mode;

//int loopstatus = 0;
//int All;
//int RX_Data_Recieved;
//int lastConnectionAttempt = millis();
//int connectionDelay = 5000; // try to reconnect every 5 seconds

BlynkTimer timer;
WidgetLCD lcd(V17);

//----------------------------------------------------------------------------------------------
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

void myTimerEvent()  
{
  Blynk.virtualWrite(V3, BatteryVoltage);
// TEST
//  Blynk.virtualWrite(V3, (millis() % 9990) / 1000.0);

  Blynk.virtualWrite(V5, Loop_Cycle_Mowing);
}
//----------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
  NodeMCU.begin(9600);

  Serial.println("Setting up NODEMCU");
  Serial.println("");

  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, HIGH);                          // Turn off LED Light

  WIFI_Connect();                                   // Connect to the WIFI
  Clear_APP();
  lcd.clear();
}
//----------------------------------------------------------------------------------------------

void WIFI_Connect() 
{
  long mytimeout = millis() / 1000;
  Serial.println();
  Serial.println("*** WiFi ***");

  if (blynk_host == "")
    Blynk.begin(auth, ssid, pass);
  else
    Blynk.begin(auth, ssid, pass, blynk_host, blynk_port);

  while (Blynk.connected() != WL_CONNECTED)
  {
    delay(500);
    if (millis() / 1000 > mytimeout + 3)                     // try for less than 4 seconds to connect to WiFi router
      break;
  }

  if (!Blynk.connected()) 
  {
    digitalWrite(LED, HIGH);
    Serial.println("NODEMCU Disconnected");
    Serial.println("Reconnecting . . . ");
    WIFI_Connect();
  }
  else 
  {
    digitalWrite(LED, LOW);
    Serial.println("Connected.");
    // Setup a function to be called every second
    timer.setInterval(1000L, myTimerEvent);
  }
}
//----------------------------------------------------------------------------------------------

void loop()  
{
//  loopstatus++;
//  RX_Data_Recieved = 0;                         // resets the data received bool

  if (!Blynk.connected()) 
  {
    Blynk.run();
    timer.run();
    digitalWrite(LED, HIGH);
    Serial.println("NODEMCU Disconnected");
    Serial.println("Reconnecting . . . ");
    WIFI_Connect();
  }
  else 
  {
    Blynk.run();
    timer.run();
    digitalWrite(LED, LOW);         //LED is inverted on a MODEMCU
    Receive_All_From_MEGA();
    Update_Blynk_App_With_Status();
    Serial.println("");     // new line serial monitor
  }
}
//----------------------------------------------------------------------------------------------
