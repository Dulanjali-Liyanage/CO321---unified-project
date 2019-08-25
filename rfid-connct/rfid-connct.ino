#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN         5         //D1 pin
#define SS_PIN          15        //D8 pin
#define IRSensor        A0        //D0 pin - ir sensor input

// Set these to run example.
#define FIREBASE_HOST "automatic-door-lock.firebaseio.com"
#define FIREBASE_AUTH "KMlSMn5A3JpY9u93aas2vjqChTuC9rhrPmTLgIBz"
#define WIFI_SSID "Dialog 4G 278"
#define WIFI_PASSWORD "7b13d7C1"

#define buzzer 16 //define red LED - connected to TX pin of nodemcu - D11

int relay_pin = 4; // define the relay input pin

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  pinMode(buzzer, OUTPUT);
  mfrc522.PCD_Init();   // Init MFRC522
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.print("WELCOME");

  pinMode( relay_pin , OUTPUT);// set the relay pin to output
  digitalWrite( relay_pin , HIGH);//close the door

  pinMode(IRSensor,INPUT); //get the input from the ir sensor
}


void loop() {

  
  //get the registered uid values from the firebase to a json object
  JsonObject& Registered_Tags = Firebase.get("/Employers").getJsonVariant().asObject();

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
 if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
 }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

//get the rfid tag
 Serial.print("UID tag: ");
 String uidTag = "";
 for(byte i = 0;i < mfrc522.uid.size;i++){
    uidTag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " "));
    uidTag.concat(String(mfrc522.uid.uidByte[i],HEX));
 }
  uidTag.toUpperCase();
  Serial.println(uidTag);

  //append a new uidTag string to /Registered_Tags
  //Firebase.pushString("Registered_Tags", uidTag);

 
  //check with the database whether uid is present or not
  int i = 1;
  //iterating through json object
  for(ArduinoJson::Internals::ListConstIterator<JsonPair> jpi = Registered_Tags.begin(); jpi != Registered_Tags.end(); ++jpi) {
    JsonPair jp = *jpi;
    JsonObject& o = jp.value.asObject();
    String n = o.get<String>("uidtag");
    String slotkey = String(jp.key); // get the key
    if (n == uidTag) {
        Serial.println("Authorized Access");

        //display in lcd
        Wire.begin(2,0);
        lcd.init(); 
        lcd.print(" Authorized Access");

        //door open
        digitalWrite( relay_pin , LOW);

        //update the whether he/she entered or not
        //Serial.println(slotkey);
        Firebase.setString("Employers/" + slotkey + "/enValue", "entered");
        //Firebase.pushString("History_Of_Entered/"+ );

        
        int count = 0;// count people
        int countTime = 0; // count the timer delay
        
        while(count < 2){
          if(analogRead(IRSensor) < 30){
            count++;
            while(analogRead(IRSensor) < 30){} // wait until a person is detected
          }
          delay(100); // delay if no unauthorized behind and authorized
          countTime++;
         
          if(countTime == 50){ // after 5000ms break while(count<2)
            break;
          }
        
        }
        //identify more people enter and 
        if(count == 2){
          
          Serial.println("more people");
          
          digitalWrite( relay_pin , HIGH); // close the door immediately 
         
          digitalWrite(buzzer,HIGH);// buzzer when more people entered
          delay(1000);
          digitalWrite(buzzer,LOW);
          delay(1000);

          ESP.restart(); //restart
          //break;
        }
       
        digitalWrite( relay_pin , HIGH); // close the door
        ESP.restart(); 
        //break;
    }
    
    if(i == Registered_Tags.size()){
        Serial.println(" Access Denied");

        //display in lcd
        Wire.begin(2,0);
        lcd.init(); 
        lcd.print(" Access Denied"); // Start Printing

        //buzzer sound
        digitalWrite(buzzer,HIGH);
        delay(1000);
        digitalWrite(buzzer,LOW);
        delay(1000);
        break;
    }
    i++;
   
 }
    
}
