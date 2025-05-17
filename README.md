# 🏠 IAR Smart Home System

An embedded smart home project developed for the TM4C123GH6PM microcontroller using the IAR Embedded Workbench IDE. This system modularizes essential smart home functionalities such as relay control, alarm triggering, temperature sensing, door status monitoring, and UART/Bluetooth communication.

---

## 🚀 Features

### ✅ Relay Control (Active Low)
- Control multiple relays connected to devices like lamps or plugs.
- Active-low logic for safe relay activation.
- Smart logic to avoid disabling active relays.
- Centralized module: `Relay/Relay.c`

### 🌡️ Temperature Monitoring
- Reads temperature using the **LM35DZ analog sensor**.
- Converts ADC readings to Celsius with oversampling.
- Supports filtering to eliminate noise.
- Module path: `Temperature/LM35.c`

### 🚪 Door Status Detection
- Monitors the state (open/closed) of a magnetic door sensor.
- Uses GPIO interrupt on door status pin for accurate detection.
- Logs door open/close timing via Bluetooth.
- Module path: `DoorStatus/DoorStatus.c`

### 🚨 Alarm System
- Activates a buzzer when the temperature exceeds a threshold (e.g., 36°C).
- Buzzer beeps in a toggling pattern using GPIO signals.
- Triggered automatically based on sensor readings.
- Module path: `Alarm/Alarm.c`

### 🔁 System Timing (SysTick)
- Uses SysTick for time-based events and timeouts.
- Supports basic software delay and tick timeout checking.
- Module path: `Systick/Systick.c`

### 📡 UART Communication
- UART-based communication via **HC-05 Bluetooth module**.
- UART interrupt-based receiving for real-time command handling.
- Supports command recognition for relay and alarm control.
- Module path: `UART/UART.c`

---

## 📁 Project Structure
IAR Smart Home/
├── Modules/
│ ├── Alarm/ # Alarm logic
│ ├── DIO/ # Digital I/O abstraction
│ ├── DoorStatus/ # Door sensor handling
│ ├── Relay/ # Relay control logic
│ ├── Systick/ # Timer and delay functions
│ ├── Temperature/ # LM35 temperature sensor interface
│ └── UART/ # Serial communication over UART
├── settings/ # Bitwise macros, startup, and headers
├── inc/ # Includes (if used)
├── Debug/ # IAR-generated debug files
├── main.c # Application entry point
├── tm4c123gh6pm.h # MCU register definitions
├── Smart Home.ewp # IAR project file


---

## ⚙️ System Behavior Overview

- **Relay Switching:** Accepts `'L'`, `'l'`, `'P'`, `'p'` to turn lamp and plug ON/OFF.
- **Temperature Alert:** Reads and processes temperature using ADC + averaging.
- **Door Interrupts:** Fires interrupt on any door open/close event.
- **Bluetooth Commands:** Receives commands from App Inventor app and reacts accordingly.
- **Safety Logic:** Prevents disabling relays that are currently ON (active low).

---

## 🛠️ Getting Started

### Requirements
- TM4C123GH6PM LaunchPad
- IAR Embedded Workbench for ARM
- HC-05 Bluetooth Module
- LM35DZ Temperature Sensor
- Relay module (active low)
- Magnetic door sensor
- Buzzer (active or passive) for alarm signaling

### Steps to Build & Run
1. Open `Smart Home.eww` in IAR Embedded Workbench.
2. Connect your TM4C123 board.
3. Build and download the project to the device.
4. Use a mobile app (e.g., App Inventor-based) to communicate with the system over Bluetooth.

---

## 📲 Bluetooth Command Table

| Command | Action                |
|---------|------------------------|
| `'L'`   | Turn ON Lamp Relay     |
| `'l'`   | Turn OFF Lamp Relay    |
| `'P'`   | Turn ON Plug Relay     |
| `'p'`   | Turn OFF Plug Relay    |
| `'T'`   | Send Temperature        |
| `'D'`   | Send Door Status        |
| `'A'`   | Trigger Alarm (custom)  |

---

## 📌 Notes
- Make sure relay pins are not floating; use pull-ups or pull-downs as needed.
- All relay logic assumes **active low configuration**.
- App Inventor app can monitor real-time status via UART/Bluetooth.

---

## 📃 License

This project is for academic and educational use. Adapt it freely with proper attribution.


