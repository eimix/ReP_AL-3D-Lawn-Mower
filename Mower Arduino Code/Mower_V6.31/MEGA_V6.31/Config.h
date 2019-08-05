#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG_LEVEL 2 // 0 - None; 1 - Error; 2 - Warning; 3 - Info; 4 - ALL

//Pin setup for Arduino MEGA

//-----------------------------------------------------------------------------------------------------------------------------------
// Motors
//-----------------------------------------------------------------------------------------------------------------------------------

#define WheelMotorDriver_L298N

#define MowMotorDriver_RELAY   // 30A 30VDC
//#define MowMotorDriver_BTS7960 // 43A

#ifdef WheelMotorDriver_L298N
//Pin Setup for the wheel Motor Bridge Controller
//Motor A
#define ENAPin 7                // EN Pins need a digital pin with PWM
#define IN1Pin 6                // IN Pins dont need digital PWM 
#define IN2Pin 5
//Motor B
#define ENBPin 2                // EN Pins need a digital pin with PWM
#define IN3Pin 4                // IN Pins dont need digital PWM
#define IN4Pin 3
#endif

//Motor Blades
#ifdef MowMotorDriver_BTS7960
#define RPWM 8
#define L_EN 9
#define R_EN 10
#endif

#ifdef MowMotorDriver_RELAY
#define MowMotorRelayPin 8
#endif

//Relay Switch
#define Relay_Motors 24         // be careful that you really use PIN24.  The order is sometimes labelled
// so it looks like 24 is actually 22.

//-----------------------------------------------------------------------------------------------------------------------------------
// Membrane Switch
//-----------------------------------------------------------------------------------------------------------------------------------

#define StartKeyPin 50 //connect wire 1 to pin 2
#define PlusKeyPin 51  //connect wire 2 to pin 3
#define MinusKeyPin 52 //connect wire 3 to pin 4
#define StopKeyPin 53  //connect wire 4 to pin 5

//-----------------------------------------------------------------------------------------------------------------------------------
// Sonar
//-----------------------------------------------------------------------------------------------------------------------------------

//Sonar Setup for Front Sonars 1-3
#define echoPin1 34   //S1               
#define trigPin1 35
#define echoPin2 36   //S2 
#define trigPin2 37
#define echoPin3 38   //S3
#define trigPin3 39

#define EchoTimeout 6000  //5882uS = ~1m distance

//-----------------------------------------------------------------------------------------------------------------------------------
// Sensors
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseLocalSensors
//#define UseRemoteSensors //This is no needen any more, remove this option and code after confirmation

#ifdef UseLocalSensors
#define pinBatteryVoltage A0        // pin normally A0
#define pinChargeCurrent  A1        // pin normally A1
#define pinLoadCurrent    A2        // pin normally A2

#define pinRainSensor     A3        // pin normally A3
#endif

//Voltage and Current Sensor Settings
#define VoltageDeviderR1 6200
#define VoltageDeviderR2 62000

#define CurrentmVperA 100           //100mV/A
#define CurrentOffset 2500          //sensor -20A~20A 

//Battery Settings
#define Battery_Max              29.4                       // Max battery volts in Volts. 7S 4.2V/cell
#define Battery_Min              25.2                       // Lower Limit of battery charge before re-charge required. 3.6V/cell
#define  MaxLowBatteryCount        14                       // Instances of low battery detected before a re-charge is called..

//-----------------------------------------------------------------------------------------------------------------------------------
// Lift sensors
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseBumperSensors

#ifdef UseBumperSensors
#define BumperFrontLeftPin  42
#define BumperFrontRightPin 43
#endif

//-----------------------------------------------------------------------------------------------------------------------------------
// Lift sensors
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseLiftSensors

#ifdef UseLiftSensors
#define LiftFrontLeftPin  40
#define LiftFrontRightPin 41
#endif

//-----------------------------------------------------------------------------------------------------------------------------------
// Perimeter
//-----------------------------------------------------------------------------------------------------------------------------------

//Perimeter Wire Pins
#define pinPerimeterLeft  A5       // perimeter pin normally A5
#define pinPerimeterRight A4       // leave open
#define pinLED LED_BUILTIN

//-----------------------------------------------------------------------------------------------------------------------------------
// WiFi
//-----------------------------------------------------------------------------------------------------------------------------------

#define UseWiFi

//-----------------------------------------------------------------------------------------------------------------------------------
// Clock
//-----------------------------------------------------------------------------------------------------------------------------------

//Real Time Clock Pins
const int kCePin   = 29;  // RST
const int kIoPin   = 30;  // DAT
const int kSclkPin = 31;  // CLK

DS1302 rtc(kCePin, kIoPin, kSclkPin);

//-----------------------------------------------------------------------------------------------------------------------------------
// Error check
//-----------------------------------------------------------------------------------------------------------------------------------

#if defined(UseRemoteSensors) && defined(UseLocalSensors)
#error Only one type of sensors should be used
#endif

#if !defined(UseRemoteSensors) && !defined(UseLocalSensors)
#error At least one type of sensors should be used
#endif
//-----------------------------------------------------------------------------------------------------------------------------------

#if defined(MowMotorDriver_RELAY) && defined(MowMotorDriver_BTS7960)
#error Only one type of mow motor driver should be used
#endif

#if !defined(MowMotorDriver_RELAY) && !defined(MowMotorDriver_BTS7960)
#error At least one type of mow motor driver should be used
#endif
//-----------------------------------------------------------------------------------------------------------------------------------

#endif
