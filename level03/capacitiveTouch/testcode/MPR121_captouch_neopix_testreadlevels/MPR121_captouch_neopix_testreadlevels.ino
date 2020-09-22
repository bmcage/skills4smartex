/* MPR121 capacitive touch testing sketch. 

Connect MPR121 to a NodeMCU  
For more information on MPR121 see https://learn.sparkfun.com/tutorials/mpr121-hookup-guide?_ga=2.171784352.1236984808.1515404446-977687667.1513072901 

An OLED display is present for fast feedback via u8g2.

Variables: 
 * SERIALTESTOUTPUT : show debug info in serial at 115200
 * SHOWonU8g2: show info on the OLED display
 * connectedelectrodes: how many electrodes are used of the MPR121 ?
*/

const bool SERIALTESTOUTPUT = true;
/** UGg2 Code */
const bool SHOWonU8g2 = false;
const bool SHOWonNeoPix = true;
const int connectedelectrodes = 12; // how many touch electrodes present?
const unsigned int inbed_cap = 25; // hard coded, if <= inbed_cap, then person on top of the electrode
bool changedstate = false;

// neopixel pin
int NeoPixel = D4;
// I2C on D1 = SDA and D2 =SCL 
int pSDA = D1;
int pSCL = D2;

// OLED display with U8g2 lib
/** UGg2 Code */
//#include <U8g2lib.h>

// MPR121 constants
#include "mpr121.h"

// controlled with I2C:
#include <Wire.h>

// MPR121 IRQ pin
const int irqpin = A0;  // Analog pin 0
boolean touchStates[12];         //to keep track of the previous touch states
boolean firsttouchStates[12];    //to keep track if button was just touched
boolean releasedtouchStates[12]; //to keep track if button was just released
boolean personinbedStates[12];   // to keep track if a person in the bed (prolonged touch)
// map a position from top (0) to bottom (5) to the actual pins
int mapposition2pin[connectedelectrodes] = {4, 2, 5, 3, 1, 0};

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel myNeo_NeoPixel = Adafruit_NeoPixel(16, NeoPixel, NEO_GRB + NEO_KHZ800);


// setup the OLED display of 128x32
// Use U8X8_PIN_NONE if the reset pin is not connected
#define REPORTING_PERIOD_MS     500

/** UGg2 Code */
//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, pSCL, pSDA, U8X8_PIN_NONE);
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, pSDA, pSCL);

bool initialized = false;
byte beat = 0;

unsigned long starttime, startshowtime, interval, intervalshow;
unsigned long startinbedtime, intervalinbed;

void setup(void) {
  interval = 1000L;
  intervalshow = 500L;
  intervalinbed = 1000L;
  if (SERIALTESTOUTPUT) {
    Serial.begin(115200);   // We'll send debugging information via the Serial monitor
    Serial.print("Serial startedpin! ");
  }

  //set up OLED and MPR121 over I2C bus 
  Wire.begin(pSDA, pSCL);   // Initialiseer de I2C bus op pinnen pSDA en pSCL

  myNeo_NeoPixel.begin();
  
  if (SHOWonNeoPix) {
    for (int i = 0; i <= 15; i++) {
      myNeo_NeoPixel.setPixelColor(i-1, myNeo_NeoPixel.Color(0,0,0));
      delay(50);
      myNeo_NeoPixel.show();
    }
  }
  if (SERIALTESTOUTPUT) {
    Serial.print("Color set up ");
    delay(1000);
  }
  
  mpr121_setup();
/** UGg2 Code */
//
//  if (SHOWonU8g2) {
//    u8g2.begin();
//    initial_display();
//  }
  
  for (int i=0; i < 12; i++){
    firsttouchStates[i] = 0;
    releasedtouchStates[i] = 0;
    touchStates[i] = 0;
    personinbedStates[i] = 0;
  }
  if (SERIALTESTOUTPUT) {
    Serial.print("Ended Setup Configuration! ");
  }
/** UGg2 Code */
//  if (SHOWonU8g2) {
//    show_inbed();
//  }
  starttime = millis();
  startshowtime = starttime;
}
 
void loop(void) {
  changedstate = false;
  readTouchInputs();
  //if (changedstate) {
  //  if (SHOWonU8g2) {
  //    show_touch();
  //  }
  //}
  if ((millis() - startshowtime) > intervalshow) {
    //update the screen
    if (SHOWonU8g2) {
      //show_inbed();
    }
    startshowtime = millis();
  }
  
  if (SERIALTESTOUTPUT) {
    if ((millis() - starttime) > interval) {
      show_debugdata();
      starttime = millis();
      //delay(500);
    }
  }
     show_light();
  delay(100);
}


