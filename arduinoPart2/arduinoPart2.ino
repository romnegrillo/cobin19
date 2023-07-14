/*
  Notes:
  - Test the ultrasonic first if the value in cm is correct.
  - Then test if the level is correct, low, medium and full.
  - Run the strand test code for the led strip first.
  - Try to run the updateStrandColor function if it works as expected.
  - If everything looks great, try to run the whole code. 
*/

#include <Adafruit_DotStar.h>
#include <SPI.h>

// Assuming 5 led strip per level so 5*3*3 = 45.
#define NUMPIXELS 45
#define DATAPIN 4
#define CLOCKPIN 5

const int trigPin1 = 9, echoPin1 = 10;
const int trigPin2 = 11, echoPin2 = 12;
const int trigPin3 = 13, echoPin3 = A0;

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

int fullDistance1 = 30;
int fullDistance2 = 30;
int fullDistance3 = 30;

void setup() {
  Serial.begin(9600);
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
  float distance1 = calculateDistance(trigPin1, echoPin1);
  float distance2 = calculateDistance(trigPin2, echoPin2);
  float distance3 = calculateDistance(trigPin3, echoPin3);

  String level1 = calculateLevel(distance1, fullDistance1);
  String level2 = calculateLevel(distance2, fullDistance2);
  String level3 = calculateLevel(distance3, fullDistance3);

  updateStrandColor(level1, 0);
  updateStrandColor(level2, 15);
  updateStrandColor(level3, 30);

  Serial.println("Sensor 1 Level: " + level1);
  Serial.println("Sensor 2 Level: " + level2);
  Serial.println("Sensor 3 Level: " + level3);

  delay(500);
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
  if (distance < fullDistance / 3) {
    return "Low";
  } else if (distance < (2 * fullDistance) / 3) {
    return "Medium";
  } else {
    return "Full";
  }
}

void updateStrandColor(String level, int startPixel) {
  uint32_t color;
  if (level == "Low") {
    color = strip.Color(0, 255, 0);  // Green
  } else if (level == "Medium") {
    color = strip.Color(255, 255, 0);  // Yellow
  } else {
    color = strip.Color(255, 0, 0);  // Red
  }

  for (int i = startPixel; i < startPixel + 15; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}