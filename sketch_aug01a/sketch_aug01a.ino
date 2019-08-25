#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          15         // Configurable, see typical pin layout above

// Set these to run example.
#define FIREBASE_HOST "automatic-door-lock.firebaseio.com"
#define FIREBASE_AUTH "KMlSMn5A3JpY9u93aas2vjqChTuC9rhrPmTLgIBz"
#define WIFI_SSID "Dialog 4G 278"
#define WIFI_PASSWORD "7b13d7C1"

#define buzzer 16 //define red LED - connected to D0 pin of nodemcu

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
   //lcd.print(" Hello Makers "); // Start Printing
  
}


void loop() {

  //get the registered uid values from the firebase to a json object
const ArduinoJson::JsonObject& Registered_Tags = Firebase.get("Employers").getJsonVariant().asObject();

 //Serial.print("After get ");

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

int i = 1;

    for(ArduinoJson::Internals::ListConstIterator<JsonPair> jpi = Registered_Tags.begin(); jpi != Registered_Tags.end(); ++jpi) {
    JsonPair jp = *jpi;
    JsonObject& o = jp.value.asObject();
    String n = o.get<String>("uidtag");
    //Serial.println(n);
    if (n == uidTag) {
        //Serial.println(String(jp.key));
        Serial.println("Authorized Access");
        Wire.begin(2,0);
      lcd.init(); 
      lcd.print(" Authorized access");
        break;
    }

     if(i == Registered_Tags.size()){
        Serial.println("Authorized denied");
        Wire.begin(2,0);
        lcd.init(); 
        lcd.print("Authorized denied"); // Start Printing
        digitalWrite(buzzer,HIGH);
        delay(1000);
        digitalWrite(buzzer,LOW);
        delay(1000);
        break;
    }
    i++;
   
 }

 


    
}
