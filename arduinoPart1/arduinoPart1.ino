/*
  Notes:
  - Power the servo on a separate 5V power supply, not the 5V output on Arduino.
  - Then just put a common Gnd on each. 
  - The signal pin of the servo can use any digital pins.
*/


#include <Servo.h>

const int servoPin1 = 9;
const int servoPin2 = 10;

Servo servo1;
Servo servo2;

void setup() {
  servo1.attach(servoPin1);  // attaches the servo on pin 9
  servo2.attach(servoPin2);  // attaches the servo on pin 10

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');

    if (command == "infectiousWaste") {
      // Tilt servo1 left. Adjust the angle as per your servo setup.
      servo1.write(30);   

    } else if (command == "plasticWaste") {
      // Tilt servo1 right. Adjust the angle as per your servo setup.
      servo1.write(150);   

    } else if (command == "paperWaste") {
      // Turn servo2 left. Adjust the angle as per your servo setup.
      servo2.write(30);   
    }

    // Wait for 3 seconds.
    delay(3000);

    // Send back confirmation message.
    Serial.println("tiltDone");
    /
  }
}
