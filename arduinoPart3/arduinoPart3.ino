/*
  Notes:
  - Start with this one, it's the easiest part.
  - Just check if the IR is inverse logic.
*/

const int irPin = 2;      
const int motorPin = 3;  

void setup() {
  pinMode(irPin, INPUT);
  pinMode(motorPin, OUTPUT);

  // Start with the motor off.
  digitalWrite(motorPin, LOW);
}

void loop() {
  // "!" is added assuming the IR is inverse logic.
  int irState = !digitalRead(irPin);

  // If the IR sensor output is LOW, an object is detected.
  if (irState) {
    // Turn on the motor.
    digitalWrite(motorPin, HIGH);
  } else {
    // No object detected - turn off the motor.
    digitalWrite(motorPin, LOW);
  }
}
