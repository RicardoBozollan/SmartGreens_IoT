# STEP 1: Connect the sensors and relay to the ESP32.

### Materials Needed:
- ESP32
- Single-board computer (SBC)
- Light Dependent Resistor (LDR)
- Resistor 10K ohms
- Soil Moisture Sensor
- 1 Channel Relay Module 12V

 The image below shows the pinout of an ESP32.

![ESP32-Pinout](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/0ef3eec4-5258-40cb-9af9-7b21e8751342)


### Assemble the ESP32 to Soil Moisture Sensor

Connect the soil moisture sensor as shown in the following image.

![2](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/05e23f2b-5f3e-4bcf-8b75-c2c5c52b3be3)

Soil moisture sensor pinout:

| SOIL MOISTURE SENSOR | VCC | GND | D0 | A0 |
|----------|----------|----------|----------|----------|
| ESP32  | +3.3 V  | GND  |  -  |  VN  |



### Assemble the ESP32 to LDR

Connect the LDR with an resistor of 10K ohms to ESP32 as shown in the following image.
![3](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/d9941e9d-90a3-469c-944e-2afaeb1a847a)

### Assemble the ESP32 to the Relay Module

Connect the 1 Channel Relay Module 12V to ESP32 as shown in the following image:

![5](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/6e3e6338-5d07-4f7c-81a4-1d46548bff2d)

Relay Module pinout:

| 1 Channel Relay Module 12V | VCC | GND | IN |
|----------|----------|----------|----------|
| ESP32  | +3.3 V  | GND  |  D23  |


### Circuit assembled

The image below shows how the circuit should be assembled for the execution of this project.
![CircuitoFinal](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/87141e78-44a6-46f1-ae9a-5d6cf30f8c17)
