#include <SoftwareSerial.h>

// Rx and Tx pins connected to Arduino 1.
SoftwareSerial mySerial(10, 11);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available()) {
    String receivedString = mySerial.readStringUntil('\n');
    receivedString.trim();
    
    if (receivedString == "button1Clicked") {
      Serial.println("Button 1 clicked from Arduino 1.");

    } else if (receivedString == "button2Clicked") {
      Serial.println("Button 2 clicked from Arduino 2.");

    }else if (receivedString == "button3Clicked") {
      Serial.println("Button 3 clicked from Arduino 3.");

    }
    //    else {
    //      Serial.println("Received another string: " + receivedString);
    //      // Add any action for the else statement.
    //    }
  }
}
