#include <SoftwareSerial.h>

int incomingByte = 0; //stores last received bluetooth data
int alertstatus = 0;  //status of alert
int buzzing = 0;      //for buzz timing

unsigned long previousMillis = 0;       

const long interval = 1000;    //interval for on off timing

SoftwareSerial blSerial(8, 9); // RX, TX //initialize softwareserial for bluetooth module

void setup()
{
  Serial.begin(9600);   //begin arduino serial communication
  blSerial.begin(9600); //begin serial communication for Bluetooth module
  pinMode(4, OUTPUT); //set led pin as output
  pinMode(7, OUTPUT); //set vibrator pin as output
  pinMode(5, OUTPUT); //set buzzer pin as output
}

void loop() 
{
  unsigned long currentMillis = millis();
      
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;

    if(alertstatus==1 && buzzing==0)
    {
      digitalWrite(4, HIGH);  //turn on led
      digitalWrite(7, HIGH);  //turn on vibrator
      digitalWrite(5, HIGH);  //turn on buzzer
      buzzing=1;  
    } 
    else if(alertstatus==1 && buzzing==1)
    {
      digitalWrite(4, LOW); //turn off led
      digitalWrite(7, LOW); //turn off vibrator
      digitalWrite(5, LOW); //turn off buzzer
      buzzing=0;
    } 
  }
  
  if(blSerial.available() > 0)
  {
    incomingByte = blSerial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

    if(incomingByte == 169 && digitalRead(4)== LOW)
    {
      Serial.println("ON");
      alertstatus = 1;
      //digitalWrite(4, HIGH);
      //digitalWrite(7, HIGH);
      //tone(5, 1000);
    }
    else if(incomingByte == 169 && digitalRead(4)== HIGH)
    {
      Serial.println("OFF");
      alertstatus = 0;
      digitalWrite(4, LOW);
      digitalWrite(7, LOW);
      noTone(5);
      buzzing=0;
    }
  }
}
