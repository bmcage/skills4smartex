/** 
 *  Code to read out pressure value of smartshoe
 */


/** 
 *  BEGIN CONFIGURATION
 */
// Pin layout
const int Dig_Pressure_LOW = D5;
const int Dig_Pressure_HIGH = D3;  //D7 in other project
const int Pressure_Sensor = A0;

// Every xx ms write out to serial measurement
const int WRITE2SERIAL_MS = 10;

#define SERIALTESTOUTPUT true     // show test output on serial monitor at 115200 baud
#define SERIAL_SPEED     57600    // set speed for serial connection
/** 
 *  BEGIN WIFI CONFIGURATION
 */

// replace this with your wifi credentials
const bool WPA2_ENTERPRISE_CONNECTION = false;  // false for normal WPA2 connection, true for WPA2 Enterprise
static const char* username = "xxxxxxxx"; // Username for WPA Enterprise authentification

static const char* ssid = "XXX";
static const char* password = "XXX";

bool use_static_IP = false;         //use a static IP address
uint8_t static_IP[4] = {192, 168, 1, 42}; 
uint8_t static_gateway_IP[4] = {192, 168, 1, 1};// if you want to use static IP make sure you know what the gateway IP is, here 192.168.1.1

//NTP settings
//#define NTP_ADDRESS  "raspberrypi.local"  // change this to whatever pool is closest (see ntp.org)
//#define NTP_ADDRESS  "192.168.1.15"       // eth0 address of the raspberry pi Ingegno Maker Space
#define NTP_ADDRESS "pool.ntp.org"          // a pool on the internet

//dummy matt server/broker needed for wifilib.h
//const char* mqtt_server = "broker.mqtt-dashboard.com";
//const char* mqtt_server = "raspberrypi.local";
const char* mqtt_server = "192.168.1.24";  //eth0 address of the raspberry pi - Ingegno

/** 
 *  BEGIN VL53L0X DIST SENSOR CONFIGURATION
 */
 
// install https://github.com/adafruit/Adafruit_VL53L0X
// via manage libraries in Arduino IDE

// true or false to indicate if dist sensor is present
const bool VL53L0X_CONNECTED = true;
const int VL53L0X_SDA_PIN = D2;
const int VL53L0X_SCL_PIN = D1;

/** 
 *  END CONFIGURATION
 */


//wifi and timing lib
#include "wifilib.h"

WiFiClient client;

//distance sensor
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X dist_sens = Adafruit_VL53L0X();

int PressureADC;
VL53L0X_RangingMeasurementData_t dist_measure;
int i;

int ard_effect0_status = -1;
unsigned long ard_effect0_start, ard_effect0_time;
#define EFFECT0_PERIOD WRITE2SERIAL_MS

// Deze functie beschrijven...
void WriteSerial() {
  //Variables of this effect are reffered to with ard_effect0
  boolean restart = false;
  ard_effect0_time = millis() - ard_effect0_start;
  if (ard_effect0_time > EFFECT0_PERIOD) {
    //end effect, make sure it restarts
    if (ard_effect0_status > -1) {
    }
    restart = true;
    ard_effect0_status = -1;
    ard_effect0_start = ard_effect0_start + ard_effect0_time;
    ard_effect0_time = 0;
  }
  if (not restart && ard_effect0_status == -1) {
    ard_effect0_status = 0;
    ard_effect0_start = ard_effect0_start + ard_effect0_time;
    ard_effect0_time = 0;
    determine_localtimenow();
    Serial.print(millis()); Serial.print(F(", "));
    Serial.print(PressureADC); Serial.print(F(", "));
    Serial.print(localtimenow);
    if (VL53L0X_CONNECTED) {
      dist_sens.rangingTest(&dist_measure, false); // pass in 'true' to get debug data printout!
      Serial.print(F(", "));
      if (dist_measure.RangeStatus != 4) {
        Serial.print(dist_measure.RangeMilliMeter);
      } else {
        Serial.print(-1);
      }
    }
    Serial.println(F(", "));
  }
}

void WriteHeaderSerial(){
  // csv output on serial
  Serial.print(F("time, Measured Pressure Val [0,1023], NTP Time"));
  if (VL53L0X_CONNECTED) {
    Serial.print(F(", "));
    Serial.print(F("Distance (mm)"));
  }
  Serial.println(F(", "));
}

void setup() {
  pinMode(Dig_Pressure_LOW, OUTPUT);
  pinMode(Dig_Pressure_HIGH, OUTPUT);
  pinMode(Pressure_Sensor, INPUT);
  Serial.begin(SERIAL_SPEED);
  // wait until serial port opens for native USB devices
  //while (! Serial) {
  //  delay(1);
  //}
  
  ard_effect0_status = -1;
  ard_effect0_start = millis();

  PressureADC = 0;
  digitalWrite(Dig_Pressure_LOW, LOW);
  digitalWrite(Dig_Pressure_HIGH, HIGH);

  //setup distance sensor
  if (VL53L0X_CONNECTED) {
    Wire.begin(VL53L0X_SDA_PIN, VL53L0X_SCL_PIN);
    Serial.println(F("Adafruit VL53L0X test"));
    if (!dist_sens.begin()) {
      Serial.println(F("Failed to boot VL53L0X"));
      while(1);
    }
  }
  //connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
   setupWiFi(true); // Connect to local Wifi
  }
  ip = WiFi.localIP();
  obtainedwifi = true;

  // connect to NTP 
  timeClient.begin();   // Start the NTP UDP client
  delay(1000);
  obtainDateTime();
  NTPstartTijd = millis();
  currentTime = NTPstartTijd;
  
  WriteHeaderSerial();
}

void loop() {
  PressureADC = analogRead(Pressure_Sensor);
  WriteSerial();
  
}
