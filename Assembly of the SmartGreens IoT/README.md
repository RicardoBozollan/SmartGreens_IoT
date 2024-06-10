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

   To create a Node-Red UI, the node-red-dashboard and node-red-node-mysql palette was added. The created flow can be found in the Node-Red Flow folder.
   
# STEP 6: MariaDB Configuration
   To have the history and actively track the environmental activities the plants are facing, the data being sent to node-red is going to be stored in a database. There are many options of open-source database applications, however, the operating system of the SBC needs to be compatible with the app. Many Database apps require a 64 bit system OS, such as InfluxDB.
   However, since the loaded Armbian image in the repurposed TVBox is 32 bits, MariaDB was chosen given it's compatibility. It's install and configuration required the following steps:
   
   ```bash   
   sudo apt update
   sudo apt install mariadb-server
   ```  
   After installation, it's a good idea to run the security script to set a root password and remove insecure defaults:
    
   ```bash
   sudo mysql_secure_installation
   ```
   Then, log into MariaDB and create a database and user for your Node-RED application:
   
   ```bash
   sudo mysql -u root -p

   CREATE DATABASE iot_data;
   CREATE USER 'iot_user'@'localhost' IDENTIFIED BY 'password';
   GRANT ALL PRIVILEGES ON iot_data.* TO 'iot_user'@'localhost';
   FLUSH PRIVILEGES;
   EXIT;
   ```
   Now, go back to MariaDB and create the sensor_data table:

   ```bash
   sudo mysql -u root -p

   USE iot_data;

   CREATE TABLE sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    light_level FLOAT,
    humidity FLOAT,
    timestamp DATETIME
   );
   ```

   Ensure the table was created successfully:
   
   ```bash
   SHOW TABLES;
   ```
   You should see sensor_data listed. Use ```EXIT()``` to go back from the MariaDB to the linux terminal.

   With this step done, your database is all set!
   Remember to go to the MySql node in Node-Red and configure the connection accordingly.

# STEP 7: Grafana Installation
   Grafana is a great way of visualizing and keeping track of the data.
   To install Grafana, firstly add the repository to your system:

   ```bash
   sudo apt update
   sudo apt install -y software-properties-common
   sudo add-apt-repository "deb https://packages.grafana.com/oss/deb stable main"
   ```

   Add the Grafana GPG key:

   ```bash
   wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -
   ```

  Update your package index and install Grafana:
  
  ```bash
  sudo apt update
  sudo apt install grafana
  ```

  Start and enable the Grafana service:

  ```bash
  sudo systemctl start grafana-server
  sudo systemctl enable grafana-server
  ```

  Now, with Grafana enabled and running, it should be accessible at 'http://localhost:3000' in your web browser. the default credentials are admin/admin for usernam/password, 
  and you'll be prompted to change the password.

  To add our MariaDB database as a data source, go to Configuration (gear icon) > Data Sources > Add data source, and select MySQL as the data source type.
  From there, you'll be prompted to configure the data source according to your MariaDB credentials.
  
  You can find the developed dashboard JSON on the Grafana dashboard folder. Copy it and paste on the Grafana import tab, and you should be completely set up!
  

# References

Installing NodeJs: https://www.digitalocean.com/community/tutorials/how-to-install-node-js-on-ubuntu-20-04

Installing Node-Red: https://nodered.org/docs/getting-started/local

Installing Mosquitto: https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-debian-10

