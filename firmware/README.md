# LYFT Firmware
The LYFT firmware follows a classical multi-layer approach, consisting of a hardware abstraction layer (HAL) and an application layer. This partitioning allows you to port the fimrware easily to other platforms. 

The HAL layer utilizes the ASF framework (version 3.49.1) for Microchip's SAM MCU's with the following components:
- DAC (callback)
- EXTINT (callback)
- NVM
- PORT
- SERCOM I2C (callback)
- SERCOM USART (polled)
- SYSTEM (core system drivers)
- TC (callback)
- TCC (callback)

The LYFT application is separated into four modules: 
- Audio: all functins related to audio features
- Display: all functions related to the display
- Keypad: all functins to evaluate keystrokes
- Motor: all functions to control the motors including LIN bus communication

For the AS1115 display driver IC a driver module was written. 
