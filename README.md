

### 1. GitHub Repository Title
*PIC16F877A-Based Temperature Monitoring System with I2C, SPI & UART | Proteus Simulation*

### 2. GitHub Short Description (for About section)
> Real-time temperature monitoring system using PIC16F877A and LM35 sensor with I2C, SPI and UART communication protocols simulated in Proteus 8 Professional. Shows live bus arbitration and sensor data logging on Virtual Terminal.

### 3. README.md File

Copy this full code and create a file named `README.md` in your repo:
# 🌡️ PIC16F877A Temperature Monitoring System - I2C, SPI & UART Communication

This project demonstrates how the **PIC16F877A microcontroller** manages multi-protocol communication using **I2C, SPI, and UART** to acquire and transmit temperature data from an **LM35 sensor**, simulated in **Proteus 8 Professional**.

This is Project 5 - Schematic Capture Simulation showing real-time bus activity and sensor interfacing.


<img width="1600" height="999" alt="image" src="https://github.com/user-attachments/assets/be708d57-5096-4f33-93bf-8ffd109c8867" />

## 📌 Features

- **Microcontroller:** PIC16F877A (40-pin)
- **Temperature Sensor:** LM35 Analog Sensor (10mV/°C)
- **Communication Protocols Demonstrated:**
    - **I2C:** SDA, SCL lines with TRIG monitoring
    - **SPI:** DIN, DOUT, SCK, SS, TRIG lines
    - **UART:** RXD, TXD, RTS, CTS - Data logged on Virtual Terminal
- **Real-time Debugging:** I2C Debugger & SPI Debugger for bus analysis
- **Output:** Temperature value displayed on Virtual Terminal (e.g., 26.0°C, 31.5°C)

## 🔧 Circuit Diagram / Schematic

The schematic is designed in Proteus 8:

- **U2 - LM35:** VOUT connected to AN0 (RA0/AN0 - Pin 2) of PIC16F877A for ADC conversion.
- **U1 - PIC16F877A:**
    - RC3/SCK/SCL (Pin 18) -> SCL to I2C Debugger
    - RC4/SDI/SDA (Pin 23) -> SDA to I2C Debugger
    - RC5/SDO (Pin 24), RC3, etc. connected to SPI Debugger
    - RC6/TX/CK (Pin 25) and RC7/RX/DT (Pin 26) -> UART Virtual Terminal
- **Debuggers:** Used to visualize Bus Arbitration and Clock Stretching

## ⚙️ How It Works

1.  **Sensing:** LM35 senses ambient temperature and outputs an analog voltage.
2.  **ADC Conversion:** PIC16F877A reads the analog voltage on AN0 and converts it to digital using its 10-bit internal ADC.
3.  **Processing:** The formula `Temperature = (ADC_Value * 5.0 / 1024.0) * 100.0` is used to calculate temperature.
4.  **Communication Management:**
        - **Bus Arbitration:** When multiple masters try to access the I2C bus, the controller checks SDA line state. If a HIGH transmitted is sensed as LOW, it withdraws to prevent data collision (like a walkie-talkie protocol).
        - **Clock Stretching:** The slow slave device (sensor) holds the SCL line LOW to pause the fast master, preventing data loss (like a handshake delay).
5.  **Transmission:** Final temperature data is sent via UART and displayed on the Virtual Terminal.

## 🛠️ Software & Hardware Requirements

- **Simulation Tool:** Proteus 8 Professional - Schematic Capture
- **Compiler:** MPLAB X IDE with XC8 Compiler / MikroC for PIC
- **Components:** PIC16F877A, LM35, VIRTUAL TERMINAL, I2C DEBUGGER, SPI DEBUGGER

## 🚀 How to Run the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/PIC16F877A-Temp-Monitoring.git
2. Open `Project5.pdsprj` in Proteus 8.
3. Load the hex file (`.hex`) into PIC16F877A: Right-click on U1 -> Edit Properties -> Program File.
4. Click Run (Play button) to start simulation.
5. Observe temperature readings on Virtual Terminal and bus activity on I2C/SPI Debugger windows.

## 📁 Repository Structure
/Proteus-Project/
├── Project5.pdsprj       # Proteus Project File
├── Project5.pdsprj        # Schematic File
├── main.c                # Source Code (XC8 / MikroC)
├── main.hex              # Compiled Hex file
├── README.md
└── Screenshot.png        # Your simulation screenshot
## 📊 Sample Output
SS Inactive / Active logs in SPI Debugger
26.0°C displayed on LM35 Probe
31.5°C -> Displayed on Virtual Terminal
## 👨‍💻 Author

*Madhan Kumar*
- Embedded Systems Enthusiast
- Project Focus: C1616 / FA77A Bus Communication Systems

## 📄 License

This project is open-source under the MIT License. Feel free to use and modify.


