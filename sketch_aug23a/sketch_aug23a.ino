#include <SPI.h>
#include "SoftwareSerial.h" //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);

void setup() {
  pinMode(13, INPUT); 
  //Serial S Begin at 9600 Baud
  s.begin(9600);
}

void loop() {
  Serial.println(digitalRead(13));
//digitalWrite(13,digitalRead(4));
  //Write '123' to Serial
  if(digitalRead(13) == 1){
    s.write(1);
    delay(1000);
  }
}
