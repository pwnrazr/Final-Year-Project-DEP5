#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>

SoftwareSerial BLSerial(2, 3); // RX, TX

RF24 radio(9,10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

const byte channel1[6] = "00001";  // Channel 1 address
const byte channel2[6] = "00002";  // Channel 2 address

char sendmsg[1] = "";       // initialize array
char sendmsg2[1] = "";       // initialize array

int ch1button = 5;            // button pin for channel 1
int buttonState1 = 0;         // current state of the button0
int lastButtonState1 = 0;     // previous state of the button0

int blrecv = 0;

void setup() 
{
  Serial.begin(9600);
  BLSerial.begin(9600);
  radio.begin();

  pinMode(ch1button, INPUT_PULLUP);
  Serial.println("Start");
}
void loop() 
{
  if(BLSerial.available() > 0)
  {
    blrecv = BLSerial.read();
    Serial.print("Bluetooth received: ");
    Serial.write(blrecv);
    if(blrecv == 'A')
    {
      SendMessageCH1();
    }
    else if(blrecv == 'B')
    {
      SendMessageCH2();
    }
  }
  
  buttonState1 = digitalRead(ch1button);
  if (buttonState1 != lastButtonState1) 
  {
    if (digitalRead(ch1button) == LOW) 
      {
        SendMessageCH1();
      }
    // Debounce
    delay(100);
  }
  lastButtonState1 = buttonState1;
}

void SendMessageCH1()
{
  radio.openWritingPipe(channel1); 
  radio.stopListening();

  sendmsg[0] = 'E';
  
  Serial.print("sending: ");
  Serial.println(sendmsg);
  radio.write(&sendmsg, sizeof(sendmsg));
  Serial.println("Sent");
}

void SendMessageCH2()
{
  radio.openWritingPipe(channel2); 
  radio.stopListening();

  sendmsg2[0] = 'G';

  Serial.print("sending: ");
  Serial.println(sendmsg);
  radio.write(&sendmsg, sizeof(sendmsg));
  Serial.println("Sent");
}

void SendStopMsg()
{
  radio.openWritingPipe(channel1); 
  radio.stopListening();

  sendmsg[0] = 'Z';
  
  Serial.print("sending: ");
  Serial.println(sendmsg);
  radio.write(&sendmsg, sizeof(sendmsg));
  Serial.println("Sent");

  delay(50);
  
  radio.openWritingPipe(channel2); 
  radio.stopListening();

  sendmsg2[0] = 'Z';

  Serial.print("sending: ");
  Serial.println(sendmsg);
  radio.write(&sendmsg, sizeof(sendmsg));
  Serial.println("Sent");
}
