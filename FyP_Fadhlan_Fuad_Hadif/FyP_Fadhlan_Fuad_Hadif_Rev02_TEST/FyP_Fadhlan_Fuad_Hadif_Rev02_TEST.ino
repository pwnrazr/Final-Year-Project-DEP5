#include <SoftwareSerial.h>

SoftwareSerial BLSerial(10, 11); // RX, TX

//Current reading setup start
const int ampereIn = A0;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
int ampereRawValue= 0;
int ACSoffset = 2500; 
double ampereVoltage = 0;
double Amps = 0;
//Current reading setup stop

//Ohm reading setup start
int ohmanalogPin = A1;
int ohmraw = 0;
int ohmVin = 5;
float ohmVout = 0;
float ohmR1 = 10000;
float ohmR2 = 0;
float buffer = 0;
//Ohm reading setup stop


//Voltage reading setup start
#define NUM_SAMPLES 10

int voltagePin = A2;
int voltageSum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage
//Voltage reading setup stop

void setup() 
{
  Serial.begin(9600);
  BLSerial.begin(9600);
}

void loop() 
{
  ampereRead();
  delay(1000);
  ohmRead();
  delay(1000);
  voltageRead();
  delay(1000); 
}

void ampereRead()
{
  ampereRawValue = analogRead(ampereIn);
  ampereVoltage = (ampereRawValue / 1024.0) * 5000; // Gets you mV
  Amps = ((ampereVoltage - ACSoffset) / mVperAmp);
 
  Serial.print("Ampere Raw Value = " ); // shows pre-scaled value 
  Serial.print(ampereRawValue); 
  Serial.print("\t mV = "); // shows the voltage measured 
  Serial.print(ampereVoltage,3); // the '3' after voltage allows you to display 3 digits after decimal point
  Serial.print("\t Amps = "); // shows the voltage measured 
  Serial.println(Amps,3); // the '3' after voltage allows you to display 3 digits after decimal point
}
void ohmRead()
{
  ohmraw = analogRead(ohmanalogPin);
  
  if(ohmraw) 
  {
    buffer = ohmraw * ohmVin;
    ohmVout = (buffer)/1024.0;
    buffer = (ohmVin/ohmVout) -1;
    ohmR2 = ohmR1 * buffer;
    Serial.print("Vout: ");
    Serial.println(ohmVout);
    Serial.print("R2: ");
    Serial.println(ohmR2);
  }
}

void voltageRead()
{
  // take a number of analog samples and add them up
    while (sample_count < NUM_SAMPLES) {
        voltageSum += analogRead(voltagePin);
        sample_count++;
        delay(10);
    }
    // calculate the voltage
    // use 5.0 for a 5.0V ADC reference voltage
    // 5.015V is the calibrated reference voltage
    voltage = ((float)voltageSum / (float)NUM_SAMPLES * 5.015) / 1024.0;
    // send voltage for display on Serial Monitor
    // voltage multiplied by 11 when using voltage divider that
    // divides by 11. 11.132 is the calibrated voltage divide
    // value
    Serial.print(voltage * 11.132);
    Serial.println (" V");
    sample_count = 0;
    voltageSum = 0;
}
