int latchPin = 5; // 74HC595 ST_CP
int clockPin = 6; // 74HC595 SH_CP
int dataPin = 4;  // 74HC595 DS

byte data = 0;
float freq = 1000.0f; // 1khz

void setup(){
  data = 0;
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop(){
  data = int(128+128*sin(2*3.14159*micros()/50000.0f));
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(1);
}