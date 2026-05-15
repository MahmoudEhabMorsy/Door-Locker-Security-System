# Door Locker Security System

> **Educational Project Notice**
> This project was developed as an educational embedded systems application using AVR microcontrollers and Embedded C programming. The system demonstrates secure password handling, UART communication between ECUs, EEPROM storage, motor control, buzzer alarms, keypad interfacing, and LCD visualization in a real-time embedded environment.

## Overview

The Door Locker Security System is a smart embedded security application designed using a dual-microcontroller architecture.

The system consists of:

* Human Machine Interface ECU (HMI ECU)
* Control ECU

The project simulates a secure electronic door locking system where users can:

* Create a password
* Unlock the door securely
* Change the password
* Trigger security alarms after invalid attempts

The system demonstrates several advanced embedded systems concepts including:

* UART communication
* EEPROM data storage
* Password authentication
* Motor control
* LCD and keypad interfacing
* Timer-based operations
* Embedded security logic
* Modular firmware architecture

The implementation was developed in Embedded C using low-level AVR programming.

---

# Features

* Password-protected door access
* Dual-ECU architecture
* UART communication between microcontrollers
* Password storage in EEPROM
* LCD user interface
* Keypad password entry
* DC motor door control
* Buzzer alarm system
* Timer-based motor operation
* Security lockout after failed attempts
* Modular embedded driver architecture

---

# Technologies Used

* Embedded C
* AVR Microcontroller Programming
* UART Communication
* EEPROM Interfacing
* LCD Interfacing
* Keypad Interfacing
* DC Motor Control
* Timer Modules
* GPIO Programming
* Interrupt Handling
* Eclipse CDT
* AVR-GCC

---

# Hardware Components

The project was designed using:

* Two AVR Microcontrollers
* LCD Display
* Keypad Module
* DC Motor
* Buzzer
* External EEPROM
* UART Communication Lines
* Breadboard and Embedded Circuit Components
* Power Supply

---

# System Architecture

The system follows a distributed embedded architecture using two ECUs.

| ECU         | Responsibility                                  |
| ----------- | ----------------------------------------------- |
| HMI ECU     | Handles user interaction through LCD and keypad |
| Control ECU | Processes passwords and controls hardware       |

Communication between both ECUs is performed using:

```text
UART Serial Communication
```

---

# Project Structure

```text
Door-Locker-Security-System-main/
│
├── Door Locker Security System/
│
├── HMI_ECU/
│   ├── main.c
│   ├── lcd.c
│   ├── keypad.c
│   ├── uart.c
│   ├── timer.c
│   ├── gpio.c
│   └── drivers...
│
├── Control_ECU/
│   ├── main.c
│   ├── uart.c
│   ├── eeprom.c
│   ├── dc_motor.c
│   ├── buzzer.c
│   ├── timer.c
│   ├── gpio.c
│   └── drivers...
```

---

# System Workflow

The system operates using the following sequence:

```text
User Enters Password
          ↓
HMI ECU Sends Password via UART
          ↓
Control ECU Verifies Password
          ↓
If Correct → Open Door
If Incorrect → Retry Counter Increments
          ↓
Three Failed Attempts → Activate Buzzer Alarm
```

---

# Password Management

The system supports:

* Initial password creation
* Password confirmation
* Password verification
* Password change functionality

Passwords are securely stored inside:

```text
External EEPROM
```

This allows password persistence after power loss.

---

# UART Communication

The two ECUs communicate using UART serial communication.

### UART Responsibilities

| Function               | Description                    |
| ---------------------- | ------------------------------ |
| Password Transfer      | Sends entered passwords        |
| Command Exchange       | Synchronizes system states     |
| Authentication Signals | Indicates valid/invalid access |

Example functionality:

```c
UART_sendByte()
UART_receiveByte()
```

---

# LCD User Interface

The LCD provides a simple user interface for:

* Password entry prompts
* System status messages
* Door state notifications
* Error messages

Example display:

```text
Enter Password:
```

The LCD driver supports:

* Character display
* Cursor positioning
* Dynamic screen updates

---

# Keypad Interfacing

The keypad is used for:

* Password input
* User interaction
* Command selection

Example functionality:

```c
KEYPAD_getPressedKey()
```

