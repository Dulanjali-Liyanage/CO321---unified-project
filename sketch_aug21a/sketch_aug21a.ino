
void setup() {
  // put your setup code here, to run once:
pinMode(9, INPUT); 
Serial.begin(115200);
delay(1000);
}

void loop() {
Serial.println(digitalRead(13));
//digitalWrite(13,digitalRead(4));
delay(1000);
}
