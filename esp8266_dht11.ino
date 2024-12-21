#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht11(DHTPIN, DHTTYPE);

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
String URL = "http://192.168.0.102/dht11_project/text_data.php";

float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(115200);
  dht11.begin();
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  Load_DHT11_Data();
  String postData = "temperature=" + String(temperature, 1) + "&humidity=" + String(humidity, 1);

  WiFiClient client;
  HTTPClient http;
  http.begin(client, URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);
  String payload = "";

  if (httpCode > 0) {
    payload = http.getString();
    Serial.printf("Response from server: %s\n", payload.c_str());
  } else {
    Serial.printf("HTTP POST failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(5000);
}

void Load_DHT11_Data() {
  temperature = dht11.readTemperature();
  humidity =
