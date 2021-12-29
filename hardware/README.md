# LYFT Hardware 
The LYFT hardware consists of two PCB boards. A controller board and an HMI board. 

### Controller Board
The controller board holds the SAM-D21 micrcontroller and handles all the motor control as well as the HMI communication. Desk motors are addressed using the LIN bus while the HMI board is connected via I2C. A buzzer is used to generate sounds and alarms. 

### HMI Board
The HMI board is equipped with an AS1115 display driver to control both the 7-segment display and all six buttons. 

### Library
Parts which are not existent in the standard KiCad library are published here. 
