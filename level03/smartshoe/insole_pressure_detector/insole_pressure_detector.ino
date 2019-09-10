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

/** 
 *  END CONFIGURATION
 */
 
int PressureADC;
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
  Serial.print(millis()); Serial.print(", ");
  Serial.print(PressureADC); Serial.println(", ");
  }
}

void WriteHeaderSerial(){
  // csv output on serial
  Serial.print("time"); Serial.print(", ");
  Serial.print("Measured Pressure Val [0,1023]"); Serial.println(", ");
}

void setup() {
  pinMode(Dig_Pressure_LOW, OUTPUT);
  pinMode(Dig_Pressure_HIGH, OUTPUT);
  pinMode(Pressure_Sensor, INPUT);
  Serial.begin(57600);
  ard_effect0_status = -1;
  ard_effect0_start = millis();


  PressureADC = 0;
  digitalWrite(Dig_Pressure_LOW, LOW);
  digitalWrite(Dig_Pressure_HIGH, HIGH);

  WriteHeaderSerial();
}

void loop() {
  PressureADC = analogRead(Pressure_Sensor);
  
  WriteSerial();

}
