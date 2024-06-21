#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <SoftwareSerial.h>
#include <ESP32Servo.h>

// Set these to run example.
#define FIREBASE_HOST "https://final-project-applicatio-d2f82-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "ehqbp3ieW9QyW7aJlqTu3t130qDAr8j1oQp37wch"
#define WIFI_SSID "Mete"
#define WIFI_PASSWORD "95896624"

const int Software_RX = 18;
const int Software_TX = 19;

const int ledPin1 = 25;
const int ledPin2 = 26;
const int ledPin3 = 27;
const int servoPin = 12;
  
const int trigPin = 16;
const int ecoPin = 17;

SoftwareSerial BT_HC06(Software_RX,Software_TX); 
Servo servoMotor;

void setup() {
  Serial.begin(9600);
  BT_HC06.begin(9600);  // bt module start
  
  servoMotor.attach(servoPin);
  
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(ecoPin, INPUT);

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

  Firebase.setString("LED1/State", "OFF");
  Firebase.setString("LED2/State", "OFF");
  Firebase.setString("LED3/State", "OFF");
  Firebase.setInt("SERVO/Angle", 0);
  Firebase.setFloat("DISTANCE", 0);

}

void loop() {
 
 if(Firebase.getString("LED1/State") == "ON"){
   digitalWrite(ledPin1,HIGH);
 }    
 else{
   digitalWrite(ledPin1,LOW);
 }

if(Firebase.getString("LED2/State") == "ON"){
  digitalWrite(ledPin2,HIGH);
}
else{
  digitalWrite(ledPin2,LOW);
}

if(Firebase.getString("LED3/State") == "ON"){
  digitalWrite(ledPin3,HIGH);
}
else{
  digitalWrite(ledPin3,LOW);
}  

// Distance measure sensor
// Triggering
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the duration
  unsigned long duration = pulseIn(ecoPin, HIGH);
  unsigned long distance = 0.0343*duration/2;

  Firebase.setFloat("DISTANCE", distance); // send distance sensor value to firebase DISTANCE key
  
  // send distance data to BT terminal 
  BT_HC06.write("Distance: ");
  BT_HC06.print((distance));
  BT_HC06.write(" cm");
  BT_HC06.write("\n");
  
  
  Serial.print("Distance: ");
  Serial.println((distance));
  Serial.print("cm: ");
  
  
  // get servo angle value from db
  int servoAngle = Firebase.getInt("SERVO/Angle");
  servoMotor.write(servoAngle);

  delay(100);
}
