#include <Servo.h>

Servo myServo1;
Servo myServo2;
const int buzzPin = 7;

void resetServoPositions() {
  myServo1.write(90);
  myServo2.write(180);
}

void setup() {
  // Attach servo.
  myServo1.attach(9);
  myServo2.attach(10);

  pinMode(buzzPin, OUTPUT);

  Serial.begin(9600);

  // Wait for serial command to start the
  // system so that the servo will stay as is.
  while (true) {
    if (Serial.available()) {
      String command = Serial.readStringUntil('\n');

      if (command == "start") {
        break;
      }
    }
  }

  // Turn back servo to default position.
  resetServoPositions();
}

void loop() {
  int wasteType = 0;

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    //Serial.println("Received command: " + command);

    if (command == "paperWaste") {
      // Tilt servo1 left. Adjust the angle as per your servo setup.
      //Serial.println("Tilt servo 1 left");
      for (int i = 90; i >= 50 ; i--) {
        myServo1.write(i);
        delay(20);
      }
      wasteType = 1;

    } else if (command == "plasticWaste") {
      // Tilt servo1 right. Adjust the angle as per your servo setup.
      //Serial.println("Tilt servo 1 right");
      for (int i = 90; i <= 120 ; i++) {
        myServo1.write(i);
        delay(20);
      }
      wasteType = 2;

    } else if (command == "infectiousWaste") {
      // Turn servo2 forwards. Adjust the angle as per your servo setup.
      //Serial.println("Tilt servo 2 forwards");
      for (int i = 180; i >= 135; i--) {
        myServo2.write(i);
        delay(20);
      }
      wasteType = 3;
    } else if (command == "invalid") {
      // Turn on buzzer for certain period
      // if more than two categories is detected.
      for (int i = 0; i < 5; i++) {
        digitalWrite(buzzPin, HIGH);
        delay(500);
        digitalWrite(buzzPin, LOW);
        delay(500);
      }
    }

    // Add delay in sending for serial buffer.
    delay(1000);

    if (wasteType != 0) {
      // Add another delay to wait for the tilting.
      //Serial.println("Waiting for tilt to finish...");
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
