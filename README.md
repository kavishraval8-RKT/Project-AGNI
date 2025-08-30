# Project-AGNI
AGNI (Advanced Ground-test Node for Ignition) is a custom PCB for rocket motor test stand. Built on ESP32-S2, it serves as the central node for ignition safety, load cell amplifier, accelerometer, thermocouples, and wireless telemetry—enabling safe ignition, thrust measurement, and real-time data logging.

## Features
- **Ignition Safety** – Dedicated MOSFET-based arming and firing circuit with GPIO-controlled safety.
- **Sensor Integration** – HX711 load cell amplifier, MPU6050 accelerometer, and MAX6675 thermocouple modules.
- **Wireless Telemetry** – HC-12 module for long-range communication.
- **Data Logging** – MicroSD card storage for test data.
- **Servo Control** – For arming and mechanical actuation.
- **ESP32-S2-WROOM** – Central microcontroller with USB/FTDI communication support.

---

## Hardware Overview
- Designed in **EasyEDA**
- Based on **ESP32-S2-WROOM**
- 7V–12V battery input with TLV series LDOs regulation to 5V and 3.3V
- GPIO-based safety switches
- Modular headers for sensors and communication modules
- Remote servo based ignition for extra safety
  
---

## Repository Contents
- Complete circuit schematics exported from EasyEDA
- PCB layout files and Gerber files for fabrication
- Source code for ESP32-S2
– Documentation and images

---

## How It Works
1. Power input (7V–12V) is regulated to 5V and 3.3V using TLV series LDOs.  
2. ESP32-S2 handles ignition logic, sensor data collection, and telemetry.  
3. Safety switch and MOSFET circuit ensure controlled ignition.  
4. Data is transmitted wirelessly (HC-12) and logged to MicroSD for analysis.  

---

## System Sensing
-HX711 (Load Cell Amp): Captures thrust data for precise force measurement.
-MPU6050 (Accel/Gyro): Monitors vibrations and motion during ignition.
-MAX6675 (Thermocouples): Measures motor case, nozzle, and exhaust temperatures.
-Servo (PWM): Controls mechanical arming and actuation.

---

## License
This project is released under the MIT License.
