#include <WiFi.h>
#include <DHT.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* WIFI_NAME = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// Replace with your ThingSpeak API key and Channel ID
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";
// Replace with your Zapier Webhook URL
const char* zapierWebhookUrl = "YOUR_ZAPIER_WEBHOOK_URL";

// Pin configuration
const int DHTPin = 15;
const int LDRPin = 34;

// DHT sensor type
#define DHTTYPE DHT22

DHT dht(DHTPin, DHTTYPE);

// Arrays to store constant values for temperature, humidity, and LDR
const float temperatures[] = {25.5, 26.0,24.8, 25.1};
const float humidities[] = {60.0, 65.5, 58.3, 62.1};
const int ldrValues[] = {500, 520, 480, 510};

int arrayIndex = 0; // Index to iterate through the arrays

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  dht.begin();
}

void loop() {
  // Read constant values from the arrays
  float temperature = temperatures[arrayIndex];
  float humidity = humidities[arrayIndex];
  int ldrValue = ldrValues[arrayIndex];

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temperature: ");
  Serial.print(temperature);
  Serial.print("C, LDR Value: ");
  Serial.println(ldrValue);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Send data to ThingSpeak
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(humidity) + "&field2=" + String(temperature) + "&field3=" + String(ldrValue);
    http.begin(url.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("Data pushed to ThingSpeak successfully");
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      Serial.println("Failed to push data to ThingSpeak");
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(zapierWebhookUrl) + "?humidity=" + String(humidity) + "&temperature=" + String(temperature) + "&ldrValue=" + String(ldrValue);
    http.begin(url.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.println("Data sent to Zapier successfully");
    } else {
      Serial.println("Failed to send data to Zapier");
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(20000); // Upload every 20 seconds
  arrayIndex = (arrayIndex + 1) % 4; // Move to the next set of values
  delay(20000); // Upload every 20 seconds
}
