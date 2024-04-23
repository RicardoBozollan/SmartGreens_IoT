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


# MQTT Architecture Diagram of the Project
![1](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/1460b8b2-aad9-42cd-ac72-f825328d0f64)

# Sensors Architecture
![2](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/e3e58214-910f-499f-8244-47b1ae7bd506)


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

   ```cpp
   #include <WiFi.h>
   #include <PubSubClient.h>

   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWD";
   const char* mqttServer = "";
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
To create a Node-Red UI, the node-red-dashboard pallete was added. 
The following Flow was created:
```json
[
    {
        "id": "f9c17568.a3d348",
        "type": "tab",
        "label": "Value Comparison Flow",
        "disabled": false,
        "info": ""
    },
    {
        "id": "fbdf11cf.2f61f",
        "type": "ui_numeric",
        "z": "f9c17568.a3d348",
        "name": "Minimum Light Level",
        "label": "Maximum Light Level",
        "tooltip": "",
        "group": "b078eeb6.a7e938",
        "order": 1,
        "width": 0,
        "height": 0,
        "wrap": false,
        "passthru": true,
        "topic": "",
        "topicType": "str",
        "format": "{{value}}",
        "min": 0,
        "max": "4000",
        "step": "100",
        "className": "",
        "x": 190,
        "y": 140,
        "wires": [
            [
                "e2b4eac4.c07ff"
            ]
        ]
    },
    {
        "id": "e2b4eac4.c07ff",
        "type": "function",
        "z": "f9c17568.a3d348",
        "name": "Save Value",
        "func": "flow.set('savedLight', msg.payload);\nreturn msg;",
        "outputs": 1,
        "timeout": "",
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 450,
        "y": 140,
        "wires": [
            []
        ]
    },
    {
        "id": "ed09cc8f.873d38",
        "type": "mqtt in",
        "z": "f9c17568.a3d348",
        "name": "",
        "topic": "light_level",
        "qos": "2",
        "datatype": "auto-detect",
        "broker": "e1b270ad8d5ea72a",
        "nl": false,
        "rap": false,
        "inputs": 0,
        "x": 140,
        "y": 340,
        "wires": [
            [
                "3c0c2726.7a3b42",
                "fc3fe8b493b8f6ad",
                "f184dc545813ec20"
            ]
        ]
    },
    {
        "id": "3c0c2726.7a3b42",
        "type": "function",
        "z": "f9c17568.a3d348",
        "name": "Compare Values",
        "func": "var savedValue = flow.get('savedLight') || 0;\nvar mqttValue = msg.payload;\n\nif (savedValue < mqttValue) {\n    msg.payload = true;\n} else {\n    msg.payload = false;\n}\n\nreturn msg;",
        "outputs": 1,
        "timeout": "",
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 350,
        "y": 340,
        "wires": [
            [
                "10f68907316282c9",
                "8b1be8de5f6aa974"
            ]
        ]
    },
    {
        "id": "fc3fe8b493b8f6ad",
        "type": "debug",
        "z": "f9c17568.a3d348",
        "name": "debug 8",
        "active": false,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 360,
        "y": 460,
        "wires": []
    },
    {
        "id": "10f68907316282c9",
        "type": "debug",
        "z": "f9c17568.a3d348",
        "name": "debug 9",
        "active": false,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "payload",
        "targetType": "msg",
        "statusVal": "",
        "statusType": "auto",
        "x": 620,
        "y": 280,
        "wires": []
    },
    {
        "id": "f184dc545813ec20",
        "type": "ui_gauge",
        "z": "f9c17568.a3d348",
        "name": "",
        "group": "b078eeb6.a7e938",
        "order": 0,
        "width": 0,
        "height": 0,
        "gtype": "gage",
        "title": "Light Level",
        "label": "units",
        "format": "{{value}}",
        "min": 0,
        "max": "3500",
        "colors": [
            "#00b500",
            "#e6e600",
            "#ca3838"
        ],
        "seg1": "",
        "seg2": "",
        "diff": false,
        "className": "",
        "x": 350,
        "y": 260,
        "wires": []
    },
    {
        "id": "4e7f819def45ea91",
        "type": "ui_numeric",
        "z": "f9c17568.a3d348",
        "name": "Minimum Light Level",
        "label": "Minimum Moisture Level",
        "tooltip": "",
        "group": "b078eeb6.a7e938",
        "order": 1,
        "width": 0,
        "height": 0,
        "wrap": false,
        "passthru": true,
        "topic": "",
        "topicType": "str",
        "format": "{{value}}",
        "min": 0,
        "max": "100",
        "step": "5",
        "className": "",
        "x": 180,
        "y": 560,
        "wires": [
            [
                "d7343ceee64d7201"
            ]
        ]
    },
    {
        "id": "d7343ceee64d7201",
        "type": "function",
        "z": "f9c17568.a3d348",
        "name": "Save Value",
        "func": "flow.set('savedMoisture', msg.payload);\nreturn msg;",
        "outputs": 1,
        "timeout": "",
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 470,
        "y": 560,
        "wires": [
            []
        ]
    },
    {
        "id": "c22da9811174be89",
        "type": "mqtt in",
        "z": "f9c17568.a3d348",
        "name": "",
        "topic": "soil_moisture",
        "qos": "2",
        "datatype": "auto-detect",
        "broker": "e1b270ad8d5ea72a",
        "nl": false,
        "rap": false,
        "inputs": 0,
        "x": 150,
        "y": 720,
        "wires": [
            [
                "6a50d92344084f78"
            ]
        ]
    },
    {
        "id": "79ce55cef3c3bd73",
        "type": "function",
        "z": "f9c17568.a3d348",
        "name": "Compare Values",
        "func": "var savedValue = flow.get('savedMoisture') || 0;\nvar mqttValue = msg.payload;\n\nif (savedValue > mqttValue) {\n    msg.payload = true;\n} else {\n    msg.payload = false;\n}\n\nreturn msg;",
        "outputs": 1,
        "timeout": "",
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 620,
        "y": 720,
        "wires": [
            [
                "e73a9c96f6e7ad9c",
                "617de7431a833229",
                "62360ed1a5af29d5"
            ]
        ]
    },
    {
        "id": "13d04d7ec2d16ab9",
        "type": "debug",
        "z": "f9c17568.a3d348",
        "name": "debug 10",
        "active": false,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 560,
        "y": 840,
        "wires": []
    },
    {
        "id": "e73a9c96f6e7ad9c",
        "type": "debug",
        "z": "f9c17568.a3d348",
        "name": "debug 11",
        "active": false,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "payload",
        "targetType": "msg",
        "statusVal": "",
        "statusType": "auto",
        "x": 900,
        "y": 660,
        "wires": []
    },
    {
        "id": "9743cc3dae689805",
        "type": "ui_gauge",
        "z": "f9c17568.a3d348",
        "name": "",
        "group": "b078eeb6.a7e938",
        "order": 0,
        "width": 0,
        "height": 0,
        "gtype": "gage",
        "title": "Soil Moisture Level",
        "label": "units",
        "format": "{{value}}",
        "min": 0,
        "max": "100",
        "colors": [
            "#00b500",
            "#e6e600",
            "#ca3838"
        ],
        "seg1": "",
        "seg2": "",
        "diff": false,
        "className": "",
        "x": 630,
        "y": 640,
        "wires": []
    },
    {
        "id": "6a50d92344084f78",
        "type": "range",
        "z": "f9c17568.a3d348",
        "minin": "2100",
        "maxin": "4100",
        "minout": "100",
        "maxout": "0",
        "action": "scale",
        "round": true,
        "property": "payload",
        "name": "",
        "x": 380,
        "y": 720,
        "wires": [
            [
                "79ce55cef3c3bd73",
                "13d04d7ec2d16ab9",
                "9743cc3dae689805"
            ]
        ]
    },
    {
        "id": "617de7431a833229",
        "type": "ui_text",
        "z": "f9c17568.a3d348",
        "group": "b078eeb6.a7e938",
        "order": 0,
        "width": 0,
        "height": 0,
        "name": "",
        "label": "Water Pump",
        "format": "{{msg.payload}}",
        "layout": "row-spread",
        "className": "",
        "style": false,
        "font": "",
        "fontSize": 16,
        "color": "#000000",
        "x": 910,
        "y": 740,
        "wires": []
    },
    {
        "id": "8b1be8de5f6aa974",
        "type": "ui_text",
        "z": "f9c17568.a3d348",
        "group": "b078eeb6.a7e938",
        "order": 0,
        "width": 0,
        "height": 0,
        "name": "",
        "label": "Retractable Shade",
        "format": "{{msg.payload}}",
        "layout": "row-spread",
        "className": "",
        "style": false,
        "font": "",
        "fontSize": 16,
        "color": "#000000",
        "x": 630,
        "y": 400,
        "wires": []
    },
    {
        "id": "62360ed1a5af29d5",
        "type": "mqtt out",
        "z": "f9c17568.a3d348",
        "name": "",
        "topic": "pump_state",
        "qos": "2",
        "retain": "true",
        "respTopic": "",
        "contentType": "",
        "userProps": "",
        "correl": "",
        "expiry": "",
        "broker": "e1b270ad8d5ea72a",
        "x": 910,
        "y": 860,
        "wires": []
    },
    {
        "id": "b078eeb6.a7e938",
        "type": "ui_group",
        "name": "Control Panel",
        "tab": "56c2b56e.6d42b8",
        "order": 1,
        "disp": true,
        "width": "6",
        "collapse": false
    },
    {
        "id": "e1b270ad8d5ea72a",
        "type": "mqtt-broker",
        "name": "",
        "broker": "192.168.15.160",
        "port": "1883",
        "clientid": "",
        "autoConnect": true,
        "usetls": false,
        "protocolVersion": "4",
        "keepalive": "60",
        "cleansession": true,
        "autoUnsubscribe": true,
        "birthTopic": "",
        "birthQos": "0",
        "birthRetain": "false",
        "birthPayload": "",
        "birthMsg": {},
        "closeTopic": "",
        "closeQos": "0",
        "closeRetain": "false",
        "closePayload": "",
        "closeMsg": {},
        "willTopic": "",
        "willQos": "0",
        "willRetain": "false",
        "willPayload": "",
        "willMsg": {},
        "userProps": "",
        "sessionExpiry": ""
    },
    {
        "id": "56c2b56e.6d42b8",
        "type": "ui_tab",
        "name": "Home",
        "icon": "dashboard",
        "order": 1
    }
]
```