---

# Door Motor Control

The Control ECU operates a DC motor responsible for:

* Door opening
* Door locking
* Timed door movement

The motor operation sequence:

```text
Rotate Clockwise → Door Opens
Pause
Rotate Anti-Clockwise → Door Closes
```

Example functions:

```c
DcMotor_Rotate()
```

---

# Security Alarm System

The system includes a buzzer alarm mechanism.

### Alarm Activation

The buzzer activates when:

```text
Three consecutive incorrect password attempts occur
```

This simulates a basic intrusion detection mechanism.

Example functionality:

```c
Buzzer_on()
Buzzer_off()
```

---

# EEPROM Storage

The project uses external EEPROM for persistent password storage.

### EEPROM Features

| Feature             | Purpose                             |
| ------------------- | ----------------------------------- |
| Non-Volatile Memory | Preserves password after power loss |
| Password Storage    | Stores user credentials securely    |

Example functionality:

```c
EEPROM_writeByte()
EEPROM_readByte()
```

---

# Timer Usage

Timers are used for:

* Door opening duration
* Door closing duration
* Alarm timing
* Delay generation

This demonstrates:

* Real-time embedded behavior
* Timer-based task scheduling
* Embedded timing control

---

# Core Embedded Concepts Demonstrated

This project demonstrates several important embedded systems concepts:

* Multi-ECU communication
* UART serial communication
* EEPROM interfacing
* Embedded security systems
* Timer-based operations
* LCD and keypad interfacing
* DC motor control
* Real-time embedded systems
* Modular firmware architecture
* Hardware/software integration

---

# Program Workflow Example

## Successful Authentication

```text
Enter Password
      ↓
Password Correct
      ↓
Door Unlocking...
      ↓
Door Locking...
```

## Failed Authentication

```text
Wrong Password
Attempts Remaining: 2
```

After multiple failures:

```text
Security Alarm Activated
```

---

# Development Environment

The project was developed using:

* Eclipse CDT
* AVR-GCC Compiler
* Embedded C

Included libraries:

```c
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
```

---

# How to Build and Run

## 1. Clone the Repository

```bash
git clone https://github.com/MahmoudEhabMorsy/Door-Locker-Security-System.git
```

## 2. Open the Project

Import both ECUs into:

* Eclipse CDT
* Atmel Studio
* Microchip Studio

## 3. Compile the Projects

Compile:

* HMI ECU firmware
* Control ECU firmware

Using AVR-GCC.

## 4. Flash Each MCU

Upload firmware to both AVR microcontrollers using:

* USBasp
* AVRDUDE
* Atmel Programmer

## 5. Connect Hardware

Ensure proper connection of:

* LCD display
* Keypad
* EEPROM
* DC motor
* Buzzer
* UART communication lines

---

# Educational Value

This project provides practical experience with:

* Embedded firmware engineering
* Secure embedded system design
* Multi-microcontroller communication
* UART-based networking
* EEPROM memory interfacing
* Real-time embedded control systems
* Driver-based firmware architecture

It serves as an excellent practical introduction to:

* Embedded security systems
* Automotive/body-control style architectures
* Smart embedded applications

---

# Possible Future Improvements

Potential enhancements include:

* RFID authentication
* Fingerprint sensor integration
* Bluetooth/Wi-Fi connectivity
* Mobile application support
* OLED/TFT graphical display
* Encryption support
* RTOS integration
* Event logging system
* Cloud connectivity
* Remote door control
* Advanced intrusion detection

---

# Known Limitations

* Educational prototype implementation
* Basic password protection logic
* No encryption support
* Limited UI functionality
* No remote connectivity

---

# Software Design Highlights

The project emphasizes:

* Modular embedded driver architecture
* Multi-ECU embedded design
* Real-time embedded behavior
* Secure password handling concepts
* Reusable firmware modules
* Hardware abstraction techniques

---

# Author

Developed by:

* Mahmoud Ehab

---

# License

This project is provided for educational and learning purposes.

You are free to study, modify, and extend the implementation for academic or personal use.

---

# Acknowledgments

Special thanks to:

* AVR embedded systems documentation
* Embedded systems learning communities
* Open-source firmware development resources
* Educational embedded systems platforms
