# 📡 Smart Anti-Theft System with 2FA Authentication

A low-cost, embedded security system designed to protect vehicles or sensitive assets using **2-Factor Authentication (2FA)**. This project uses an **ATmega328P** (Arduino), **SIM800L GSM module**, **Neo6M GPS**, and a **4x4 keypad** to detect unauthorized access and notify users via SMS with GPS location data.

---

## 🛠️ Hardware Components

- ✅ ATmega328P (Arduino UNO/Nano)
- ✅ SIM800L GSM Module
- ✅ Neo6M GPS Module
- ✅ I2C 16x2 LCD Display
- ✅ 4x4 Matrix Keypad
- ✅ Relay Module (for alarm or lock control)
- ✅ Buzzer (for feedback)
- ✅ 3.7V Battery + Boost Converter (3.7V to 5V)
- ✅ Jumper wires, breadboard or custom PCB

---

## 🔐 Features

- 🔢 Password Authentication via Keypad (2FA)
- 📲 SMS Command Reception & Processing
- 🛰️ GPS Location Tracking with Google Maps link
- 🔁 Relay Control via SMS (ON/OFF)
- 💬 Real-time Feedback on LCD
- 📡 Dual SMS Notification (Primary & Secondary numbers)
- 🔔 Buzzer feedback for input confirmation

---

## 🧪 How It Works

1. System boots and prompts for a 4-digit password on the LCD.
2. Upon successful password entry, SIM800L module gets ready to receive SMS.
3. Recognized SMS commands (`ON`, `OFF`, `LOCATION`) control the relay or request GPS location.
4. When `LOCATION` is sent, a Google Maps link of the GPS coordinates is returned via SMS.

---

## 📲 SMS Commands

| Command    | Function                          |
|------------|-----------------------------------|
| `ON`       | Activates relay (e.g. siren/alarm)|
| `OFF`      | Deactivates relay                 |
| `LOCATION` | Sends GPS coordinates via SMS     |

---

## ⚙️ Setup & Installation

### 1. Wiring
Connect all components based on the following:

- **SIM800L TX** → Arduino Pin 10  
- **SIM800L RX** → Arduino Pin 11  
- **GPS TX** → Arduino Pin 8  
- **LCD SDA** → A4  
- **LCD SCL** → A5  
- **Keypad Rows** → Pins 4, 5, 6, 7  
- **Keypad Columns** → Pins A0, A1, A2, A3  
- **Relay** → Pin 2  
- **Buzzer** → Pin 3  

### 2. Upload Code
- Use the Arduino IDE.
- Install the required libraries:
  - `LiquidCrystal_I2C`
  - `SoftwareSerial`
  - `Keypad`
  - `TinyGPS++`

### 3. Power Up
- Use a 3.7V lithium battery + boost converter to power both the microcontroller and modules.
- Ensure GSM and GPS modules are getting stable 5V.
