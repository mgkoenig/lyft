# LYFT Firmware
The LYFT firmware follows a classical multi-layer approach, consisting of a hardware abstraction layer (HAL) and an application layer. The application functions are separated into several files, grouped by their principal tasks. This concept allows you to port the fimrware easily to other platforms. 

![alt text](https://github.com/mgkoenig/lyft/blob/main/firmware/architecture.png?raw=true)

The HAL layer utilizes the ASF framework (currently version 3.49.1) for Microchip's SAM MCU's with the following components:
- DAC (callback) [for audio volume control]
- EXTINT (callback) [get notified of keypad inputs]
- NVM [saving configuration and desk positions]
- PORT [GPIO pin control]
- SERCOM I2C (callback) [talk to AS1115 LED controller]
- SERCOM USART (polled) [using a UART module for LIN communication]
- SYSTEM (core system drivers)
- TC (callback) [handling motor timings, keypad timings ond more]
- TCC (callback)

The LYFT application is separated into four modules: 
- Audio: all functins related to audio features
- Display: all functions related to the display
- Keypad: all functins to evaluate keystrokes and holding times
- Motor: all functions to control the motors including LIN bus communication

For the AS1115 display driver IC a driver module was written. 
