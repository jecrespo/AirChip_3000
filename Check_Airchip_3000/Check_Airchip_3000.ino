//Check data sent by AirChip_3000 and callback data
//Based on https://www.arduino.cc/en/Tutorial/SoftwareSerialExample

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Running Check AirChip_3000");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(19200);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  if (mySerial.available()) {
    while (mySerial.available()) {
      char c = mySerial.read();
      Serial.write(c);
      delay(50);
    }
    Serial.println();
  }
  if (Serial.available()) {
    while (Serial.available()) {
      char c = Serial.read();
      mySerial.write(c);
      delay(10);
    }
    Serial.println();
  }
}
