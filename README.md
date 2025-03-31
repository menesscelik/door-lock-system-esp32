# ESP32-S3 Keypad Controlled Servo Door Lock System

## 📑 Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Circuit Design](#circuit-design)
- [How to Set Up](#how-to-set-up)
- [Code Explanation](#code-explanation)
- [Usage](#usage)

---

## 🧠 Introduction

This project is a password-protected door lock system built using an **ESP32-S3 microcontroller**, a **4x4 matrix keypad**, and a **servo motor**. It allows the user to enter a numeric password to unlock the door via the servo motor. The system gives visual feedback through the servo's motion.

## ✨ Features

- Password-based locking mechanism  
- Visual feedback using servo motor positions  
- Supports password confirmation and reset  
- Compact and simple hardware setup  
- Default password: `1907`  
  - Correct: Servo rotates to 180° → waits 5 seconds → returns to 10°  
  - Incorrect: Servo briefly moves to 30° → returns to 10°

## 🔧 Hardware Requirements

- ESP32-S3 Development Board  
- 4x4 Matrix Keypad  
- SG90 or compatible Servo Motor  
- Jumper Wires  
- Breadboard (optional)

## 💻 Software Requirements

- ESP-IDF (for firmware development)  
- VS Code or other ESP-IDF compatible editor  
- USB Cable for programming  
- Serial Monitor for debugging (e.g., minicom, PuTTY)

## 🔌 Circuit Design

- Connect the 4x4 keypad to GPIOs on the ESP32-S3 as defined in your code  
- Connect the servo signal wire to a PWM-capable GPIO pin (e.g., GPIO18)  
- Power the servo via 5V and GND  
- Use common GND between ESP32 and servo for stable operation

## ⚙️ How to Set Up

1. Connect all hardware components according to the circuit design  
2. Clone or download this project to your ESP-IDF workspace  
3. Configure the project using `idf.py menuconfig` if needed  
4. Compile and flash using:  
   ```bash
   idf.py build
   idf.py -p [YOUR_PORT] flash monitor
