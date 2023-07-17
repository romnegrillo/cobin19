#include <Servo.h>

Servo myServo1;
Servo myServo2;

void setup()
{
  // Attach servo.
  // D10 servo 1.
  // D9  servo 2.

  myServo1.attach(10);
  myServo2.attach(9);

  Serial.begin(9600);
}

void loop()
{
  myServo1.write(50);
  myServo2.write(50);
  delay(1000);
}
