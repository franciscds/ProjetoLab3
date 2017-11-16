#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port conect.
  }
  Serial.println("Settings!");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available()){
    Serial.write(mySerial.read());
  }
  if (Serial.available()){
    mySerial.write(Serial.read());
  }
}
