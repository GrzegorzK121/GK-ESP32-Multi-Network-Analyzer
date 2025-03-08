# GK ESP32 Multi-Network Analyzer

Advanced system for analyzing WiFi, Bluetooth, and GSM networks using ESP32. Enables scanning, analysis, and visualization of results on an OLED display, with interactive menu navigation.

## Project Diagram
See: [Project Diagram Image]

## Simple Documentation
See: [Simple Documentation Image]

## Menu Screenshot
See: [Menu Screenshot Image]

## Example Feature Screenshot - WiFi Network Details
See: [WiFi Network Details Screenshot Image]

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

