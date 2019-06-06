// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;

// defines variables
long duration;
int distance;
int buzzfrequency = 3000;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
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
  distance = duration * 0.034 / 2;
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);

  if (distance <= 40 && distance >= 35)
  {
    tone(4, buzzfrequency, 50);
    delay(300);
  }
  else if(distance <= 34 && distance >= 30)
  {
    tone(4, buzzfrequency, 50);
    delay(250);
  }
  else if(distance <= 29 && distance >= 25)
  {
    tone(4, buzzfrequency, 50);
    delay(200);
  }
  else if(distance <= 24 && distance >= 20)
  {
    tone(4, buzzfrequency, 50);
    delay(175);
  }
  else if(distance <= 19 && distance >= 15)
  {
    tone(4, buzzfrequency, 50);
    delay(150);
  }
  else if(distance <= 14 && distance >= 10)
  {
    tone(4, buzzfrequency, 50);
    delay(125);
  }
  else if(distance < 10)
  {
    tone(4, buzzfrequency, 50);
    //delay(125);
  }
  
  /*
  if (distance == 15)
  {
    tone(4, buzzfrequency, 50);
    delay(300);
  }
  else if(distance == 14)
  {
    tone(4, buzzfrequency, 50);
    delay(250);
  }
  else if(distance == 13)
  {
    tone(4, buzzfrequency, 50);
    delay(200);
  }
  else if(distance == 12)
  {
    tone(4, buzzfrequency, 50);
    delay(175);
  }
  else if(distance == 11)
  {
    tone(4, buzzfrequency, 50);
    delay(150);
  }
  else if(distance == 10)
  {
    tone(4, buzzfrequency, 50);
    delay(125);
  }
  else if(distance == 9)
  {
    tone(4, buzzfrequency, 50);
    delay(100);
  }
  else if(distance == 8)
  {
    tone(4, buzzfrequency, 50);
    delay(75);
  }
  else if(distance <= 7)
  {
    tone(4, buzzfrequency, 50);
    //delay(150);
  } */
  
}
