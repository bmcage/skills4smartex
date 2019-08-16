int PressureADC;
int Dig_Pressure_LOW = D5;
int Dig_Pressure_HIGH = D7;
int Pressure_Sensor = A0;

void setup() {
  pinMode(Dig_Pressure_LOW, OUTPUT);
  pinMode(Dig_Pressure_HIGH, OUTPUT);
  pinMode(Pressure_Sensor, INPUT);
  Serial.begin(57600);

  PressureADC = 0;
  digitalWrite(Dig_Pressure_LOW, LOW);
  digitalWrite(Dig_Pressure_HIGH, HIGH);

}

void loop() {
  PressureADC = analogRead(Pressure_Sensor);
  Serial.print("Measured Pressure Val [0,1023] = ");
  Serial.println(PressureADC);
  delay(1000);

}
