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
   
##2. Mosquitto MQTT Broker
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
   
##3. ESP32 Configuration
##4. Node-Red Configuration
