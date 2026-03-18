# Active Scan Solar Tracker & Temp Monitor

An embedded hardware project I built using an Arduino UNO to maximize solar panel efficiency. Instead of standard comparative tracking, this system maps the surrounding light intensity using a sweep algorithm to find the absolute brightest spot, while also monitoring ambient temperature.

## Hardware Components
* Arduino UNO (ATmega328P)
* 2x Servo Motors (Pan & Tilt rotation)
* LDR (Light Dependent Resistor / Photodiode)
* Dallas DS18B20 Temperature Sensor
* 1602 LCD Display with I2C module

## Software Implementation & Sweep Algorithm
The code is written in C++ using Object-Oriented principles. I created custom classes (`Photodiode` and `Servomotor`) to encapsulate the hardware logic. 

To find the light source, I implemented a "Sweep & Scan" algorithm:
1. The servos scan the area in 10-degree increments across both axes.
2. At each step, the system reads the light value, storing the highest value and its corresponding X/Y coordinates.
3. Once the scan is complete, the servos automatically drive the panel to the exact coordinates of the maximum light intensity.
4. The system rests, reads the ambient temperature via the Dallas sensor, displays the stats on the LCD, and repeats the cycle.

## Hardware Challenges (The "3 Broken LDRs" Issue)
Initially, I planned to use a 4-sensor closed-loop control system. However, during testing, I discovered a hardware issue where 3 out of my 4 photodiodes were outputting faulty, maxed-out analog readings (1000-1023) constantly, regardless of actual light. 

Instead of abandoning the project, I adapted the software architecture. I mapped the entire tracking logic to rely solely on the one working sensor (Photodiode ID 3 on pin A2) by changing the tracking strategy from "sensor comparison" to "active area scanning".
