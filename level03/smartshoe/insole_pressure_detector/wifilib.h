
#include <ESP8266WiFi.h>
//WPA2 Enterprise support
extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}
#include <WiFiUdp.h>

#include "myNTPClient.h"
#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>

//MQTT library
#include <PubSubClient.h>

bool personinbed = true;

unsigned long NTPUpdateInterval = 60000 ;
 
unsigned long NTPstartTijd;
unsigned long NTPlastUpdate;
time_t utc, localtimenow=0;
unsigned long currentTime;
unsigned long wifireconnectTime = 0;
unsigned long mqttreconnectTime = 0;
bool correcttimeset = false, firstcorrecttime = false;

IPAddress ip;
//IPAddress  mqtt_ip(mqtt_server_IP[0], mqtt_server_IP[1], mqtt_server_IP[2], mqtt_server_IP[3]);

bool obtainedwifi = false;

// Define NTP properties
#define NTP_OFFSET   60 * 60      // Summer Time ?
//#define NTP_OFFSET   60 * 0       // Winter Time ?
#define NTP_INTERVAL 60 * 1000    // In miliseconds
bool UK_DATE = false;

// Set up the NTP UDP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
//NTPClient timeClient2(ntpUDP, NTP_ADDRESS2, NTP_OFFSET, NTP_INTERVAL);

// Set up the MQTT client
WiFiClient espClient;
PubSubClient MQTTclient(espClient);

String date;
String t;
//const char * days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"} ;
//const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
//const char * ampm[] = {"AM", "PM"} ;
const char * days[] = {"Zo", "Ma", "Di", "Wo", "Do", "Vr", "Za"} ;
const char * months[] = {"Jan", "Feb", "Maa", "Apr", "Mei", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
const char * ampm[] = {"VM", "NM"} ;


// set up the wifi connection. If wait, we wait for the wifi to come up
// indefenitely
void setupWiFi(bool wait)
{
  int nrpoints;
  // Connect to WiFi network
  if (SERIALTESTOUTPUT) {
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  }

  //we only try to set up wifi once every 2 minutes !
  if (wait || (millis() - wifireconnectTime > 2*60000L)) {
    //clean up any old config that are still present
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    if (WPA2_ENTERPRISE_CONNECTION) {
      // WPA2 Connection starts here
      // Setting ESP into STATION mode only (no AP mode or dual mode)
      wifi_set_opmode(STATION_MODE);
      struct station_config wifi_config;
      memset(&wifi_config, 0, sizeof(wifi_config));
      strcpy((char*)wifi_config.ssid, ssid);
      wifi_station_set_config(&wifi_config);
      wifi_station_clear_cert_key();
      wifi_station_clear_enterprise_ca_cert();
      wifi_station_set_wpa2_enterprise_auth(1);
      wifi_station_set_enterprise_identity((uint8*)username, strlen(username));
      wifi_station_set_enterprise_username((uint8*)username, strlen(username));
      wifi_station_set_enterprise_password((uint8*)password, strlen(password));
      wifi_station_connect();
      // WPA2 Connection ends here
    } else if (use_static_IP) {
      // we force a static IP for this station and set 
      IPAddress stationIP(static_IP[0], static_IP[1], static_IP[2], static_IP[3]);
      IPAddress gateway(static_gateway_IP[0], static_gateway_IP[1], static_gateway_IP[2], static_gateway_IP[3]); // set gateway to match your network
      IPAddress subnet(255, 255, 255, 0); // set subnet mask to match yourelse
      WiFi.mode(WIFI_STA);
      delay(100);
      
      //set a static IP address
      WiFi.config(stationIP, gateway, subnet);
      WiFi.begin(ssid, password);
    } else {
      WiFi.mode(WIFI_AP);
      WiFi.begin(ssid, password);
    }
    wifireconnectTime = millis();
  }

  if (wait) {
    unsigned long startwait = millis();
    while (WiFi.status() != WL_CONNECTED) {
      nrpoints += 1;
      delay(500);
      if (SERIALTESTOUTPUT) {
        Serial.print(".");
      }
      if (nrpoints > 50) {
        nrpoints = 0;
         if (SERIALTESTOUTPUT) Serial.println(" ");
      }
      if (millis() - startwait > 20000L ) {
        Serial.println("");
        Serial.println("Could not connect WiFi");
        return;
      }
    }
    
    if (SERIALTESTOUTPUT) {
      Serial.println("");
      Serial.println("WiFi connected");
    }
  }
}

void MQTTsubscribe2topics() {
  //write here all MQTT topics we subscribe to. Act on it in the callback!
  //MQTTclient.subscribe("xxxxx");
}

void MQTTpublish_reconnected() {
  //publish a message that we connected to 
  MQTTclient.publish("RoomEnv_T_RH-Status", "Sensor T-RH reconnected to MQTTserver");
}

boolean MQTTpublish(const char* topic, const char* payload) {
  // API to publish to the server. 
  // this is used to notify of sensor data
  MQTTclient.publish(topic, payload);
}

void MQTTreconnect() {
  // We connect to the MQTT broker
  
  //we only try to set up mqtt connection once every 1 minutes !
  if (!MQTTclient.connected() && (millis() < 60000L || millis() - mqttreconnectTime > 1*60000L)) {
    if (SERIALTESTOUTPUT) {
     Serial.print("Attempting MQTT connection...");
    }
    // Create a random client ID
    String clientId = "RoomEnv_T_RH-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (MQTTclient.connect(clientId.c_str())) {
      if (SERIALTESTOUTPUT) {
        Serial.println("connected now");
      }
      // Once connected, publish an announcement ?
      MQTTpublish_reconnected();
      // ... and resubscribe to all topics
      MQTTsubscribe2topics();
    } else {
      if (SERIALTESTOUTPUT) {
        Serial.print("failed, rc=");
        Serial.print(MQTTclient.state());
        Serial.println(" try again in 60 seconds");
      }
    }
    mqttreconnectTime = millis();
  }
}

