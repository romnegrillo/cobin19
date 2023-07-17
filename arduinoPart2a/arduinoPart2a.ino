#include <Adafruit_DotStar.h>
#include <SPI.h>

// Button pins.
const int buttonPin1 = A0;
const int buttonPin2 = A1;
const int buttonPin3 = A2;

// Assuming 5 led strip per level so 5*3*3 = 45.
#define NUMPIXELS 45
#define DATAPIN 6
#define CLOCKPIN 7

// Ultrasonic pins.
const int trigPin1 = 9, echoPin1 = 10;
const int trigPin2 = 12, echoPin2 = 13;
const int trigPin3 = 3, echoPin3 = 4;

// LED strip controller.
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

int fullDistance1 = 60;
int fullDistance2 = 60;
int fullDistance3 = 60;

int fullbin = 40;
int mediumbin = 45;
int emptybin = 50;

// Motor sealing delay in ms.
const int sealingDelay = 5000;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  strip.begin();  // Initialize pins for output
  strip.show();   // Turn all LEDs off ASAP
}

void loop() {
  // Ultrasonic, led strip listener.
  binLevelStatusListener();

  // Buttons to seal listener.
  buttonSealListener();

  delay(500);
}

void binLevelStatusListener() {
  float distance1 = calculateDistance(trigPin1, echoPin1);
  float distance2 = calculateDistance(trigPin2, echoPin2);
  float distance3 = calculateDistance(trigPin3, echoPin3);

  String level1 = calculateLevel(distance1, fullDistance1);
  String level2 = calculateLevel(distance2, fullDistance2);
  String level3 = calculateLevel(distance3, fullDistance3);

    Serial.println("Sensor 1 Level: " + level1);
    Serial.println("Sensor 2 Level: " + level2);
    Serial.println("Sensor 3 Level: " + level3);

  // Update strand colors based on the level.
  updateStrandColor(level1, 0);
  updateStrandColor(level2, 15);
  updateStrandColor(level3, 30);
}

void buttonSealListener() {
  // "!" because we made buttons INPUT_PULLUP.
  bool buttonStatus1 = !digitalRead(buttonPin1);
  bool buttonStatus2 = !digitalRead(buttonPin2);
  bool buttonStatus3 = !digitalRead(buttonPin3);

  // For testing, remove this later.
  delay(5000);
  buttonStatus1 = true;

  if (buttonStatus1) {
    // Send command to Arduino 2 to seal bin 1
    // and wait for it to finish.
    sendAndWaitMotorSequence("button1Clicked");
  }

  else if (buttonStatus2) {
    // Send command to Arduino 2 to seal bin 2.
    sendAndWaitMotorSequence("button2Clicked");
  }

  else if (buttonStatus3) {
    // Send command to Arduino 2 to seal bin 3.
    sendAndWaitMotorSequence("button3Clicked");
  }
}

void sendAndWaitMotorSequence(String command) {
  // Send the actual string command.
  Serial.println(command);

  //Wait for Arduino 2 to send "doneMotorSequence" command
  //and break out of the loop.
  while (true) {
    Serial.println("Waiting for Arduino 2 to send doneMotorSequence command...");

    if (Serial.available()) {
      String receivedString = Serial.readStringUntil('\n');
      receivedString.trim();

      Serial.println(receivedString);

      if (receivedString == "doneMotorSequence") {
        break;
      }
    }

    delay(3000);
  }
}

float calculateDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.034) / 2;  // Speed of sound wave divided by 2 (go and back)

  return distance;
}

String calculateLevel(float distance, int fullDistance) {
  if (distance < fullbin) {
    return "Full";
  } else if (distance < mediumbin) {
    return "Medium";
  } else {
    return "Low";
  }
}

void updateStrandColor(String level, int startPixel) {
  if (level == "Low") {
    for (int i = startPixel; i < startPixel + 15; i++) {
      uint32_t color;
      if (i < startPixel + 5) {
        color = strip.Color(255, 0, 0);  // Green
      } else if (i < startPixel + 10) {
        color = strip.Color(0, 0, 0);  // Off
      } else {
        color = strip.Color(0, 0, 0);  // Off
      }

      strip.setPixelColor(i, color);
    }
  } else if (level == "Medium") {
    for (int i = startPixel; i < startPixel + 15; i++) {
      uint32_t color;
      if (i < startPixel + 5) {
        color = strip.Color(0, 0, 0);  // Off
      } else if (i < startPixel + 10) {
        color = strip.Color(255, 255, 0);  // Yellow
      } else {
        color = strip.Color(0, 0, 0);  // Off
      }

      strip.setPixelColor(i, color);
    }
  } else {
    for (int i = startPixel; i < startPixel + 15; i++) {
      uint32_t color;
      if (i < startPixel + 5) {
        color = strip.Color(0, 0, 0);  // Off
      } else if (i < startPixel + 10) {
        color = strip.Color(0, 0, 0);  // Off
      } else {
        color = strip.Color(0, 255, 0);  // Red
      }

      strip.setPixelColor(i, color);
    }
  }

  strip.show();
}
