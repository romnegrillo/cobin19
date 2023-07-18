#include <Servo.h>

Servo myServo1;
Servo myServo2;

void resetServoPositions() {
  myServo1.write(90);
  myServo2.write(180);

  Serial.println("Servo at 90 degrees.");
}

void setup() {
  // Attach servo.
  // D10 servo 1.
  // D9  servo 2.

  myServo1.attach(9);
  myServo2.attach(10);

  Serial.begin(9600);

  // Turn back servo to default position.
  resetServoPositions();
}

void loop() {
  int wasteType = 0;

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    Serial.println("Received command: " + command);

    if (command == "infectiousWaste") {
      // Tilt servo1 left. Adjust the angle as per your servo setup.
      Serial.println("Tilt servo 1 left");
      for (int i = 90; i >= 50 ; i--) {
        myServo1.write(i);
        delay(20);
      }
      wasteType = 1;

    } else if (command == "plasticWaste") {
      // Tilt servo1 right. Adjust the angle as per your servo setup.
      Serial.println("Tilt servo 1 right");
      for (int i = 90; i <= 120 ; i++) {
        myServo1.write(i);
        delay(20);
      }
      wasteType = 2;

    } else if (command == "paperWaste") {
      // Turn servo2 forwards. Adjust the angle as per your servo setup.
      Serial.println("Tilt servo 2 forwards");
      for (int i = 180; i >= 135; i--) {
        myServo2.write(i);
        delay(20);
      }
      wasteType = 3;
    }

    // Add delay in sending for serial buffer.
    delay(1000);

    if (wasteType != 0) {
      // Add another delay to wait for the tilting.
      Serial.println("Waiting for tilt to finish...");
      delay(3000);

      if (wasteType == 1) {
        for (int i = 50; i <= 90; i++) {
          myServo1.write(i);
          delay(20);
        }
      } else if (wasteType == 2) {
        for (int i = 120; i >= 90; i--) {
          myServo1.write(i);
          delay(20);
        }
      } else if (wasteType == 3) {
        for (int i = 135; i <= 180; i++) {
          myServo2.write(i);
          delay(20);
        }
      }




      // Send back confirmation message.
      Serial.println("tiltDone");
      wasteType = 0;
    }
  }
}
