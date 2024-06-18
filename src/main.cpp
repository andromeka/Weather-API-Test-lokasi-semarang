#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Put your WiFi Credentials here
const char* ssid = "Tamaki";
const char* password = "wunangcepe";

// URL Endpoint for the API
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "9a553a4d189678d8d2945eee265c3133";

// Replace with your location Credentials
String lat = "-6.981779358476813";
String lon = "110.41328171734197";

void setup() {
  Serial.begin(115200);
  
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // Set HTTP Request Final URL with Location and API key information
    http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);
    
    // start connection and send HTTP Request
    int httpCode = http.GET();
    
    // httpCode will be negative on error
    if (httpCode > 0) {
      // Read Data as a JSON string
      String JSON_Data = http.getString();
      Serial.println("Raw JSON data:");
      Serial.println(JSON_Data);
      
      // Retrieve some information about the weather from the JSON format
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, JSON_Data);
      
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      } else {
        // Extract and display the Current Weather Info
        const char* city = doc["name"];
        const char* country = doc["sys"]["country"];
        const char* description = doc["weather"][0]["description"];
        float temp = doc["main"]["temp"];
        float humidity = doc["main"]["humidity"];
        
        Serial.println("\nCurrent Weather Information:");
        Serial.print("Location: ");
        Serial.print(city);
        Serial.print(", ");
        Serial.println(country);
        Serial.print("Description: ");
        Serial.println(description);
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.println(" °C");
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
      }
    } else {
      Serial.println("Error: Unable to fetch weather data");
    }
    http.end();
  }
  
  // Wait for 30 seconds before next update
  Serial.println("\nWaiting 30 seconds before next update...");
  delay(30000);
}