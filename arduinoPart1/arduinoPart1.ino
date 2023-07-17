#include <Servo.h>

Servo myServo1;
Servo myServo2;

void resetServoPositions() {

}

void setup() {
  // Attach servo.
  // D10 servo 1.
  // D9  servo 2.

  myServo1.attach(9);
  myServo2.attach(10);

  // Turn back servo to default position.
  resetServoPositions();

  Serial.begin(9600);
}

void loop() {
  bool isWasteDetected = false;

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    Serial.println("Received command: " + command);

    if (command == "infectiousWaste") {
      // Tilt servo1 left. Adjust the angle as per your servo setup.
      Serial.println("Tilt servo 1 left");
      myServo1.write(30);
      isWasteDetected = true;

    } else if (command == "plasticWaste") {
      // Tilt servo1 right. Adjust the angle as per your servo setup.
      Serial.println("Tilt servo 1 right");
      myServo1.write(30);
      isWasteDetected = true;

    } else if (command == "paperWaste") {
      // Turn servo2 forwards. Adjust the angle as per your servo setup.
      Serial.println("Tilt servo 2 forwards");
      myServo2.write(30);
      isWasteDetected = true;
    }

    // Add delay in sending for serial buffer.
    delay(1000);

    if (isWasteDetected) {
      // Add another delay to wait for the tilting.
      Serial.println("Waiting for tilt to finish...");
      delay(3000);

      // Turn servos to default position.
      resetServoPositions();

      // Send back confirmation message.
      Serial.println("tiltDone");
    }
  }
}
