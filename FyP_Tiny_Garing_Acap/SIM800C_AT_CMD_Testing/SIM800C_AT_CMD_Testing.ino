#include <SoftwareSerial.h>

SoftwareSerial SIMSerial(10, 9); // RX, TX    //initialize Software serial for SIM800C GSM/GPRS Module

void setup() 
{
  Serial.begin(19200);
  SIMSerial.begin(19200);

  Serial.println("BEGIN");

  SendMessage();
}

void loop() 
{ 
  if(Serial.available() > 0)
  {
    SIMSerial.write(Serial.read());
  }
  
  if(SIMSerial.available() > 0)
  {
    Serial.write(SIMSerial.read());
  } 
}

 void SendMessage()
{
    SIMSerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
    SIMSerial.println("AT+CMGS=\"+60173574711\"\r"); // Replace x with mobile number
  delay(1000);
    SIMSerial.println("I am SMS from GSM Module");// The SMS text you want to send
  delay(100);
   SIMSerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