void MQTT_msg_callback(char* topic, byte* payload, unsigned int length) {
  // if a topic subscribed to in reconnect occurs, this callback is called
  if (SERIALTESTOUTPUT) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }

}

void setupMQTTClient() {
  // to call in the setup of the arduino
  MQTTclient.setServer(mqtt_server, 1883);
  MQTTclient.setCallback(MQTT_msg_callback);
}

void handleMQTTClient() {
  // to call in the loop of the arduino
  if (!MQTTclient.connected()) {
    MQTTreconnect();
  }
  MQTTclient.loop();
}

void obtainDateTime() {
  if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
  {   
    ip = WiFi.localIP();
    obtainedwifi = true;
    date = "";  // clear the variables
    t = "";
    
    // update the NTP client and get the UNIX UTC timestamp
    unsigned long epochTime; 
    //if (random(0,2) == 0) {
      timeClient.update();
      NTPlastUpdate = millis();
      epochTime =  timeClient.getEpochTime();
    //} else {timeClient2.update();
    //  NTPlastUpdate = millis();
    //  epochTime =  timeClient2.getEpochTime();
    //}

    // convert received time stamp to time_t object
    time_t local;
    utc = epochTime;
    setTime(utc);
    
    // Then convert the UTC UNIX timestamp to local time
    // normal time from zon 2 march to sun 2 nov 
    TimeChangeRule euBRU = {"BRU", Second, Sun, Mar, 2, +60};  //normal time UTC + 1 hours - change this as needed
    TimeChangeRule euUCT = {"UCT", First, Sun, Nov, 2, 0};     //daylight saving time summer: UTC - change this as needed
    Timezone euBrussel(euBRU, euUCT);
    local = euBrussel.toLocal(utc);

    // now format the Time variables into strings with proper names for month, day etc
    date += days[weekday(local)-1];
    date += ", ";
    date += day(local);
    date += " ";
    date += months[month(local)-1];
    date += ", ";
    int yearnow = year(local);
    date += yearnow;
    if (firstcorrecttime) firstcorrecttime = false;
    if (!correcttimeset && yearnow > 2000) {
      firstcorrecttime = true;
      correcttimeset = true;
    }

    if (UK_DATE) {
      // format the time to 12-hour format with AM/PM and no seconds
      t += hourFormat12(local);
    } else {
      // normal format hour
      t += hour(local);
    }
      t += ":";
    if(minute(local) < 10)  // add a zero if minute is under 10
      t += "0";
    t += minute(local);
    t += " ";
    if (UK_DATE) {
      t += ampm[isPM(local)];
      t += " ";
    }

    if (SERIALTESTOUTPUT) {
      // Display the date and time
      Serial.println("");
      Serial.print("Local date: ");
      Serial.print(date);
      Serial.println("");
      Serial.print("Local time: ");
      Serial.println(t);
    }
  } else {
    //reconnect
    obtainedwifi = false;
  }
}

/* SET variable localtimenow to the correct current time */
void determine_localtimenow() {
  time_t timenow = now();
  // Then convert the UTC UNIX timestamp to local time

  // Then convert the UTC UNIX timestamp to local time
  // normal time from zon 2 march to sun 2 nov 
  TimeChangeRule euBRU = {"BRU", Second, Sun, Mar, 2, +60};  //normal time UTC + 1 hours - change this as needed
  TimeChangeRule euUCT = {"UCT", First, Sun, Nov, 2, 0};     //daylight saving time summer: UTC - change this as needed
  Timezone euBrussel(euBRU, euUCT);
  localtimenow = euBrussel.toLocal(timenow);
}
