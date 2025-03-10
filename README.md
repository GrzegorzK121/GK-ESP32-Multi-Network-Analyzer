# GK ESP32 Multi-Network Analyzer

# ESP32 Mobile Communication Terminal (GSM SIM800L, WiFi, Bluetooth, OLED GUI)

This project was developed as part of R&D (Research and Development) activities focused on Embedded Systems, utilizing the **ESP32** microcontroller, programmed in **C** within the **Espressif IDE** environment. It features an intuitive graphical user interface displayed on an **OLED** (**I²C** communication), navigated through physical buttons.

The terminal supports:
- **GSM communication** (voice calls, SMS) via the **SIM800L** module (**2G GSM**) using AT commands over **UART**.
- Comprehensive WiFi network scanning, providing detailed information including SSID, RSSI, communication channel, and security type.
- Bluetooth device discovery and detailed analysis (device name, MAC address, device type, available services, RSSI signal strength).
- Intuitive, dynamic OLED-based menu controlled through physical buttons.

The solution is designed for clarity, usability, flexibility, and scalability. It can be employed as a mobile network diagnostic terminal, wireless network monitoring tool, or as a foundation for further R&D efforts in Embedded Systems and wireless communication technologies.

## Features
- Microcontroller: **ESP32**
- Programming Language: **C**
- Development Environment: **Espressif IDE**
- GSM Module: **SIM800L** (**2G GSM**), communication via AT commands over **UART**
- OLED Display: 128x64 pixels, interface **I²C**
- Wireless communication: WiFi and Bluetooth scanning
- Dynamic, user-friendly menu controlled via physical buttons

### Hardware Requirements
- ESP32 microcontroller
- SIM800L GSM Module
- 128x64 OLED display (**I²C** interface)
- Physical control buttons (navigation)


Advanced system for analyzing WiFi, Bluetooth, and GSM networks using ESP32. Enables scanning, analysis, and visualization of results on an OLED display, with interactive menu navigation.

## Project Diagram
![obraz](https://github.com/user-attachments/assets/6a1e1604-939d-4b4d-9028-4809d771e0d7)

## Simple Documentation
[GK ESP32 Multi-Network Analyzer.docx](https://github.com/user-attachments/files/19145320/GK.ESP32.Multi-Network.Analyzer.docx)


## Menu Screenshot
![obraz](https://github.com/user-attachments/assets/7ba922b0-515c-455d-9a58-6c1372cd2fcf)


## Example Feature Screenshot - WiFi Network Details
![obraz](https://github.com/user-attachments/assets/dc4ec182-7424-4fa1-888b-b41c57cbeb4f)
![obraz](https://github.com/user-attachments/assets/b243ca05-ca48-4dc9-9f2c-0c8a21b75894)


Additional images and descriptions available in the documentation:
GK ESP32 Multi-Network Analyzer.docx

## Functionalities
- **WiFi Network Scanning** – Identifies available networks, signal strength, and security. Networks are displayed on an OLED menu, allowing users to select and view details such as signal strength, security type, and channel.
- **GSM Analysis (SIM800L)** – Communicates with SIM800L module, retrieving information on available BTS stations, signal strength, and operator. Users can browse cellular networks and verify connection parameters.
- **Bluetooth Scanner** – Scans BLE and BR/EDR devices, allowing users to view MAC addresses, device names, and available services.
- **OLED Menu Interface** – Interactive menu provides easy switching between system functionalities, clearly displaying results with automatic scrolling for longer names.

## Hardware Used
- **ESP32** – Main controller managing modules.
- **SIM800L** – GSM module for cellular network analysis.
- **OLED 128x64** – Display for presenting scan results and navigating menus.

## Protocols Used
- **WiFi (802.11 b/g/n)** – Network scanning.
- **Bluetooth (BLE, BR/EDR)** – Bluetooth device detection.
- **GSM/AT Commands** – Communication and network analysis via SIM800L.
- **I2C** – Communication with OLED display.
- **UART** – SIM800L control and data retrieval.

## Operation Description
1. **User Interface** – System launches startup screen, then main menu.
2. **Network Scanning** – User selects mode (WiFi, Bluetooth, GSM). System queries modules, collects data, and displays results.
3. **Reviewing Results** – After scanning, users can review details of networks/devices.
4. **Additional Options** – GSM connection testing, operator browsing, signal strength checking.

## How to Run?
### 1. Clone Repository
```
git clone https://github.com/GrzegorzK121/GK-ESP32-Multi-Network-Analyzer.git
cd GK-ESP32-Multi-Network-Analyzer
```
### 2. Clean Previous Builds
```
idf.py clean
```
### 3. Configure and Build
```
idf.py build
```
### 4. Flash to ESP32
```
idf.py flash monitor
```
### 5. Use OLED Menu
After startup, use OLED display to browse networks, analyze signals, and navigate menus.

## License
This project is licensed under the MIT License.

## Contribution
Feel free to fork and submit pull requests for further development.

