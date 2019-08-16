int PressureADC;
int lowPressure;
int mediumPressure;
int highPressure;
int BlueColor;
int i;
int GreenColor;
int Dig_Pressure_LOW = D5;
int Dig_Pressure_HIGH = D7;
int neopixels = D3;
int Pressure_Sensor = A0;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel myNeo_neopixels = Adafruit_NeoPixel(2, D3, NEO_RGBW + NEO_KHZ800);

void setup() {
  pinMode(Dig_Pressure_LOW, OUTPUT);
  pinMode(Dig_Pressure_HIGH, OUTPUT);
  myNeo_neopixels.begin();
  myNeo_neopixels.show();
  pinMode(Pressure_Sensor, INPUT);
  Serial.begin(57600);

  PressureADC = 0;
  lowPressure = 150;
  mediumPressure = 400;
  highPressure = 650;
  digitalWrite(Dig_Pressure_LOW, LOW);
  digitalWrite(Dig_Pressure_HIGH, HIGH);

}

void loop() {
  PressureADC = analogRead(Pressure_Sensor);
  Serial.print("Measured Pressure Val [0,1023] = ");
  Serial.println(PressureADC);
  if (PressureADC < lowPressure) {
    PressureADC = lowPressure;
  } else if (PressureADC > highPressure) {
    PressureADC = highPressure;
  }
  if (PressureADC < mediumPressure) {
    BlueColor = ((PressureADC - lowPressure) * 99.9) / (mediumPressure - lowPressure);
    for (i = 1; i <= 2; i++) {
      myNeo_neopixels.setPixelColor(i-1, myNeo_neopixels.Color((100 - BlueColor),0,BlueColor));
      myNeo_neopixels.show();
    }
  } else {
    GreenColor = ((PressureADC - mediumPressure) * 99.9) / (highPressure - mediumPressure);
    for (i = 1; i <= 2; i++) {
      myNeo_neopixels.setPixelColor(i-1, myNeo_neopixels.Color(0,GreenColor,(100 - GreenColor)));
      myNeo_neopixels.show();
    }
  }
  delay(1000);

  map(0, 0, 1024, 0, 100);

}
