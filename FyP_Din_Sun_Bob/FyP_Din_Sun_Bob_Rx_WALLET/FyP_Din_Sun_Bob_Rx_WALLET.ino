#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

int state = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[1] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text[0]);
    if(text[0] = "E")
    {
      if(state==0)
      {
        tone(2, 10000);
        state = 1;
      }
      else if(state==1)
      {
        noTone(2);
        state = 0;
      }
    }
  }
}
