//Polling timing
long previousMillis = 0;
long previousMillis1 = 0;
long previousMillis2 = 0;
long previousMillis3 = 0;

//Motor state checking and pump timing
unsigned int motorRun = 0;
unsigned int pumpRun = 0;
unsigned int pumpcount = 0;

//Motor turn on timing
unsigned int oninterval = 1000;
unsigned int onCountMax = 5;    //How long to turn on in seconds
unsigned int onCounter = 0;

//Edge detection for button
unsigned int onButtonstate = 0;
unsigned int lastonButtonstate = 0;

//Pin definitions
#define safetydoor 2
#define onbutton 3

#define relay0 4

#define buzzer0 5
#define buzzer1 6

#define trigPin0 7
#define echoPin0 8

#define trigPin1 9
#define echoPin1 10

#define trigPin2 11
#define echoPin2 12

#define grnled 24
#define yelled 23
#define redled 22

#define redwarn 25
#define grnwarn 26

#define pumprelay 27

void setup() 
{
  //Pin mode setup
  pinMode(safetydoor, INPUT_PULLUP); // Safety Door
  pinMode(onbutton, INPUT_PULLUP); // On button
  
  pinMode(relay0, OUTPUT);       // Relay
  
  pinMode(buzzer0, OUTPUT);       // Buzzer
  pinMode(buzzer1, OUTPUT);       // Buzzer
  
  pinMode(trigPin0, OUTPUT);      // Trig
  pinMode(echoPin0, INPUT);       // Echo

  pinMode(trigPin1, OUTPUT);      // Trig
  pinMode(echoPin1, INPUT);      // Echo

  pinMode(trigPin2, OUTPUT);      // Trig
  pinMode(echoPin2, INPUT);      // Echo

  pinMode(grnled, OUTPUT);      //Green LED
  pinMode(yelled, OUTPUT);      //Yellow LED
  pinMode(redled, OUTPUT);      //Red LED

  pinMode(redwarn, OUTPUT);     //red warning LED
  pinMode(grnwarn, OUTPUT);     //green warning LED

  pinMode(pumprelay, OUTPUT);  //Relay for water pump
  
  // Debug
  Serial.begin(9600);
  Serial.println("=START=");

  digitalWrite(grnwarn, HIGH);  //Turn on green warning led
}

void loop() 
{
  unsigned long currentMillis = millis();

  onButtonstate = digitalRead(onbutton);
  
  // Button and motor on part
  if(currentMillis - previousMillis > 100) 
  { 
    previousMillis = currentMillis;

    //Serial.println("Button and Motor");
    if(onButtonstate != lastonButtonstate)
    {
      if(onButtonstate == HIGH && motorRun == 0 && digitalRead(safetydoor)==0)
      {
        Serial.println("Button Pressed, Motor on");
        digitalWrite(relay0, HIGH);  // On motor Relay
        digitalWrite(pumprelay, HIGH); //Turn on pump relay
        
        digitalWrite(redwarn, HIGH);   //turn on red warning led
        digitalWrite(grnwarn, LOW);   //Turn off green led
      
        pumpRun = 1;
        pumpcount = 0;
        motorRun = 1;
      }
      else if(onButtonstate == HIGH && motorRun == 1)
      {
        digitalWrite(relay0, LOW);  // OFF Relay motor
        motorRun = 0;
        Serial.println("Motor Emergency OFF");

        digitalWrite(redwarn, LOW);   //turn OFF red warning led
        digitalWrite(grnwarn, HIGH);   //Turn ON green led

        digitalWrite(pumprelay, LOW); //Turn off pump relay
        Serial.println("Pump Relay Emergency OFF");   //Debug
        pumpRun = 0;
        onCounter = 0;
      }
    }
    lastonButtonstate = onButtonstate;
  }

  //Waits for x amount of time to turn off motor automatically
  if(currentMillis - previousMillis2 > oninterval && motorRun == 1) 
  { 
    previousMillis2 = currentMillis;

    if(onCounter < onCountMax)
    {
      Serial.println(onCounter);
      onCounter++;
    }
    else if(onCounter == onCountMax)
    {
      Serial.println("MOTOR OFF");
      digitalWrite(relay0, LOW); // Off Relay

      digitalWrite(redwarn, LOW);   //turn off red warning led
      digitalWrite(grnwarn, HIGH);   //Turn on yellow warning led

      onCounter = 0;
      motorRun = 0; 
    }
  }
  
  // Ultrasonic and buzzer part
  if(currentMillis - previousMillis1 > 100) 
  { 
    previousMillis1 = currentMillis;

    //Ultrasonic sensor 01
    long duration0, distance0;
    digitalWrite(trigPin0, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin0, HIGH);

    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin0, LOW);
    duration0 = pulseIn(echoPin0, HIGH);
    distance0 = (duration0/2) / 29.1;
    //Serial.print("Ultrasonic0: ");
    //Serial.println(distance0);    //debug
    
    //Ultrasonic sensor 02
    long duration1, distance1;
    digitalWrite(trigPin1, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin1, HIGH);

    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distance1 = (duration1/2) / 29.1;
    //Serial.print("Ultrasonic1: ");
    //Serial.println(distance1);    //debug

    //Ultrasonic sensor 3
    long duration2, distance2;
    digitalWrite(trigPin2, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin2, HIGH);

    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);
    distance2 = (duration2 / 2) / 29.1;
    //Serial.print("Ultrasonic2: ");
    //Serial.println(distance2);    //debug
    
    if(distance0 < 10)
    {
      tone(buzzer0, 1000);
    }
    else if(distance0 > 10)
    {
      noTone(buzzer0);
    }
    
    if(distance1 < 10)
    {
      tone(buzzer1, 1000);
      //Serial.println("Buzzer 1 buzzing"); //Debug
    }
    else if(distance1 > 10)
    {
      noTone(buzzer1);
    }

    if(distance2 > 50)  //red led on
    {
      digitalWrite(redled, 1);
      digitalWrite(yelled, 0);
      digitalWrite(grnled, 0);
    }
    else if(distance2 <= 50 && distance2 >= 20) //yellow led on
    {
      digitalWrite(redled, 0);
      digitalWrite(yelled, 1);
      digitalWrite(grnled, 0);
    }
    else if(distance2 < 20) //green led on
    {
      digitalWrite(redled, 0);
      digitalWrite(yelled, 0);
      digitalWrite(grnled, 1);
    }
  }

  //Pump relay timing
  if(currentMillis - previousMillis3 > 1000 && pumpRun == 1 && motorRun == 1) 
  { 
    previousMillis3 = currentMillis;

    if(pumpcount == 2)
    {
      digitalWrite(pumprelay, LOW); //Turn off relay
      Serial.println("Pump Relay OFF");   //Debug
      pumpRun = 0;
    }
    else
    {
      pumpcount++;
      Serial.print("pumpcount = ");
      Serial.println(pumpcount);       //Debug
    }
  }
}
