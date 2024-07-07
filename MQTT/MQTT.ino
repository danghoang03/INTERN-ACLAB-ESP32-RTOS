#include <WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>

const char WIFI_SSID[] = "ACLAB";     // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "ACLAB2023";  // CHANGE TO YOUR WIFI PASSWORD

const char MQTT_BROKER_ADRRESS[] = "broker.hivemq.com";  // CHANGE TO MQTT BROKER'S ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "Dang-esp32-mqtt";  

const char MQTT_USERNAME[] = "HaiDang";                        // CHANGE IT IF REQUIRED, empty if not required
const char MQTT_PASSWORD[] = "DangHai";                        // CHANGE IT IF REQUIRED, empty if not required

// The MQTT topics that ESP32 should publish/subscribe
const char PUBLISH_TOPIC[] = "Dang/Led";    
const char SUBSCRIBE_TOPIC[] = "Dang/Led";  

//const int PUBLISH_INTERVAL = 5000;  // 5 seconds

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

//unsigned long lastPublishTime = 0;

//char messageBuff[100]; //use to store message

void setup() {
  Serial.begin(115200);

  pinMode(D13, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("ESP32 - Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  connectToMQTT();
}

void loop() {
  mqtt.loop();
}

void connectToMQTT() {
  // Connect to the MQTT broker
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);

  // Create a handler for incoming messages
  mqtt.onMessage(messageHandler);

  Serial.print("ESP32 - Connecting to MQTT broker");

  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (!mqtt.connected()) {
    Serial.println("ESP32 - MQTT broker Timeout!");
    return;
  }

  // Subscribe to a topic, the incoming messages are processed by messageHandler() function
  if (mqtt.subscribe(SUBSCRIBE_TOPIC))
    Serial.print("ESP32 - Subscribed to the topic: ");
  else
    Serial.print("ESP32 - Failed to subscribe to the topic: ");

  Serial.println(SUBSCRIBE_TOPIC);
  Serial.println("ESP32 - MQTT broker Connected!");
}

// void sendToMQTT() {
//   StaticJsonDocument<200> message;
//   message["timestamp"] = millis();
//   message["data"] = analogRead(0);  // Or you can read data from other sensors
//   char messageBuffer[512];
//   serializeJson(message, messageBuffer);

//   mqtt.publish(PUBLISH_TOPIC, messageBuffer);

//   Serial.println("ESP32 - sent to MQTT:");
//   Serial.print("- topic: ");
//   Serial.println(PUBLISH_TOPIC);
//   Serial.print("- payload:");
//   Serial.println(messageBuffer);
// }

void messageHandler(String &topic, String &payload) {
  if (payload == "on"){
    digitalWrite(D13, HIGH);
    mqtt.publish(PUBLISH_TOPIC, "LED is turn on");
  }
  else if(payload == "off"){
    digitalWrite(D13, LOW);
    mqtt.publish(PUBLISH_TOPIC, "LED is turn off");
  }
}
