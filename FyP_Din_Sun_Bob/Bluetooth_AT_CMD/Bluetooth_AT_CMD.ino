#include <SoftwareSerial.h>

SoftwareSerial BLSerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  BLSerial.begin(38400);

}

void loop() {
  if(Serial.available() > 0)
  {
    BLSerial.write(Serial.read());
  }

  if(BLSerial.available() > 0)
  {
    Serial.write(BLSerial.read());
  }

}
