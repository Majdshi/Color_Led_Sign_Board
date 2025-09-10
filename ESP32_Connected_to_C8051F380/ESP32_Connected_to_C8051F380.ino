//ESP32 #1 (TCP Client - Separate Values)
#include <WiFi.h>

const char* ssid = "*********";
const char* password = "*******";

const char* server_ip = "10.0.0.14"; // ESP32 #2 IP
const int server_port = 5000;

WiFiClient client;

// Variables
uint8_t second_, minute_, hour_, day_, date_, month_, year_;

void connectToServer() {
  Serial.print("Connecting to server...");
  if (client.connect(server_ip, server_port)) {
    Serial.println("Connected!");
  } else {
    Serial.println("Connection failed");
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17

  // Wi-Fi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");

  // TCP connect
  connectToServer();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(2000);
    return;
  }

  if (!client.connected()) {
    connectToServer();
    delay(2000);
    return;
  }

  if (Serial2.available() >= 11) {
    second_ = Serial2.read();
    minute_ = Serial2.read();
    hour_   = Serial2.read();
    day_    = Serial2.read();
    date_   = Serial2.read();
    month_  = Serial2.read();
    year_   = Serial2.read();

    uint8_t hum_H = Serial2.read();
    uint8_t hum_L = Serial2.read();
    uint8_t temp_H = Serial2.read();
    uint8_t temp_L = Serial2.read();

    // Optional: reconstruct values
    uint16_t humidity = (hum_H << 8) | hum_L;
    int16_t temp = (temp_H << 8) | temp_L;

    // Send full CSV with time, date, humidity, temp
    client.printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                  second_, minute_, hour_, day_, date_, month_, year_,
                  humidity, temp);

    Serial.printf("Sent: %d,%d,%d,%d,%d,%d,%d | Hum=%d | Temp=%.1f\n",
                  second_, minute_, hour_, day_, date_, month_, year_,
                  humidity, temp / 10.0f);
}

  delay(500);
}