// initialize display 
/** UGg2 Code */
//void initial_display()
//{
//  if (not initialized)
//  {
//    u8g2.clearBuffer(); // clear the internal memory
//    u8g2.setFont(u8g2_font_smart_patrol_nbp_tf); // choose a suitable font
//    
//    u8g2.drawStr(0,10,"Capacitive");
//    u8g2.drawStr(40,25,"Touch!");  // write something to the internal memory
//    u8g2.sendBuffer();          // transfer internal memory to the display
//    delay(4000);  
//  
//    initialized = true;
//  }
//}

/** UGg2 Code */
//void show_touch() {
//  u8g2.clearBuffer(); // clear the internal memory
//  u8g2.setFont(u8g2_font_smart_patrol_nbp_tf); // choose a suitable font
//  u8g2.drawStr(0,10,"Parts touched:");  // write something to the internal memory
//  int xpos = 10;
//  for (int i=0; i<connectedelectrodes; i++) {
//    if (touchStates[mapposition2pin[i]] == 0) {
//      u8g2.drawStr(xpos, 25, "0");
//    } else {
//      u8g2.drawStr(xpos, 25, "1");
//    }
//    xpos += 15;
//  }
//  u8g2.sendBuffer();          // transfer internal memory to the display
//}

/** UGg2 Code */
//void show_inbed() {
//  u8g2.clearBuffer(); // clear the internal memory
//  u8g2.setFont(u8g2_font_smart_patrol_nbp_tf); // choose a suitable font
//  u8g2.drawStr(0,10,"Person in bed:");  // write something to the internal memory
//  int xpos = 10;
//  for (int i=0; i<connectedelectrodes; i++) {
//    if (personinbedStates[mapposition2pin[i]] == 0) {
//      u8g2.drawStr(xpos, 25, "0");
//    } else {
//      u8g2.drawStr(xpos, 25, "1");
//    }
//    xpos += 15;
//  }
//  u8g2.sendBuffer();          // transfer internal memory to the display
//}

/*
 * Read what button is touched and set 
 */
void readTouchInputs(){
  if (checkInterrupt()){
    changedstate = true;
    //read the touch state from the MPR121
    Wire.requestFrom(0x5A, 2); 
    
    byte LSB = Wire.read();
    byte MSB = Wire.read();
    
    uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states

    for (int i=0; i < 12; i++){  // Check what electrodes were pressed
      firsttouchStates[i] = 0;
      releasedtouchStates[i] = 0;
      if(touched & (1<<i)){
      
        if (touchStates[i] == 0){
          //pin i was just touched
          if (SERIALTESTOUTPUT ) {
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" was just touched");
          }
          firsttouchStates[i] = 1;
        } else if(touchStates[i] == 1){
          //pin i is still being touched
        }
        touchStates[i] = 1;      
      } else {
        if (touchStates[i] == 1){
          if (SERIALTESTOUTPUT ) {
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" is no longer being touched");
          }
          releasedtouchStates[i] = 1;
          //pin i is no longer being touched
        }
        touchStates[i] = 0;
      }
    }
  }
  // we also determine in bed state based on a fixed cutoff
  if ((millis() - startinbedtime) > intervalinbed) {
    //update the inbed states
    for (uint8_t i=0; i<connectedelectrodes; i++) {
      if (filteredData(i) <= inbed_cap || baselineData(i) <= inbed_cap) {
        personinbedStates[i] = true; 
      } else {
        personinbedStates[i] = false; 
      }
    }
    startinbedtime = millis();
  }
}
void show_light() {
  for (uint8_t i=0; i<12; i++) {
    if (SHOWonNeoPix) {
      if (filteredData(i) > 90) {
        myNeo_NeoPixel.setPixelColor(i, myNeo_NeoPixel.Color(0,0,0));
        myNeo_NeoPixel.show();
      } else {
        myNeo_NeoPixel.setPixelColor(i, myNeo_NeoPixel.Color(0,100,0));
        myNeo_NeoPixel.show();
      }
    }
  }
}

void show_debugdata() {
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(touched(), HEX);

  Serial.print("Filt: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(baselineData(i)); Serial.print("\t");
  }
  Serial.println();
}

/* 
 *  Setup of the MPR121 sensor, setting operation and thresholds.
 */
