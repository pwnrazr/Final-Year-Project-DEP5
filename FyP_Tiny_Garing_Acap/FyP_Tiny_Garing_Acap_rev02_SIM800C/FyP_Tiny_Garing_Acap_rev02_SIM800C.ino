#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "HX711.h"

SoftwareSerial SIMSerial(10, 9); // RX, TX    //initialize Software serial for SIM800C GSM/GPRS Module

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// HX711.DOUT  - pin #A1
// HX711.PD_SCK - pin #A0
HX711 scale(A1, A0);    // parameter "gain" is ommited; the default value 128 is used by the library

float currentweight = 0;

unsigned long previousMillis = 0; 
unsigned long previousMillis2 = 0; 

unsigned int smslimiter = 0;

void setup() 
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  SIMSerial.begin(9600);
  
  scale.set_scale(137.f);      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();                 // reset the scale to 0
}

void loop() 
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 100) 
  {
    previousMillis = currentMillis;
    
    currentweight = scale.get_units();
    //Serial.println(currentweight);

    updateLCD();    //Update lcd with latest weight reading
  
    if(currentweight > 2000)
    {
      lcd.setCursor(0,1);
      lcd.print("Overweight!!!"); //display on 2nd line
      SendMessage();
    }
    else if(currentweight < 2000)
    {
      lcd.setCursor(0,1);
      lcd.print("                ");  //clears 2nd line
    }
  }

  if (currentMillis - previousMillis2 >= 1000) // limits to one SMS every minute MAX
  {
    previousMillis2 = currentMillis;

    if(smslimiter > 0 && smslimiter < 60)
    {
      smslimiter++;
      Serial.print("SMSLIMITER: ");
      Serial.println(smslimiter);
    }
    else if(smslimiter == 60)
    {
      smslimiter = 0;
      Serial.println("READY TO SEND SMS");
    }
  }
  
}

void updateLCD()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Weight:");
  lcd.setCursor(15,0);
  lcd.print("g");
  lcd.setCursor(7,0);
  lcd.print(currentweight); 
}

 void SendMessage()
{
  if(smslimiter == 0)
  {
    smslimiter = 1;
      SIMSerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
      SIMSerial.println("AT+CMGS=\"+60173574711\"\r"); // Replace with mobile number
    delay(1000);
      SIMSerial.println("Overweight limit reached!");// The SMS text you want to send
    delay(100);
      SIMSerial.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    Serial.println("SENT SMS");
  }
}
