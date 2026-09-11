# Smart Industrial Environment Monitor (ESP32)

An industrial-grade IoT prototype built on ESP32 to monitor real-time environmental conditions (temperature and humidity) using a DHT11 sensor. This project demonstrates robust firmware architecture, non-blocking execution, network resilience, and structured version control alignment based on international corporate standards.

## 🚀 Key Features
- **Non-blocking Architecture:** Utilizes `millis()` instead of `delay()` to maintain deterministic system loops and high responsiveness.
- **Robust Network State-Machine:** Implemented automatic Wi-Fi and MQTT reconnection routines to prevent system hang ups during network dropouts.
- **Data Isolation & Security:** Custom unique MQTT topic trees deployed to avoid payload collision on public brokers.
- **Clean Code & Documentation:** Codebase is fully documented using the **Doxygen Style** for optimal readability and corporate maintenance.

## 🛠️ Tech Stack & Hardware Components
- **Microcontroller:** ESP32 (Espressif Systems)
- **Sensor:** DHT11 (Temperature & Humidity)
- **Framework:** Arduino Framework via PlatformIO (VS Code)
- **Communication Protocol:** MQTT (Message Queuing Telemetry Transport)
- **MQTT Broker:** `test.mosquitto.org` (Port 1883)
- **Data Format:** Structured JSON payloads (`{"temperature": xx.xx, "humidity": xx.xx}`)

## 📁 Repository Structure
```text
.
├── .gitignore               # Filters build artifacts from version control
├── platformio.ini           # Centralized configuration and strict library locking
├── include/                 # Header files (if applicable)
├── lib/                     # Custom local libraries
└── src/
    └── main.cpp             # Main application source code with Doxygen documentation
```

## 🔌 Hardware Pin Mapping

| Component | ESP32 Pin (GPIO) | Standard Wire Color | Function |
|:---|:---|:---|:---|
| DHT11 VCC | 3.3V / 5V | Red | Power Supply |
| DHT11 GND | GND | Black / Dark Blue | Ground |
| DHT11 DATA | GPIO 4 | Yellow / Green | Data Signal (Single-bus) |

## 📊 Live Telemetry Visualization
The telemetry data is compiled into JSON and streamed every 2 seconds. It can be monitored and visualized in real-time using **MQTT Explorer** or any standard MQTT client under the following isolated topic branch:
`industrial/pabrik/[YOUR_NAME]/sensor1`

## 📈 Git Workflow Standards Deployed
This project strictly follows the **Feature Branching Workflow** used in corporate agile environments:
1. `chore: initial commit` - Base PlatformIO framework setup.
2. `feat: implement non-blocking DHT11 sensor reading` - Core sensor mechanics.
3. `feat: implement robust WiFi connection and MQTT publishing` - Networking interface.
4. `security: customize MQTT publish topic` - Topic isolation and naming optimization.