void mpr121_setup(void){

  set_register(0x5A, ELE_CFG, 0x00); 
  
  // Section A - Controls filtering when data is > baseline.
  set_register(0x5A, MHD_R, 0x01);
  set_register(0x5A, NHD_R, 0x01);
  set_register(0x5A, NCL_R, 0x00);
  set_register(0x5A, FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  set_register(0x5A, MHD_F, 0x01);
  set_register(0x5A, NHD_F, 0x01);
  set_register(0x5A, NCL_F, 0xFF);
  set_register(0x5A, FDL_F, 0x02);
  
  // Section C - Sets touch and release thresholds for each electrode
  set_register(0x5A, ELE0_T, TOU_THRESH);
  set_register(0x5A, ELE0_R, REL_THRESH);
 
  set_register(0x5A, ELE1_T, TOU_THRESH);
  set_register(0x5A, ELE1_R, REL_THRESH);
  
  set_register(0x5A, ELE2_T, TOU_THRESH);
  set_register(0x5A, ELE2_R, REL_THRESH);
  
  set_register(0x5A, ELE3_T, TOU_THRESH);
  set_register(0x5A, ELE3_R, REL_THRESH);
  
  set_register(0x5A, ELE4_T, TOU_THRESH);
  set_register(0x5A, ELE4_R, REL_THRESH);
  
  set_register(0x5A, ELE5_T, TOU_THRESH);
  set_register(0x5A, ELE5_R, REL_THRESH);
  
  set_register(0x5A, ELE6_T, TOU_THRESH);
  set_register(0x5A, ELE6_R, REL_THRESH);
  
  set_register(0x5A, ELE7_T, TOU_THRESH);
  set_register(0x5A, ELE7_R, REL_THRESH);
  
  set_register(0x5A, ELE8_T, TOU_THRESH);
  set_register(0x5A, ELE8_R, REL_THRESH);
  
  set_register(0x5A, ELE9_T, TOU_THRESH);
  set_register(0x5A, ELE9_R, REL_THRESH);
  
  set_register(0x5A, ELE10_T, TOU_THRESH);
  set_register(0x5A, ELE10_R, REL_THRESH);
  
  set_register(0x5A, ELE11_T, TOU_THRESH);
  set_register(0x5A, ELE11_R, REL_THRESH);
  
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  set_register(0x5A, FIL_CFG, 0x04);
  
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes
  
  
  // Section F
  // Enable Auto Config and auto Reconfig
  /*set_register(0x5A, ATO_CFG0, 0x0B);
  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
  
  set_register(0x5A, ELE_CFG, 0x0C);
  
}

/*
 * Check the MPR121. When an action happened, returns LOW, otherwise HIGH
 */
boolean checkInterrupt(void){
  return (analogRead(irqpin) < 600);
}

// set values over I2C
void set_register(int address, unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}

/*********************************************************************/
/* ADAFRUIT LIB METHODS */
/*********************************************************************/
void setThresholds(uint8_t touch, uint8_t release) {
  for (uint8_t i=0; i<12; i++) {
    writeRegister(MPR121_TOUCHTH_0 + 2*i, touch);
    writeRegister(MPR121_RELEASETH_0 + 2*i, release);
  }
}

uint16_t  filteredData(uint8_t t) {
  if (t > 12) return 0;
  return readRegister16(MPR121_FILTDATA_0L + t*2);
}

uint16_t  baselineData(uint8_t t) {
  if (t > 12) return 0;
  uint16_t bl = readRegister8(MPR121_BASELINE_0 + t);
  return (bl << 2);
}

uint16_t  touched(void) {
  uint16_t t = readRegister16(MPR121_TOUCHSTATUS_L);
  return t & 0x0FFF;
}

uint8_t readRegister8(uint8_t reg) {
    Wire.beginTransmission(MPR121_I2CADDR_DEFAULT);
    Wire.write(reg);
    Wire.endTransmission(false);
    while (Wire.requestFrom(MPR121_I2CADDR_DEFAULT, 1) != 1);
    return ( Wire.read());
}

uint16_t readRegister16(uint8_t reg) {
    Wire.beginTransmission(MPR121_I2CADDR_DEFAULT);
    Wire.write(reg);
    Wire.endTransmission(false);
    while (Wire.requestFrom(MPR121_I2CADDR_DEFAULT, 2) != 2);
    uint16_t v = Wire.read();
    v |=  ((uint16_t) Wire.read()) << 8;
    return v;
}

/**************************************************************************/
/*!
    @brief  Writes 8-bits to the specified destination register
*/
/**************************************************************************/
void writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MPR121_I2CADDR_DEFAULT);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)(value));
    Wire.endTransmission();
}
