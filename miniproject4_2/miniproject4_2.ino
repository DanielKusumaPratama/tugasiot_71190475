#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 2. Define the API Key */
#define API_KEY "AIzaSyAuEgIlXITJif6S0OSBoe9GGBsgtPuiju8"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://danieliot-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 1. Define the WiFi credentials */
#define WIFI_SSID "realme 5 Pro"
#define WIFI_PASSWORD "99999999"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;
String path = "/miniproject4";

static const int servoPin = 13;
int ldr = A3;
bool check = true;
int servoWrite;

Servo servo1;

void setup() {
    Serial.begin(115200);
    servo1.attach(servoPin);
     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
//  auth.user.email = USER_EMAIL;
//  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  //Or use legacy authenticate method
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = "oLDNLw7ka8a62iLMpNh6HnAGpiFgVR8mWjY8aSdJ";

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop() {
  int ang = analogRead(ldr);
  int mapAng = map(ang, 0,4095, 0, 100);
  if(mapAng < 24){
    check = true;
  }
  if(Firebase.ready()){
    if(check){
      for(int posDegrees = 0; posDegrees <= 180; posDegrees+=10) {
          servo1.write(posDegrees);
          int ang = analogRead(ldr);
          int mapAng = map(ang, 0,4095, 0, 100);
          if(mapAng >= 25){
            servoWrite = posDegrees;
            Serial.println("yey");
            Serial.println(mapAng);
            Firebase.setInt(fbdo, path+"/nilai_ldr", mapAng);
            check = false;
            break;
          }
          else{
            Serial.println("no");
            Firebase.setInt(fbdo, path+"/nilai_ldr", mapAng);
          }
          Firebase.setInt(fbdo, path+"/nilai_servo", posDegrees);
          delay(1000);
      }
    }
    if(check){
      for(int posDegrees = 180; posDegrees >= 0; posDegrees-=10) {
          servo1.write(posDegrees);
          int ang = analogRead(ldr);
          int mapAng = map(ang, 0,4095, 0, 100);
          if(mapAng >= 30){
            Serial.println("yey");
            Serial.println(mapAng);
            Firebase.setInt(fbdo, path+"/nilai_ldr", mapAng);
            servoWrite = posDegrees;
            check = false;
            break;
          }
          else{
            Serial.println("no");
            Firebase.setInt(fbdo, path+"/nilai_ldr", mapAng);
          }
          Firebase.setInt(fbdo, path+"/nilai_servo", posDegrees);
          delay(1000);
      }
    }
    servo1.write(servoWrite);
  }
//    for(int posDegrees = 0; posDegrees <= 180; posDegrees+=10) {
//        servo1.write(posDegrees);
//        int ang = analogRead(ldr);
//        Serial.println(ang);
//        if(ang >= 1500){
//          Serial.println("yey");
//        }
//        else{
//          Serial.println("no");
//        }
//        delay(200);
//    }
//
//    for(int posDegrees = 180; posDegrees >= 0; posDegrees-=10) {
//        servo1.write(posDegrees);
//        int ang = analogRead(ldr);
//        Serial.println(ang);
//        if(ang >= 1500){
//          Serial.println("yey");
//        }
//        else{
//          Serial.println("no");
//        }
//        delay(200);
//    }
//  int ang = analogRead(ldr);
//  if(ang >= 1000){
//    servo1.write(180);
//  }
//  else{
//    servo1.write(0);
//  }
}

//void setup(){
//  Serial.begin(9600);
//}
//
//void loop(){
//  int ang = analogRead(ldr);
//  Serial.println(ang);
//  delay(2000);
//}
