int incomingByte = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() 
{
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

    if(incomingByte == 169 && digitalRead(4)== LOW)
    {
      Serial.println("ON");
      digitalWrite(4, HIGH);
      tone(5, 1000);
    }
    else if(incomingByte == 169 && digitalRead(4)== HIGH)
    {
      Serial.println("OFF");
      digitalWrite(4, LOW);
      noTone(5);
    }
    /*
    if(incomingByte == 48)
    {
      Serial.println("OFF");
      digitalWrite(4, LOW);
      noTone(5);
    } */
  }
}
