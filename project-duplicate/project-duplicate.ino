#include <SPI.h>
#include <MFRC522.h> // rfid header
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h> // lcd header
#include <LiquidCrystal_I2C.h> // lcd header

#define RST_PIN         5         //D1 pin -- rfid pin
#define SS_PIN          15        //D8 pin -- rfid pin

// Set these to run code
#define FIREBASE_HOST "automatic-door-lock.firebaseio.com" // your firebase databse name
#define FIREBASE_AUTH "KMlSMn5A3JpY9u93aas2vjqChTuC9rhrPmTLgIBz" // your firebase database secret key
#define WIFI_SSID "Dialog 4G 278"  // your wifi name
#define WIFI_PASSWORD "7b13d7C1"   // your wifi password

#define IRSensor        A0  //A0 pin - ir sensor input

#define buzzer 16 //define buzzer - connected to D0 pin of nodemcu

int relay_pin = 4; // define the relay input pin D4

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

LiquidCrystal_I2C lcd(0x27, 20, 4); // you should check the address of the lcd and add it as the first parameter
                                    // other 2 parameters where your cursor is going to be initially

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

  Wire.begin(2, 0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.print("WELCOME");

  pinMode( relay_pin , OUTPUT);// set the relay pin to output
  digitalWrite( relay_pin , HIGH);//close the door

  pinMode(IRSensor, INPUT); //get the input from the ir sensor
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
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidTag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " "));
    uidTag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uidTag.toUpperCase();
  Serial.println(uidTag);

  //append a new uidTag string to /Registered_Tags
  //Firebase.pushString("Registered_Tags", uidTag);


  //check with the database whether uid is present or not
  int i = 1;

  //iterating through json object
  for (ArduinoJson::Internals::ListConstIterator<JsonPair> jpi = Registered_Tags.begin(); jpi != Registered_Tags.end(); ++jpi) {

    JsonPair jp = *jpi;
    JsonObject& o = jp.value.asObject();
    String n = o.get<String>("uidtag"); // get the uidtag in the database as a string
    String slotkey = String(jp.key); // get the key

    if (n == uidTag) { // check whether the given uidtag is equal to the uidtag in databse
      Serial.println("Authorized Access");

      //display in lcd
      Wire.begin(2, 0);
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

      while (count < 2) {
        if (analogRead(IRSensor) < 30) { // when a person is detected
          count++;
          while (analogRead(IRSensor) < 30) {} // wait until a person is detected
        }
        delay(100); // delay if no unauthorized behind and authorized
        countTime++;

        if (countTime == 50) { // after 5000ms break while(count < 2)
          break; // stop counting the people
        }

      }

      //identify more people enter and sound the buzzer
      if (count == 2) {

        Serial.println("more people");

        digitalWrite( relay_pin , HIGH); // close the door immediately

        digitalWrite(buzzer, HIGH); // buzzer when more people entered
        delay(1000);
        digitalWrite(buzzer, LOW);
        delay(1000);

        ESP.restart(); //restart the code after the closed
        //break;
      }

      digitalWrite( relay_pin , HIGH); // close the door
      ESP.restart(); //restart the code after the door closed
      //break;
    }

    if (i == Registered_Tags.size()) { // uid tag is not present inside the registered tags of the database
      Serial.println(" Access Denied");

      //display in lcd
      Wire.begin(2, 0);
      lcd.init();
      lcd.print(" Access Denied"); // Start Printing

      //buzzer sound
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      delay(1000);
      break; // break the for loop of iterating through the json object
    }
    i++; // count the number of uid tags inside the registered tags of the database

  }

}
