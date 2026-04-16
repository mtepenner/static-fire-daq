# 🚀 Static Fire DAQ

An ultra-fast, deterministic Data Acquisition (DAQ) and control system designed for aerospace static fire testing. This project provides a robust, low-latency pipeline from bare-metal sensor reads and critical abort loops up to a high-frequency, web-based mission control dashboard.

## 📑 Table of Contents
- [Features](#-features)
- [Architecture](#-architecture)
- [Technologies Used](#-technologies-used)
- [Installation](#-installation)
- [Usage](#-usage)
- [Contributing](#-contributing)
- [License](#-license)

## 🌟 Features
* **Deterministic Safety Loop:** Bare-metal C++ firmware featuring a critical <1ms redline abort loop that evaluates pressure against limits.
* **High-Speed Sensor Processing:** Utilizes direct register access for high-speed Analog-to-Digital Conversion (ADC) and Kalman filtering to smooth noisy data without adding latency.
* **Actuator Control:** Solenoid control logic to trigger MOSFETs, enabling rapid closure of the Main Oxidizer Valve (MOV).
* **High-Throughput Telemetry Ingestion:** A Go-based gateway that reads raw byte streams at massive baud rates (e.g., 2,000,000 baud). It perfectly frames data packets using Consistent Overhead Byte Stuffing (COBS).
* **Real-Time Mission Control:** A React and TypeScript dashboard featuring WebGL-accelerated charts for 1000Hz rendering, a Valve Status HUD for Ignition/Purge/Main valves, and a prominent Manual Abort override.
* **Persistent Cold Storage:** A dedicated data logger that subscribes to the telemetry gateway and writes every data point to CSV files for comprehensive post-test analysis.

## 🏗️ Architecture
The system is divided into four highly specialized subsystems:
1. **Firmware (C/C++):** The deterministic control and safety loop running bare-metal on a microcontroller (e.g., Teensy 4.1) to prevent thread blocking.
2. **Telemetry Gateway (Go):** The high-throughput ingestion server that bridges the serial port to a WebSocket hub for broadcasting decoded JSON.
3. **Mission Control Dashboard (React):** The frontend UI that visualizes the firehose of WebSocket data and provides critical operator interfaces.
4. **Data Logger (Go/Python):** A lightweight service dedicated purely to writing telemetry to disk.

## 🛠️ Technologies Used
* **Firmware:** C/C++, PlatformIO
* **Telemetry & Logging:** Go (Golang), Serial Communications, COBS
* **Frontend UI:** React, TypeScript, WebGL, WebSockets
* **Deployment & CI/CD:** Docker, Docker Compose, Make, GitHub Actions

## 💻 Installation

### Prerequisites
* Docker and Docker Compose installed.
* PlatformIO installed (for flashing firmware).
* A compatible microcontroller (e.g., Teensy 4.1) connected via USB.

### Setup Steps
1. Clone the repository:
   ```bash
   git clone [https://github.com/yourusername/static-fire-daq.git](https://github.com/yourusername/static-fire-daq.git)
   cd static-fire-daq
   ```
2. Flash the firmware to your microcontroller using the provided Makefile commands:
   ```bash
   make flash-firmware
   ```
3. Boot up the software suite (Go Gateway, React dashboard, and Logger) locally using Docker Compose:
   ```bash
   docker-compose up --build -d
   ```

## 🎮 Usage
Once the hardware is connected and the containers are running:
1. Open your browser and navigate to the Mission Control Dashboard (typically `http://localhost:3000`).
2. Monitor the high-speed WebGL charts for real-time pressure and thrust telemetry.
3. Verify the state of the system via the **Valve Status HUD**.
4. In the event of an anomaly, utilize the **Abort Panel** to manually trigger the safety sequence.
5. After the test, retrieve the generated CSV files from the `data_logger` volume for post-test analysis.

## 🤝 Contributing
Contributions are welcome. Please ensure that modifications to the control loop maintain deterministic performance. Automated CI/CD pipelines are in place via GitHub Actions to compile the C++ firmware and build the Go/React assets.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/NewSensor`)
3. Commit your Changes (`git commit -m 'Add support for new pressure transducer'`)
4. Push to the Branch (`git push origin feature/NewSensor`)
5. Open a Pull Request

## 📄 License
Distributed under the MIT License. See `LICENSE` for more information.
