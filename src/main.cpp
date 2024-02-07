#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

const char* ssid = "UW MPSK";
const char* password = "\\Wc3-y}3hc"; // Replace with your network password
#define DATABASE_URL "https://axi-1e5fb-default-rtdb.firebaseio.com/" // Replace with your database URL
#define API_KEY "AIzaSyBS-8Mf2KUqbLN7YqFXiRa_04VJYqmhRUc" // Replace with your API key
#define MAX_WIFI_RETRIES 5 // Maximum number of WiFi connection retries

const int trigPin = D2;
const int echoPin = D3;
const float soundSpeed = 0.034;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const unsigned long DEEP_SLEEP_INTERVAL = 30 * 1000000; // 30 seconds in microseconds
const unsigned long DETECTION_INTERVAL = 30000; // 30 seconds in milliseconds

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  int wifiCnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    wifiCnt++;
    if (wifiCnt > MAX_WIFI_RETRIES) {
      Serial.println("WiFi connection failed");
      ESP.restart();
    }
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * soundSpeed / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

void initFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase sign-up successful");
  } else {
    Serial.printf("Firebase sign-up failed: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);
}

void sendDataToFirebase(float distance) {
  if (Firebase.ready()) {
    // Upload data only if an object is detected (distance less than 30cm)
    if (distance < 30) {
      if (Firebase.RTDB.pushFloat(&fbdo, "detection", distance)) {
        Serial.println("Data sent to Firebase");
        Serial.print("PATH: ");
        Serial.println(fbdo.dataPath());
        Serial.print("TYPE: ");
        Serial.println(fbdo.dataType());
      } else {
        Serial.println("Failed to send data to Firebase");
        Serial.print("REASON: ");
        Serial.println(fbdo.errorReason());
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  connectToWiFi();
  initFirebase();
}

void loop() {
  float currentDistance = measureDistance();

  // If an object is detected (distance less than 30cm)
  if (currentDistance < 30) {
    sendDataToFirebase(currentDistance);
  }

  // Deep sleep for power saving
  Serial.println("Going to deep sleep...");
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_INTERVAL);
  esp_deep_sleep_start();
}
