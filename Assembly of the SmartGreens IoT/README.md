# STEP 1: Connect the sensors and relay to the ESP32.

### Materials Needed:
- ESP32
- Single-board computer (SBC)
- Light Dependent Resistor (LDR)
- Resistor 10K ohms
- Relay Module
- Soil Moisture Sensor
- 1 Channel Relay Module 12V

 The image below shows the pinout of an ESP32.

![ESP32-Pinout](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/0ef3eec4-5258-40cb-9af9-7b21e8751342)


### Assemble the ESP32 to Soil Moisture Sensor

Connect the soil moisture sensor as shown in the following image.

![2](https://github.com/RicardoBozollan/SmartGreens_IoT/assets/163909522/05e23f2b-5f3e-4bcf-8b75-c2c5c52b3be3)

soil moisture sensor pinout:

|  soil moisture sensor  | VCC  | GND |  D0  |  A0  |
| ESP32  | +3.3 V  | GND  |  -  |  VN  |
