#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00002";

int state = 0;

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  
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
    if(text[0] = 'A')
    {
      if(state==0)
      {
        digitalWrite(2, HIGH);
        state = 1;
      }
      else if(state==1)
      {
        digitalWrite(2, LOW);
        state = 0;
      }
    }
  }
}
