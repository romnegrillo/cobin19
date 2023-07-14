const int irPin = 2;      
const int motorPin = 3;  

void setup() {
  pinMode(irPin, INPUT);
  pinMode(motorPin, OUTPUT);

  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // Start with the motor off.
  digitalWrite(motorPin, LOW);
  Serial.println("Motor is OFF at the start");
}

void loop() {
  // "!" is added assuming the IR is inverse logic.
  int irState = !digitalRead(irPin);

  // If the IR sensor output is LOW, an object is detected.
  if (irState) {
    // Turn on the motor.
    digitalWrite(motorPin, HIGH);
    Serial.println("Object detected, Motor is ON");
    delay(2000);
  } else {
    // No object detected - turn off the motor.
    digitalWrite(motorPin, LOW);
    Serial.println("No object detected, Motor is OFF");
  }
  
  // Print the state of the IR sensor
  Serial.print("IR state: ");
  Serial.println(irState);
  delay(2000);
}
