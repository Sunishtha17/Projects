#include <WiFi.h>
#include <HTTPClient.h>

// Ultrasonic Sensor Pins
#define TRIG_PIN 5
#define ECHO_PIN 18

long duration;
int distance;

// Wi-Fi Credentials
const char* ssid = "Wokwi-GUEST";  // Use "Wokwi-GUEST"
const char* password = "";         // No password needed


// ThingSpeak API
String apiKey = "MSIBXIL724P7TYTP";  // Replace with your ThingSpeak Write API Key
const char* server = "http://api.thingspeak.com/update";

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  // Measure distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.0344 / 2;  // Convert to cm

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  String parkingStatus;
  if (distance < 20) {
    parkingStatus = "OCCUPIED";
  } else {
    parkingStatus = "VACANT";
  }

  Serial.print("Parking Status: ");
  Serial.println(parkingStatus);

  sendToThingSpeak(parkingStatus);


  delay(15000);  // ThingSpeak allows updates every 15 seconds
}

// Function to send data to ThingSpeak
void sendToThingSpeak(String status) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Convert server to String before concatenation
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(distance);


    http.begin(url);
    int httpResponseCode = http.GET();

    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }
}
