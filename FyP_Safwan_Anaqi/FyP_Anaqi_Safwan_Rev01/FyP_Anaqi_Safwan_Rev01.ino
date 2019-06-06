#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 2038;  //2038 for 28byj-48

//Stepper myStepper(stepsPerRevolution, 12, 11, 10, 9); //original 
Stepper stepper01(stepsPerRevolution, A3, A2, A5, A4);
Stepper stepper02(stepsPerRevolution, 12, 11, 10, 9);
Stepper stepper03(stepsPerRevolution, 13, 6, 7, 8);
Stepper stepper04(stepsPerRevolution, 2, 3, 4, 5);

void setup()
{
  stepper01.setSpeed(10);
  stepper02.setSpeed(10);
  stepper03.setSpeed(10);
  stepper04.setSpeed(10);
  
  Serial.begin(9600);
}

void loop() 
{
  stepper01.step(stepsPerRevolution);
  stepper02.step(stepsPerRevolution);
  stepper03.step(stepsPerRevolution);
  stepper04.step(stepsPerRevolution);
}
