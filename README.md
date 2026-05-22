# Distributed_Data_Acquisition_Network
[insert image] 

## Overview
This project is a distributed data acquisition network that is designed for real-time monitoring of temperature, pressure, and airflow within a biomass gasifier. It is developed to support biomass gasification research through synchronized data collection and experimental analysis. It integrates embedded systems, wireless telemetry, distributed instrumentation, and custom PCB development into a scalable monitoring platform for research.



## System Architecture

The system consists of 10 independent ESP32s-based sensing nodes:
- 8 temperature nodes
- 1 pressure node
- 1 air velocity node
  
Thermal Monitoring Node contained:
- Arduino ESP32 Nano microcontroller
- Thermocouple
- MAX 31855 (Thermocouple Amplifier module)
- Switch
- 5V Battery

Differential Pressure Node contained:
- Arduino ESP32 Nano microcontroller
- Differential Pressure Transducer
- RS-485 module
- Switch
- 12V battery
- DC-DC Buck Converter*

Air Velocity Node
- Arduino ESP32 Nano microcontroller
- Wind Sensor Rev. P
- 12V battery
- DC-DC Buck Converter*
- switch

Each node transmits telemetry data wirelessly via WiFi to a centralized database for storage and analysis.

The distributed architecture enables scalable monitoring throughout the biomass gasifier while reducing wiring complexity and improving node deployment flexibility.
[insert diagram]

*DC-DC Buck Converters are implemented in the circuit to protect the MCU since it can safely input only 5V. Could have used a 5V voltage regulator, but the buck converter uses little power since it converts excess energy into currents and emits a little amount of heat. 5V regulators work fine but with power and heat management in mind, the buck converter is a better choice for me.
  
## Schematic & Wiring
I designed the schematics originally on KiCad and then transferred them to Altium to become comfortable with a design software that is used in the industry.
Below are the images of each node
