#include <SPI.h>
#include <NTPClient.h> // header for time and date
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h> // lcd header
#include <LiquidCrystal_I2C.h> // lcd header
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(D5, D6); // RX, TX

StaticJsonBuffer<200> jsonBuffer;

// Set these to run code
#define FIREBASE_HOST "automatic-door-lock.firebaseio.com" // your firebase databse name
#define FIREBASE_AUTH "KMlSMn5A3JpY9u93aas2vjqChTuC9rhrPmTLgIBz" // your firebase database secret key
#define WIFI_SSID "Dialog 4G 278"  // your wifi name
#define WIFI_PASSWORD "7b13d7C1"   // your wifi password

#define IRSensor    A0  //A0 pin - ir sensor input

#define buzzer      16 //define buzzer - connected to D0 pin of nodemcu

#define relay_pin    4 // define the relay input pin D2

const long utcOffsetInSeconds = 19800; // set the utc for the region -- For UTC +5.30 : 5.5 * 60 * 60 : 19800


LiquidCrystal_I2C lcd(0x27, 16, 2); // you should check the address of the lcd and add it as the first parameter
                                    // other 2 parameters where your cursor is going to be initially

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;


void setup() {
  
  Serial.begin(9600);   // Initialize serial communications with the PC

  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);
  delay(100);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  Serial.println("Waiting for valid finger..."); 

  Wire.begin(2, 0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
    
  
  //SPI.begin();      // Init SPI bus

  //connect to wifi.
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

  pinMode( relay_pin , OUTPUT);// set the relay pin to output
  digitalWrite( relay_pin , HIGH);//close the door

  pinMode(IRSensor, INPUT); //get the input from the ir sensor

  timeClient.begin(); // begin the time calculation
 
}

void loop(){

  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("WELCOME");

  lcd.setCursor(0,1);
  lcd.print("Your Fingerprint");
  
  
  //get the registered data from the firebase to a json object
  JsonObject& Registered_Tags = Firebase.get("/Employers").getJsonVariant().asObject();

  int f = getFingerprintIDez();

  String finger_id = String(f);

  if(f < 0){
   return; 
  }

    //check with the database whether uid is present or not
    int i = 1;

    //iterating through json object
    for (ArduinoJson::Internals::ListConstIterator<JsonPair> jpi = Registered_Tags.begin(); jpi != Registered_Tags.end(); ++jpi) {

      JsonPair jp = *jpi;
      JsonObject& o = jp.value.asObject();
      String n = o.get<String>("fingerprint_id"); // get the fingerprint_id in the database
      String username = o.get<String>("username");
      String uidtag = o.get<String>("uidtag");
      String slotkey = String(jp.key); // get the key

      if(n == finger_id){
        
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Access Granted");
        delay(1000);

        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("Welcome ");
        lcd.setCursor(2,1);
        lcd.print(username);

        //door open
        digitalWrite( relay_pin , LOW);

        int count = 0;// count people
        int countTime = 0; // count the timer delay
  
        while (count < 2) {
          if (analogRead(IRSensor) < 30) { // when a person is detected
            count++;
            while (analogRead(IRSensor) < 30) {} // wait until the person is enter through the door
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
  
          timeClient.update(); // update the time calculation
          Firebase.pushString("Re_Access_Denied", timeClient.getFormattedDate());
  
          digitalWrite( relay_pin , HIGH); // close the door immediately
  
          //display in lcd
          
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print("Access Denied"); // Start Printing
  
          digitalWrite(buzzer, HIGH); // buzzer when more people entered
          delay(1000);
          digitalWrite(buzzer, LOW);
          delay(1000);
          
        }

        timeClient.update(); // update the time calculation

      // The formattedDate comes with the following format:
      // 2018-05-28T16:00:13Z
      // We need to extract date and time
      formattedDate = timeClient.getFormattedDate();
      Serial.println(formattedDate);

      // Extract date
      int splitT = formattedDate.indexOf("T");
      dayStamp = formattedDate.substring(0, splitT);
      Serial.print("DATE: ");
      Serial.println(dayStamp);

      // Extract time
      timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
      Serial.print("HOUR: ");
      Serial.println(timeStamp);
      
      Firebase.setString("Employers/" + slotkey + "/re_enter_date", dayStamp); // update the employer entered date in the firebase database
      Firebase.setString("Employers/" + slotkey + "/re_enter_time", timeStamp); // update the employer entered time in the firebase database
      Firebase.pushString("Re_History_Of_Entered/",finger_id + "u" + uidtag + "u" + dayStamp + "u" + timeStamp); // update the history

      digitalWrite( relay_pin , HIGH); // close the door
      return;
      
      }

      if (i == Registered_Tags.size()) { // uid tag is not present inside the registered tags of the database
        
        return;
      }
      i++; // count the number of uid tags inside the registered tags of the database
      
    }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {

  
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK){
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
    Serial.println("NO MATCH");
  

     //display in lcd
     lcd.clear();
     lcd.setCursor(2,0);
     lcd.print("NO MATCH"); // Start Printing

     
  
     //buzzer sound
     digitalWrite(buzzer, HIGH);
     delay(500);
     digitalWrite(buzzer, LOW);
     delay(2000);
  
    return -1;
  }
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  
  return finger.fingerID; 
}
