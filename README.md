# SmartGreens_IoT
 Smart garden with ESP32, powered by SBC like Raspberry Pi. Communication via MQTT, SBC acts as broker and hosts Node-RED. Automates care for small plants, monitoring and controlling environmental conditions remotely.
 
# Motivation
The motivation behind the SmartGreens IoT project lies in the intention to facilitate the cultivation of plants and gardens, especially for individuals with busy schedules and little time available for gardening. Through the implementation of IoT technology in embedded systems, the goal is to enable people to care for their plants remotely and automatically, eliminating the need for constant manual intervention. This provides an accessible and intuitive solution for those who wish to cultivate gardens and/or plants at home, but may lack the experience or time to do so in a traditional manner.

# Objectives
1. Develop a smart garden system based on ESP32 that is accessible and easy to assemble.

2. Integrate soil moisture, luminosity, and other sensors to monitor the environmental conditions of the garden.

3. Implement actuators such as automated irrigation systems, light control, and ventilation to adjust the environmental conditions as necessary.

4. Establish MQTT communication to send sensor data to an online dashboard, allowing remote and real-time monitoring of the garden.

5. Create a user-friendly interface for the user, allowing manual control of devices and providing insights into the garden's status and cultivation tips.

6. Document the assembly and configuration process of the system, providing resources and instructions for others to replicate the project in their own gardens.


# MQTT Architecture Diagram
![1](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/1460b8b2-aad9-42cd-ac72-f825328d0f64)


# Install and Configuration

Any embedded linux system should work (Raspeberry Pi, Orange Pi, etc), as long as it's compatible with Node-Red and Mosquitto MQTT broker.

## 1. Node-red   
   Firstly, Node-Red requires NodeJs, so it must be installed first. It can be installed via apt, with the following command:
   
   ```bash
   sudo apt update
   sudo apt install nodejs
   node -v
   ```
   If everything worked, the node -v command should output the NodeJs version.

   Now, to install node-red, use the following command:

   ```bash
   sudo npm install -g --unsafe-perm node-red
   ```
   To enable node-red on startup, use pm2, with the following commands:

   ```bash
   sudo npm install -g pm2
   pm2 start /usr/bin/node-red -- -v
   pm2 save
   pm2 startup
   ```
   Now, Node-Red is installed and will run on startup.
   
## 2. Mosquitto MQTT Broker
   To install Mosquitto, only one command is needed:
   
   ```bash
   sudo apt install mosquitto mosquitto-clients
   ```
   However, by default the newer versions of Mosquitto enable only localhost clients, so it's necessary to create a listner on port 1883 and allow anyonymous connections.
   To do the following, go to /etc/mosquitto directory:
   
   ```bash
   cd /etc/mosquitto
   ```
   Then, you're going to use a text editor, and edit some configurations on mosquitto.conf:
   
   ```bash
   sudo nano mosquitto.conf
   ```
   Add these two lines in the file:
   
   ```bash
   allow_anonymous true
   listener 1883
   ```

   Now execute the following comand, to load the new changes:
   ```bash
   sudo mosquitto -v -c mosquitto.conf
   ```
   After that, reboot your system and the broker is set up.
   
## 3. ESP32 Configuration
   Ensure that the ESP32 has the PubSubClient library installed before uploading the code. The specific developed code for the ESP32 can be found here:

   ```arduino
   #include <WiFi.h>
   #include <PubSubClient.h>

   const char* ssid = "SOKA_FUNDO_REP";
   const char* password = "perguntapronelson";
   const char* mqttServer = "192.168.15.160";
   const int mqttPort = 1883;
   const char* mqttUser = "";
   const char* mqttPassword = "";

   #define SOIL_PIN 39
   #define LDR_PIN 36
   #define RELAY_PIN 23

   WiFiClient espClient;
   PubSubClient client(espClient);

   bool wifiConnected = false;

   void setup() {
     Serial.begin(9600);
     connectToWiFi();
     pinMode(RELAY_PIN, OUTPUT);
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
       client.subscribe("pump_state"); // Subscribe to the pump_state topic
     } else {
       Serial.println("\nFailed to connect to WiFi");
     }
   }

   void reconnect() {
     Serial.println("Attempting MQTT connection...");
     if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
       Serial.println("Connected to MQTT broker");
       client.subscribe("pump_state"); // Subscribe to the pump_state topic after reconnection
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

     if (String(topic) == "pump_state") {
       if (message == "true") {
         digitalWrite(RELAY_PIN, HIGH); // Activate relay
       } else {
         digitalWrite(RELAY_PIN, LOW); // Deactivate relay
       }
     }
   }
   ```

## 4. Node-Red Configuration
