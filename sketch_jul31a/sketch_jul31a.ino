const int buzzer = 10;

void setup() {
  // put your setup code here, to run once:
pinMode(buzzer,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(buzzer,LOW);
delay(1000);
digitalWrite(buzzer,HIGH);
delay(2000);
}
