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
- 7V–12V battery input with LM7805-based regulation to 5V
- GPIO-based safety switches
- Modular headers for sensors and communication modules

---

## Repository Contents
- `schematics/` – Complete circuit schematics exported from EasyEDA
- `pcb/` – PCB layout files and Gerber files for fabrication
- `firmware/` – Source code for ESP32-S2
- `docs/` – Documentation and images

---

## How It Works
1. Power input (7V–12V) is regulated to 5V using LM7805.  
2. ESP32-S2 handles ignition logic, sensor data collection, and telemetry.  
3. Safety switch and MOSFET circuit ensure controlled ignition.  
4. Data is transmitted wirelessly (HC-12) and logged to MicroSD for analysis.  

---

## Schematic Snapshot
![AGNI Schematic](docs/AGNI_schematic.png)

---

## License
This project is released under the MIT License.
