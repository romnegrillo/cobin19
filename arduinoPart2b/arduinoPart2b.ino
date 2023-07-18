#include <SoftwareSerial.h>
#include <AFMotor.h>

// Initialize the motor on M1 output
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);

// Universal forward and backward delay for all motors in ms.
const int MOTOR_FORWARD_DELAY = 8000;
const int MOTOR_BACKWARD_DELAY = 8000;
const int MOTOR_PAUSE_DELAY = 1000;

void setup() {
  Serial.begin(9600);
  initMotors();
}

void initMotors() {
  // Set the speed to start, from 0 (off) to 255 (max speed)
  motor1.setSpeed(255);
  motor1.run(RELEASE);

  motor2.setSpeed(255);
  motor2.run(RELEASE);

  motor3.setSpeed(255);
  motor3.run(RELEASE);

}

void motorSequence1() {
  Serial.println("Running motor sequence 1.");

  motor1.run(FORWARD);
  delay(MOTOR_FORWARD_DELAY);

  motor1.run(RELEASE);
  delay(MOTOR_PAUSE_DELAY);

  motor1.run(BACKWARD);
  delay(MOTOR_BACKWARD_DELAY);
  
  motor1.run(RELEASE);
  sendMotorSequenceDoneCommand();
}

void motorSequence2() {
  Serial.println("Running motor sequence 2.");

  motor2.run(FORWARD);
  delay(MOTOR_FORWARD_DELAY);

  motor2.run(RELEASE);
  delay(MOTOR_PAUSE_DELAY);

  motor2.run(BACKWARD);
  delay(MOTOR_BACKWARD_DELAY);
  
  motor2.run(RELEASE);
  sendMotorSequenceDoneCommand();
}

void motorSequence3() {
  Serial.println("Running motor sequence 3.");

  motor3.run(FORWARD);
  delay(MOTOR_FORWARD_DELAY);

  motor3.run(RELEASE);
  delay(MOTOR_PAUSE_DELAY);

  motor3.run(BACKWARD);
  delay(MOTOR_BACKWARD_DELAY);

  motor3.run(RELEASE);
  sendMotorSequenceDoneCommand();
}

void sendMotorSequenceDoneCommand() {
  Serial.println("doneMotorSequence");
}

void loop() {
  if (Serial.available()) {
    String receivedString = Serial.readStringUntil('\n');
    receivedString.trim();

    Serial.println("Received string: " + receivedString);

    if (receivedString == "button1Clicked") {
      Serial.println("Button 1 clicked from Arduino 1.");

      // Run the forward and backward sequence of motor 1.
      motorSequence1();

    } else if (receivedString == "button2Clicked") {
      Serial.println("Button 2 clicked from Arduino 2.");

      // Run the forward and backward sequence of motor 2.
      motorSequence2();

    } else if (receivedString == "button3Clicked") {
      Serial.println("Button 3 clicked from Arduino 3.");

      // Run the forward and backward sequence of motor 2.
      motorSequence3();
    }
    //    else {
    //      Serial.println("Received another string: " + receivedString);
    //      // Add any action for the else statement.
    //    }
  }
}
