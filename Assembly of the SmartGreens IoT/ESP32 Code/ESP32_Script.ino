#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SOKANELA";
const char* password = "eutunois119";
const char* mqttServer = "192.168.15.160";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

#define SOIL_PIN 34
#define LDR_PIN 35
#define RELAY_PIN 26
#define LED_PIN 27

WiFiClient espClient;
PubSubClient client(espClient);

bool wifiConnected = false;

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!wifiConnected) {
    connectToWiFi();
  } else {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    readSensors();
  }
  delay(1000);
}

void readSensors() {
  int soil = analogRead(SOIL_PIN);
  int light = analogRead(LDR_PIN);

  client.publish("soil_moisture", String(soil).c_str());
  client.publish("light_level", String(light).c_str());
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    client.subscribe("actuators_state"); // Subscribe to the actuators_state topic
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

void reconnect() {
  Serial.println("Attempting MQTT connection...");
  if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
    Serial.println("Connected to MQTT broker");
    client.subscribe("actuators_state"); // Subscribe to the actuators_state topic after reconnection
  } else {
    Serial.print("Failed to connect to MQTT broker, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  handleActuatorsState(message);
}

void handleActuatorsState(String message) {
  if (message.length() != 2) {
    Serial.println("Invalid message length");
    return;
  }

  // Handle pump state
  if (message[0] == '1') {
    digitalWrite(RELAY_PIN, HIGH); // Activate pump
  } else if (message[0] == '0') {
    digitalWrite(RELAY_PIN, LOW); // Deactivate pump
  } else {
    Serial.println("Invalid pump state");
  }

  // Handle bulb state
  if (message[1] == '1') {
    digitalWrite(LED_PIN, HIGH); // Turn on bulb
  } else if (message[1] == '0') {
    digitalWrite(LED_PIN, LOW); // Turn off bulb
  } else {
    Serial.println("Invalid bulb state");
  }
}
