Project Overview: 
This project implements a heartbeat monitoring system using a PIC16F877A microcontroller. The 
system reads data from a HW487 heartbeat sensor and displays the calculated beats per minute (BPM) 
on a 1602A LCD display. This setup allows for real-time monitoring of a person's heart rate. 

Hardware Configuration: 
Microcontroller: PIC16F877A 
Display: 1602A LCD (16x2 characters) 
Sensor: HW487 heartbeat sensor 
Clock: 20 MHz crystal oscillator with two 32 pF capacitors 
Contrast Control: Potentiometer for LCD 
Additional Components: 4.7 kΩ resistor (likely for pull-up) 
Programmer: PICkit 3 for hex file upload 

Software Development: 
The project is coded in C language, compiled using the XC8 compiler for PIC microcontrollers. The 
software is structured to manage both the Analog-to-Digital Converter (ADC) for sensor reading and 
the LCD for data display. 
Microcontroller Configuration: 
Oscillator: High-speed external (HS) mode, utilizing the 20 MHz crystal 
Watchdog Timer: Disabled 
Power-up Timer: Disabled 
Brown-out Reset: Enabled 
Low-Voltage Programming: Disabled 
Code Protection: Disabled for both main memory and EEPROM 

LCD Interface: 
Operation Mode: 4-bit interface 
Port Usage: PORTD pins for data (RD4-RD7) and control (RD2, RD3) 
Contrast: Adjustable via potentiometer 
Functions: Implemented for initialization, command sending, character/string display, and cursor 
positioning 

ADC Configuration: 
Resolution: 10-bit, right-justified result 
Input: AN0 (RA0) used for the heartbeat sensor 
Clock: ADC clock set to Fosc/16 
Channel: Configurable, but primarily using channel 0

Conclusion: 
This project demonstrates a functional integration of analog sensing and digital display using a PIC 
microcontroller. It serves as a solid foundation for more advanced health monitoring devices. The use 
of common components and a standard programming interface (PICkit 3) makes this project 
accessible and modifiable. While the current implementation is basic, it provides a clear path for future 
enhancements in both hardware design and software algorithms. 
