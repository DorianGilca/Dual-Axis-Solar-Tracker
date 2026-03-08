# Arduino Solar Tracker

This is an embedded hardware project that automatically directs a solar panel towards the brightest light source in its environment. The system maximizes energy absorption by continuously adjusting its angle based on real-time light intensity readings.

## Hardware Components
* Arduino UNO (ATmega328P)
* Servo Motors (for panel rotation)
* LDRs (Light Dependent Resistors / Photoresistors)
* 10k Ohm Resistors
* Breadboard and Jumper Wires

## Hardware Architecture
The project uses a closed-loop control system. The LDRs act as light sensors and are connected to the analog input pins of the Arduino via voltage divider circuits. The Arduino continuously compares the analog voltage readings from opposite sensors.

If one sensor receives more light than the other, the microcontroller calculates the difference and sends a PWM (Pulse Width Modulation) signal to the servo motors. The servos adjust the panel's physical position until the light hitting all sensors is balanced.

## Software Implementation
The code is written in C++ using the Arduino IDE and relies on the built-in `Servo.h` library for motor control. 
The logic reads the analog values, calculates the error margin, and applies a basic proportional control loop to move the servos. A small deadband (tolerance threshold) is included in the algorithm to prevent the servos from constantly jittering when the difference in light intensity is negligible.

## How to run
1. Assemble the circuit according to the provided schematic diagram.
2. Connect the Arduino UNO to your computer.
3. Open the `.ino` file in the Arduino IDE.
4. Compile and upload the code to the microcontroller.
5. Use a flashlight near the LDRs to test the tracking response and verify motor direction.
