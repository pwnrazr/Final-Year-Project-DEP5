#include "Adafruit_FONA.h"
#include "TinyGPS++.h"

#define FONA_RST 7  //need to be set for FONA library, despite RST pin not used
#define trigPin 6   //for ultrasonic sensor
#define echoPin 5   //for ultrasonic sensor
#define buzzer 10

//For ultrasonic sensor
long duration;
int distance;

HardwareSerial *fonaSerial = &Serial3;
 
Adafruit_FONA fona = Adafruit_FONA(FONA_RST); //Setup SIM800 library
TinyGPSPlus gps;                              //For NEO GPS module

float latitude, longitude, speed_kph, heading, speed_mph, altitude; //Stores GPS data

// timing setup
unsigned long previousMillis1 = 0;  //GPS location and GPRS updating timer
const long interval1 = 10000; 

unsigned long previousMillis2 = 0;  //Bluetooth timer
const long interval2 = 1000; 

unsigned long previousMillis3 = 0;  //Ultrasonic timer
const long interval3 = 100; 

unsigned long previousMillis4 = 0;  //for sms limiting
const long interval4 = 1000; 

int blrecv; //Stores bluetooth receive data
int temp;   //To dump any stray serial data

int smsLimiter = 0;

int alert = 0;  // if alert is 1 send notification, if 0 do nothing

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.begin(115200);   //Serial Monitor
  Serial1.begin(9600);  //GPS Module
  Serial2.begin(9600);  //BL Module

fonaSerial->begin(4800);  //SIM800 Module init using FONA library
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  };
  Serial.println(F("FONA is OK"));

  fona.enableGPRS(true);  //Enable GPRS(SIM800)
  fona.enableGPS(true);   //Enable GPS(SIM800)
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis1 >= interval1)
  {
    previousMillis1 = currentMillis;
  //Uses SIM800 module to get latitude and longitude data
  boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);
  Serial.println(F("Checking for Cell network..."));
  if (fona.getNetworkStatus() == 1) {
    boolean gsmloc_success = fona.getGSMLoc(&latitude, &longitude);
  }

    /*
    while (Serial1.available() > 0) //GPS Communication on NEO 6m
    {
      gps.encode(Serial1.read());
      if (gps.location.isUpdated())
      {
        latitude = (gps.location.lat(), 6);
        longitude = (gps.location.lng(), 6);
        
        Serial.print("Lat=");
        Serial.print(latitude);
        Serial.print("Lon="); 
        Serial.println(longitude);
      }
    } */
    while(Serial2.available() > 0)  //Flush serial2(bluetooth)
    {
      temp = Serial2.read();
      Serial2.flush();
    }
    updategps();  //Call thingspeak update function
  }

  //Bluetooth communication
  if (currentMillis - previousMillis2 >= interval2)
  {
    previousMillis2 = currentMillis;
    
      if(Serial2.available() > 0)
      {
        blrecv = Serial2.read();

        if(blrecv == 100)
        {
            char charlon[11];
            char charlat[11];
            String curlocation;
            
            dtostrf(longitude, 10, 6, charlon); //float to string
            dtostrf(latitude, 10, 6, charlat);  //float to string
            
            curlocation = charlat;
            curlocation += ",";
            curlocation += charlon; //Stitch together latitude and longitude separated by a comma, will be split in the mobile app
            curlocation += ",";

            if(alert==1)
            {
              curlocation += "Alert";
            }
            else if(alert==0)
            {
              curlocation += "Safe";
            }
            
            Serial.println("Sent location data to bluetooth");
            Serial2.print(curlocation); //actually sends the data through bluetooth
        }
      }
  }

  if (currentMillis - previousMillis3 >= interval3)   //Buzzer and ultrasonic sensor part
  {
    previousMillis3 = currentMillis;

    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.034/2;
    
    if(distance >= 100)
    {
      tone(buzzer, 1000);
      alert = 1;
      Serial.println(distance);
      
      sendSMSwarn();
    }
    else if(distance < 100)
    {
      noTone(buzzer);
      alert = 0;
      Serial.println(distance);
    }
  }

  if (currentMillis - previousMillis4 >= interval4)
  {
    previousMillis4 = currentMillis;

    if(smsLimiter > 0 && smsLimiter < 60)
    {
      Serial.print("SMS Limiter = ");
      Serial.println(smsLimiter);
      smsLimiter++;
    }
    else if(smsLimiter == 60)
    {
      Serial.println("Ready to send SMS"); 
      smsLimiter = 0;
    }
  }
}

void updategps()  //Sends current location data to thingspeak using GPRS/Internet
{

  //Stuff for HTTP thingy
  uint16_t statuscode;
  int16_t length;
  String url = "";
  char charlon[11];
  char charlat[11];
  
  dtostrf(longitude, 10, 6, charlon); //float to string
  dtostrf(latitude, 10, 6, charlat);  //float to string
  
  url = "api.thingspeak.com/update.json?api_key=QG5BVXV4LUSYQPWP&field1="; 
  url += charlon;
  url += "&field2=";
  url += charlat; //Stitches the URL, longitude and latitude together

  char buf[300];                      //Char array to store URL
  Serial.println(url);                //DEBUG      
  url.toCharArray(buf, url.length()); //converts string to char array because FONA cannot convert it by itself
        
  if (!fona.HTTP_GET_start(buf, &statuscode, (uint16_t *)&length)) {  //Connects and publishes to the URL given above
    Serial.println("Failed!");;
  }
  fona.HTTP_GET_end();  //Ends HTTP connection, should return 200 in Serial Monitor indicating a successful HTTP GET request
}

void sendSMSwarn()  // send an SMS!
{
  if(smsLimiter == 0)
  {
    smsLimiter = 1;
    Serial.println("Attempting to send SMS");
    fona.sendSMS("01136173598", "Warning! Your bag is away from you!"); //Send to Aina
    //fona.sendSMS("0173574711", "Warning! Your bag is away from you!"); //Send to My 2nd number
  }
}
