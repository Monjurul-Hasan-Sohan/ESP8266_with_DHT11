```
# Temperature and Humidity Monitoring Project

This project uses an ESP8266 NodeMCU and a DHT11 sensor to collect temperature and humidity data. The data is then sent via HTTP POST requests to a server hosted locally, where it is stored in a MySQL database. This can be useful for remote monitoring of environmental conditions.

## Project Overview

The project consists of the following components:
1. **ESP8266 NodeMCU**: Used to read data from the DHT11 sensor and transmit it over WiFi.
2. **DHT11 Sensor**: Collects temperature and humidity data.
3. **PHP Script**: Receives data from the ESP8266 and stores it in a MySQL database.
4. **Local MySQL Database**: Stores the collected temperature and humidity data.

## Features
- Read temperature and humidity values using the DHT11 sensor.
- Send data over WiFi to a web server using HTTP POST requests.
- Store data in a MySQL database for further analysis or visualization.
- Simple PHP script for data insertion into the database.

## Components Required
- **ESP8266 NodeMCU**
- **DHT11 Temperature and Humidity Sensor**
- **Local Server (Apache, MySQL, PHP)**

## Database Setup
1. Create a MySQL database named `temperature_db`.
2. Create a table named `dht11` with the following fields:
   - `id` (Primary Key, auto-increment)
   - `temperature` (FLOAT)
   - `humidity` (FLOAT)
   - `timestamp` (DATETIME, automatically filled)

Example SQL query to create the table:
```sql
CREATE TABLE dht11 (
    id INT AUTO_INCREMENT PRIMARY KEY,
    temperature FLOAT NOT NULL,
    humidity FLOAT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

## PHP Script
A PHP script (`text_data.php`) is used to accept the incoming HTTP POST request from the ESP8266 and insert the data into the MySQL database.

```php
<?php
$hostname = "localhost";
$username = "root";
$password = "";
$database = "temperature_db";

$conn = mysqli_connect($hostname, $username, $password, $database);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

if(isset($_POST["temperature"]) && isset($_POST["humidity"])) {
    $t = $_POST["temperature"];
    $h = $_POST["humidity"];

    $sql = "INSERT INTO dht11 (temperature, humidity) VALUES (".$t.", ".$h.")";

    if (mysqli_query($conn, $sql)) {
        echo "\nNew record created successfully";
    } else {
        echo "Error: " . $sql . "<br>" . mysqli_error($conn);
    }
}
?>
```

## ESP8266 Code
The ESP8266 reads data from the DHT11 sensor and sends it to the PHP script hosted on a local server.

```cpp
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
