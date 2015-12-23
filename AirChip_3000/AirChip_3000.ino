//Arduino Software Serial communication with MAX485 to AirChip 3000
//Based on https://arduino-info.wikispaces.com/SoftwareSerialRS485Example
//More applications with MAX485 https://arduino-info.wikispaces.com/RS485-Modules
//
//HW used:
//RS-485 module for Arduino (MAX485) http://yourduino.com/sunshop2/index.php?l=product_detail&p=323
//HygroClip 2 (HC2) Probe with AirChip 3000
//
//Datasheets:
//MAX485 https://www.maximintegrated.com/en/products/interface/transceivers/MAX485.html
//MAX485 http://datasheets.maximintegrated.com/en/ds/MAX1487-MAX491.pdf
//
//Other HW:
//RS485 Shield V2 https://www.sparkfun.com/products/12965
//
//Other SW:
//RS485-Arduino-Library https://github.com/Protoneer/RS485-Arduino-Library
//Modbus Libray for Linux: http://libmodbus.org/
//
//More info for 485 communications: http://www.gammon.com.au/forum/?id=11428
//
//Schema:
//Arduino -- MAX485 -- RS485 -- AirChip 3000 -- Higroclip HC2
//ASCCII commands through RS485
//GENERAL COMMAND STRUCTURE
//RS485 „{“ ID Adr Command Data Chksum CR
//RS485 | (vertical bar) this character is required only when the string is to be passed to
//another device within a RS-485 multi-drop.
//{ Beginning of the string (curly bracket)
//ID Device type identifier (one ASCII character)
//ADR RS485 Address of the device (00-64)
//Command 3 ASCII characters
//Data ASCII characters, each individual data block is followed by a semi-colon (separator)
//Chksum Checksum or curly bracket }
//CR End of the string (Carriage Return)
//* Each individual data block, including the last, is followed by a semi-colon (separator)
//* Leading and trailing space characters are allowed
//* The decimal symbol is always a dot

////////// LIBRARIES //////////
#include <SoftwareSerial.h>

////////// DEFINITIONS //////////
#define SSerialRX        12  //Serial Receive pin
#define SSerialTX        13  //Serial Transmit pin
#define SSerialTxControl 3  //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define MAX_BUFFER_SIZE 64  //Maximum Buffer size for send or recieve

////////// OBJECTS //////////
SoftwareSerial RS485Serial(SSerialRX, SSerialTX);

////////// CONSTANTS AND VARIABLES //////////
char serial_buffer[MAX_BUFFER_SIZE];

void setup() {
  Serial.begin(9600);
  Serial.println("Send Commands and receive data through RS485 BUS.");
  Serial.println("Use Serial Monitor, type in upper window 0 + ENTER for menu");
  Serial.println("Use option 'without adjustment line' on Arduino Serial Monitor");
  Serial.println();
  Serial.println();
  pinMode(SSerialTxControl, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  RS485Serial.begin(19200);   // set the data rate for AirChip 3000
}

void loop() {
  if (Serial.available()) {
    int i = 0;
    while (Serial.available()) {
      serial_buffer[i] = Serial.read();
      delay(10);
      i++;
    }
    serial_buffer[i] = '\0';
    menu_serial(serial_buffer);
  }

  if (RS485Serial.available()) {
    int i = 0;
    while (RS485Serial.available()) {
      digitalWrite(LED_BUILTIN, HIGH);  // Show activity RS485
      serial_buffer[i] = RS485Serial.read();
      delay(50);
      i++;
      digitalWrite(LED_BUILTIN, LOW);  // Show activity RS485
    }
    serial_buffer[i] = '\0';
    Serial.println("Recieved from RS485:");
    Serial.println(serial_buffer);
  }
}

////////// FUNCTIONS //////////
void menu_serial (char *serial_buffer) {
  if (strcmp ("0", serial_buffer) == 0) {
    Serial.println("Type 1: Send read value to the probe: \"|{ 99RDD}\r\"");
    Serial.println("Type 2: Send your own command with pattern: {ID+Adr+RDD}. \r is added automatically");
    Serial.println("Type 0: This help ");
    Serial.println();
    Serial.println();
  }
  if (strcmp ("1", serial_buffer) == 0) {
    Serial.println("Send Read Command");
    serial_buffer = "|{99RDD}\r\0";
    //Send buffer to RS485
    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
    digitalWrite(LED_BUILTIN, HIGH);  // Show activity
    RS485Serial.print(serial_buffer);
    digitalWrite(LED_BUILTIN, LOW);  // Show activity
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
  }
  if (strcmp ("2", serial_buffer) == 0) {
    Serial.print("Type command to send RS485");
    int i = 0;
    while (!Serial.available()) {
      Serial.print(".");
      delay(1000);
    }
    while (Serial.available()) {
      serial_buffer[i] = Serial.read();
      delay(10);
      i++;
    }
    Serial.println();
    Serial.println();
    serial_buffer[i] = '\r';
    i++;
    serial_buffer[i] = '\0';
    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
    digitalWrite(LED_BUILTIN, HIGH);  // Show activity
    RS485Serial.print(serial_buffer);
    digitalWrite(LED_BUILTIN, LOW);  // Show activity
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
    Serial.print("Command Sent to RS485: ");
    Serial.println(serial_buffer);
  }
}
