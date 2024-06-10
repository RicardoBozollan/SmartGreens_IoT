### This page will teach the assembly process of the project, from circuitry to the configurations for establishing MQTT communication.

# STEP 1: Connect the sensors and relay to the ESP32.

### Note:
In this project, a 1 Channel Relay Module 12V is used to simulate a water pump that would be used to irrigate the garden/plant and a LED to simulate an auxiliary light.

### Materials Needed:
- ESP32
- Single-board computer (SBC)
- Light Dependent Resistor (LDR)
- 2 Resistors of 2.2K ohms
- Soil Moisture Sensor
- 1 Channel Relay Module 12V
- 1 LED

 The image below shows the pinout of an ESP32.

![ESP32-Pinout](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/0ef3eec4-5258-40cb-9af9-7b21e8751342)


### Assemble the ESP32 to Soil Moisture Sensor

Connect the soil moisture sensor as shown in the following image.

![imagem_2024-06-09_221257523](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/4d295f89-3269-40ce-ac98-683cf3f0cb86)

Soil moisture sensor pinout:

| SOIL MOISTURE SENSOR | VCC | GND | D0 | A0 |
|----------|----------|----------|----------|----------|
| ESP32  | +3.3 V  | GND  |  -  |  D34  |



### Assemble the ESP32 to LDR

Connect the LDR with an resistor of 10K ohms to ESP32 as shown in the following image.

![imagem_2024-06-09_222346550](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/140221e2-0c48-4871-8cf3-e479d82d89d1)

### Assemble the ESP32 to the Relay Module

Connect the 1 Channel Relay Module 12V to ESP32 as shown in the following image:

![imagem_2024-06-09_224215478](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/63e6a988-79b4-48cc-a34e-a43c13aed2f4)

Relay Module pinout:

| 1 Channel Relay Module 12V | VCC | GND | IN |
|----------|----------|----------|----------|
| ESP32  | +3.3 V  | GND  |  D26  |


### Assemble the ESP32 to LED

Connect the LED with an resistor of 2.2K ohms to ESP32 as shown in the following image.

![imagem_2024-06-09_225739075](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/a204bc88-426f-4a77-b51a-6c3ce464ee01)


### Circuit assembled

The image below shows how the circuit should be assembled for the execution of this project.

![imagem_2024-06-09_230537498](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/e9c5146c-112e-4244-930f-804483e10a55)


# STEP 2:Node-RED Installation

Any embedded linux system should work (Raspeberry Pi, Orange Pi, etc), as long as it's compatible with Node-Red and Mosquitto MQTT broker.
For our version of this project, a TV BOX running Armbian was used.
   
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
   
# STEP 3: Mosquitto MQTT Broker
   To install Mosquitto, only one command is needed:
   
   ```bash
   sudo apt install mosquitto mosquitto-clients
   ```
   However, by default the newer versions of Mosquitto enable only localhost clients, so it's necessary to create a listener on port 1883 and allow anyonymous connections.
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
   
# STEP 4: ESP32 Configuration Code
   Ensure that the ESP32 has the PubSubClient library installed before uploading the code. The specific developed code for the ESP32 can be found in the ESP32 code folder.
   
# STEP 5: Node-Red Configuration
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

# References

Installing NodeJs: https://www.digitalocean.com/community/tutorials/how-to-install-node-js-on-ubuntu-20-04

Installing Node-Red: https://nodered.org/docs/getting-started/local

Installing Mosquitto: https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-debian-10

