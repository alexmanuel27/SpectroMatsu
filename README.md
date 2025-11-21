# SpectroMatsu

A portable, open-source optical spectrophotometer designed for continuous water quality monitoring without the need for chemical reagents. Built around the Hamamatsu C12666MA miniature spectrometer sensor.

[//]: # (Pendiente: Insertar imagen del PCB aquí, una captura del esquemático o una foto del PCB real si está disponible)
<!-- ![PCB Image Placeholder](path/to/your/pcb_image.jpg) -->

## Table of Contents

- [Features](#features)
- [Hardware](#hardware)
  - [Sensor](#sensor)
  - [Microcontroller & Display](#microcontroller--display)
  - [Hardware Design (PCB)](#hardware-design-pcb)
- [Software](#software)
- [Installation](#installation)
- [Usage](#usage)
- [Screenshots](#screenshots)
- [License](#license)

## Features

*   **Portable & Low-Cost:** Designed for field use with a focus on affordability.
*   **Open Source:** Hardware schematics and software are freely available.
*   **Reagent-Free Monitoring:** Measures water quality parameters optically.
*   **Real-Time Data:** Captures and displays absorbance spectra.
*   **Local Data Storage:** Saves measurement samples in RAM (with plans for external EEPROM integration).
*   **WiFi Connectivity:** Enables a local web interface for data access and control.
*   **Intuitive Touch Interface:** Easy-to-use graphical menu system on a 3.5" color display.
*   **Sample Management:** Functions to take, save, list, and download sample data (individually or in bulk) via the local web interface.

## Hardware

### Sensor

*   **Model:** Hamamatsu C12666MA
*   **Type:** Miniature linear image sensor for spectral analysis.
*   **Spectral Range:** Optimized for visible light, suitable for detecting various water quality indicators through absorbance measurements.
*   **Integration:** Directly interfaces with the custom-designed PCB for signal conditioning and data acquisition.

### Microcontroller & Display

*   **Microcontroller:** ESP32-S3 (specifically, the LilyGo T-Display-S3 module).
*   **Features:** WiFi capability, sufficient processing power for basic spectroscopic calculations.
*   **Display:** 3.5" 320x170 pixel color TFT LCD with resistive touch panel.
*   **Purpose:** Provides the user interface for control, data visualization (spectra), and menu navigation.

### Hardware Design (PCB)

*   **Status:** Custom-designed PCB specifically for integrating the Hamamatsu C12666MA sensor with the LilyGo T-Display-S3.
*   **Functionality:**
    *   Houses the Hamamatsu C12666MA sensor.
    *   Includes necessary components for sensor biasing, timing, and analog signal conditioning (e.g., amplification, filtering).
    *   Interfaces with the LilyGo T-Display-S3's GPIO pins for data acquisition and control.
    *   Provides stable mechanical mounting for the sensor relative to the optical path.
*   **Details:** [//]: # (PENDIENTE: Completar con detalles específicos del diseño del PCB: esquemáticos, componentes clave, conexiones precisas, archivos gerber si se publican, etc.)

## Software

*   **Platform:** Arduino IDE / PlatformIO.
*   **Language:** C++
*   **Core Functionality:**
    *   Controls the Hamamatsu C12666MA sensor (timing, data reading).
    *   Processes raw sensor data to generate absorbance spectra.
    *   Provides a graphical user interface (GUI) on the TFT display using a state machine architecture.
    *   Handles touch input for navigation and control.
    *   Stores measurement data (samples) in the ESP32's RAM.
    *   Manages WiFi access point (AP) mode.
    *   Runs a local web server providing a web interface for sample listing and downloading (CSV format).
    *   Implements a simple menu system accessible via the touchscreen.

### Key Functionalities

1.  **Sample Taking:**
    *   Navigate to the main screen.
    *   Press the "SAMPLE" button.
    *   The system enters a 3-second acquisition phase (simulated in the current state).
    *   After acquisition, the resulting spectrum is displayed.
    *   Press the "SAVE" button to store the spectrum in RAM with a generated name.

2.  **Sample Listing:**
    *   Access the main menu (hamburger icon).
    *   Select "SAMPLES".
    *   A list of saved sample names and dates (default date shown) is displayed on the screen.

3.  **Local Web Interface:**
    *   Activate the WiFi AP by pressing the WiFi button on the main screen.
    *   Connect your mobile device or computer to the WiFi network named `SpectroMatsu`.
    *   Open a web browser and navigate to the AP's IP address (typically `192.168.4.1`).
    *   View the list of saved samples.
    *   Download individual sample data as CSV files.
    *   Download all stored samples as a single CSV file.
    *   Turn off the WiFi AP from the web interface.

4.  **Settings & Info:**
    *   Access system information (firmware version, device model, WiFi status, sample count) via the "SETTINGS" menu.
    *   Use the "ABOUT" menu to view a QR code linking to this repository.

## Installation

1.  Clone or download this repository.
2.  Install [PlatformIO](https://platformio.org/) in your preferred IDE (VSCode recommended).
3.  Open the project folder in PlatformIO.
4.  Ensure your LilyGo T-Display-S3 is connected to your computer via USB.
5.  Build and upload the firmware using PlatformIO's upload command (`Ctrl+Alt+U` in VSCode).

## Usage

1.  Power on the device.
2.  Use the touch screen to navigate menus and take samples.
3.  Activate WiFi via the main screen button to access the web interface.
4.  Connect to the `SpectroMatsu` WiFi network and use a web browser to manage samples.

## Screenshots

[//]: # (Pendiente: Insertar capturas de pantalla aquí)
<!-- Example of how to embed an image: -->
<!-- ![Screenshot 1 - Main Screen](path/to/your/screenshot_main.jpg) -->
<!-- ![Screenshot 2 - Graph Screen](path/to/your/screenshot_graph.jpg) -->
<!-- ![Screenshot 3 - Samples List](path/to/your/screenshot_samples.jpg) -->
<!-- ![Screenshot 4 - Settings Screen](path/to/your/screenshot_settings.jpg) -->
<!-- ![Screenshot 5 - Web Interface](path/to/your/screenshot_web.png) -->

## License

This project is licensed under the [MIT License](LICENSE).

