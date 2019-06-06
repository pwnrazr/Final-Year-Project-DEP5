#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>

SoftwareSerial BLSerial(2, 3); // RX, TX

RF24 radio(9,10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

const byte channel1[6] = "00002";  // Channel 1 address

char sendmsg[1] = "";       // initialize array

int ch1button = 5;            // button pin for channel 1
int buttonState1 = 0;         // current state of the button0
int lastButtonState1 = 0;     // previous state of the button0

int blrecv = 0;

void setup() 
{
  Serial.begin(9600);
  BLSerial.begin(9600);
  radio.begin();
  radio.openWritingPipe(channel1); 
  radio.stopListening();
  
  pinMode(ch1button, INPUT_PULLUP);
  Serial.println("Start");
}
void loop() 
{
  buttonState1 = digitalRead(ch1button);
  if (buttonState1 != lastButtonState1) 
  {
    if (digitalRead(ch1button) == LOW) 
      {
        //Serial.println("send");
        //radio.openWritingPipe(channel1); 
        sendmsg[0] = "E";
        //Serial.write(sendmsg); 
        Serial.print("sending: ");
        Serial.println(sendmsg);
        radio.write(&sendmsg, sizeof(sendmsg));
        Serial.println("Sent");
      }
    // Debounce
    delay(100);
  }
  lastButtonState1 = buttonState1;

  if(BLSerial.available() > 0)
  {
    blrecv = BLSerial.read();

    if(blrecv = 120)
    {
        sendmsg[0] = "E";
        //Serial.write(sendmsg); 
        Serial.print("sending: ");
        Serial.println(sendmsg);
        radio.write(&sendmsg, sizeof(sendmsg));
        Serial.println("Sent");
    }
  }
}
