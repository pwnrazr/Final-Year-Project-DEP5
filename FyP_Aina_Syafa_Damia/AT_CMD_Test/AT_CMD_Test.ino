void setup() 
{
  Serial.begin(9600);
  Serial3.begin(9600);

}

void loop() 
{
  if(Serial.available() > 0)
  {
    Serial3.write(Serial.read());
  }
  
  if(Serial3.available() > 0)
  {
    Serial.write(Serial3.read());
  }
}
