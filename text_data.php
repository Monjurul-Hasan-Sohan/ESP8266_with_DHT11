CREATE TABLE dht11 (
    id INT AUTO_INCREMENT PRIMARY KEY,
    temperature FLOAT NOT NULL,
    humidity FLOAT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

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
