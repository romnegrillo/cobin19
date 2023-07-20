#include <Adafruit_DotStar.h>
#include <SPI.h>

// Button pins.
const int buttonPin1 = A0;
const int buttonPin2 = A2; 
const int buttonPin3 = A1;

// Assuming 5 led strip per level so 5*3*3 = 45.
#define NUMPIXELS 45
#define DATAPIN 6
#define CLOCKPIN 7

// Ultrasonic pins.
const int trigPin1 = 9, echoPin1 = 10;
const int trigPin2 = 12, echoPin2 = 13;
const int trigPin3 = 3, echoPin3 = 4;

//
String level1 = "";
String level2 = "";
String level3 = "";

// LED strip controller.
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

int fullDistance1 = 40;
int fullDistance2 = 40;
int fullDistance3 = 40;

int fullbin = 20;
int mediumbin = 30;
int emptybin = 40;

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

  level1 = calculateLevel(distance1, fullDistance1);
  level2 = calculateLevel(distance2, fullDistance2);
  level3 = calculateLevel(distance3, fullDistance3);

//  Serial.println("Distance 1: " + String(distance1));
//  Serial.println("Distance 2: " + String(distance2));
//  Serial.println("Distance 3: " + String(distance3));
//  Serial.println();

  Serial.println("Sensor 1 Level: " + level1);
  Serial.println("Sensor 2 Level: " + level2);
  Serial.println("Sensor 3 Level: " + level3);
  Serial.println();

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

  Serial.println("Button states :");
  Serial.println(buttonStatus1) ;
  Serial.println(buttonStatus2);
  Serial.println(buttonStatus3);
  Serial.println();


  // For testing, remove this later.
  //  delay(5000);
  //  buttonStatus1 = true;
  //Serial.println("button Listener");

  if (buttonStatus3 && level3 == "Full" ) {
    // Send command to Arduino 2 to seal bin 1
    // and wait for it to finish.
    Serial.println("button 1 test");
    sendAndWaitMotorSequence("button1Clicked");
  }

  else if (buttonStatus2 && level2 == "Full" ) {
    // Send command to Arduino 2 to seal bin 2.
    Serial.println("button 2 test");
    sendAndWaitMotorSequence("button2Clicked");
  }

  else if (buttonStatus1 && level1 == "Full" ) {
    // Send command to Arduino 2 to seal bin 3.
    Serial.println("button 3 test");
    sendAndWaitMotorSequence("button3Clicked");
  }
  else {
    //Serial.println("NO BUTTON CLICKED");
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
        color = strip.Color(0, 0, 0);  // Off
      } else if (i < startPixel + 10) {
        color = strip.Color(0, 0, 0);  // Off
      } else {
        color = strip.Color(255, 0, 0);  // Green
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
        color = strip.Color(255, 255, 0);  // Yellow
      }

      strip.setPixelColor(i, color);
    }
  } else {
    for (int i = startPixel; i < startPixel + 15; i++) {
      uint32_t color;

      if (i < startPixel + 5) {
        color = strip.Color(0, 255, 0);  // Red
      } else if (i < startPixel + 10) {
        color = strip.Color(0, 255, 0);  // Red
      } else {
        color = strip.Color(0, 255, 0);  // Red
      }
      strip.setPixelColor(i, color);
    }
  }

  strip.show();
}
