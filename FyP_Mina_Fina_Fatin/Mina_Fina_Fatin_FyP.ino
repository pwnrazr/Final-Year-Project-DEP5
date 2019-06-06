long previousMillis = 0;  // will store last time LED was updated
long previousMillis1 = 0;
long previousMillis2 = 5000;

unsigned int motorRun = 0;

#define relay0 4

#define buzzer0 5
#define buzzer1 6

#define trigPin0 7
#define echoPin0 8

#define trigPin1 9
#define echoPin1 10

void setup() 
{
  pinMode(2, INPUT_PULLUP); // Safety Door
  pinMode(3, INPUT_PULLUP); // On button
  
  pinMode(relay0, OUTPUT);       // Relay
  
  pinMode(buzzer0, OUTPUT);       // Buzzer
  pinMode(buzzer1, OUTPUT);       // Buzzer
  
  pinMode(trigPin0, OUTPUT);      // Trig
  pinMode(echoPin0, INPUT);       // Echo

  pinMode(trigPin1, OUTPUT);      // Trig
  pinMode(echoPin1, INPUT);      // Echo

  // Debug
  Serial.begin(9600);
  Serial.println("=START=");
}

void loop() 
{
  unsigned long currentMillis = millis();

  // Button and motor on part
  if(currentMillis - previousMillis > 100) 
  { 
    previousMillis = currentMillis;

    //Serial.println("Button and Motor");
    if(digitalRead(2)==0 && digitalRead(3)==1)
    {
      Serial.println("Button Pressed, Motor on");
      digitalWrite(relay0, HIGH);  // On Relay
    
      motorRun = 1;
    }

  //Waits for x amount of time to turn off motor automatically
  if(currentMillis - previousMillis2 > 5000 && motorRun == 1) 
  { 
    previousMillis2 = currentMillis;

    Serial.println("MOTOR OFF");
    digitalWrite(relay0, LOW); // Off Relay
    
    motorRun = 0;
  }
  
  // Ultrasonic and buzzer part
  if(currentMillis - previousMillis1 > 100) 
  { 
    previousMillis1 = currentMillis;
    //Serial.println("Ultrasonic MILLIS");

    //Ultrasonic sensor 01
    long duration0, distance0;
    digitalWrite(trigPin0, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin0, HIGH);
  //delayMicroseconds(1000); - Removed this line
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin0, LOW);
    duration0 = pulseIn(echoPin0, HIGH);
    distance0 = (duration0/2) / 29.1;
    //Serial.print("Ultrasonic0: ");
    //Serial.println(distance0);

    //Ultrasonic sensor 02
    long duration1, distance1;
    digitalWrite(trigPin1, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin1, HIGH);
  //delayMicroseconds(1000); - Removed this line
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distance1 = (duration1/2) / 29.1;
    //Serial.print("Ultrasonic1: ");
    //Serial.println(distance1);

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
      Serial.println("Buzzer 1 buzzing"); //Debug
    }
    else if(distance1 > 10)
    {
      noTone(buzzer1);
    }
  }
}
}
