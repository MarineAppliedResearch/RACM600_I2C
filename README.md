# RACM600_I2C

An Arduino library for interfacing with the **RECOM RACM600-SL** power supply over **I2C/PMBus**. This library simplifies communication with the RACM600, handling **voltage, current, and temperature monitoring**, as well as **fault detection and power control**.

## About This Project
This library was developed to facilitate **I2C communication with the RACM600-SL** power supply, allowing users to **monitor and control power parameters** using standard **PMBus commands**. It provides functions for **reading input/output voltage, current, temperature, and fault status**, as well as enabling/disabling power output.

Marine Applied Research & Exploration (MARE) develops and shares this code to support the exploration and documentation of deep-water ecosystems, contributing to their conservation and management. To sustain our mission and initiatives, please consider donating at https://maregroup.org/donate.

## Installation

To install this library:

1. Download the latest release from the [Releases](https://github.com/MarineAppliedResearch/RACM600_I2C/releases) page.
2. Extract the folder and place it inside your Arduino libraries directory (`Documents/Arduino/libraries/`).
3. Open **Arduino IDE** and include the library in your sketch:

   ```cpp
   #include "RACM600.h"
   ```

## Usage

A basic example of using the RACM600-SL power supply:

```cpp
#include <Wire.h>
#include "RACM600.h"

RACM600 psu;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    psu.begin();

    Serial.println("Checking power supply status...");

    psu.enableOutput();
    delay(500);
    Serial.println("Power Output Enabled");

    Serial.println("Checking faults...");
    psu.readFaults();
}

void loop() {
    // Read and display power parameters
    Serial.print("Voltage: "); Serial.print(psu.readVoltage()); Serial.println(" V");
    Serial.print("Current: "); Serial.print(psu.readCurrent()); Serial.println(" A");
    Serial.print("Temperature: "); Serial.print(psu.readTemperature()); Serial.println(" °C");

    // Check for any new faults
    psu.readFaults();

    delay(5000);
}
```

For more details, check out the [examples](examples/) directory.

## Features
- 📡 **I2C (PMBus) Communication** – Easy integration with the Arduino `Wire` library.
- ⚡ **Voltage & Current Monitoring** – Read real-time power output values.
- 🌡️ **Temperature Monitoring** – Check **ambient**, **PFC**, and **LLC** temperatures.
- 🔥 **Fault Detection** – Read `STATUS_WORD` and **detailed fault registers** for diagnostics.
- 🟢 **Power Control** – Enable or disable power output with a single command.
- 🚀 **Lightweight API** – Simple, efficient command structure for minimal overhead.

## Contributing
Contributions are welcome! If you'd like to contribute:

- Fork the repository.
- Create a new branch for your feature or bug fix.
- Commit your changes with meaningful commit messages.
- Open a pull request with a clear description of your changes.

### Coding Standards
- Use **clear and concise comments** to explain functionality.
- Follow **consistent indentation** (1 tab per indentation level).
- Function and variable names should be **descriptive and use camelCase**.
- When adding new features, ensure they are **well-documented and tested**.

## License
This project is licensed under the **BSD License**. See the [LICENSE](license.txt) file for details.

## Version History

### [1.0.0] - 2025-02-21
- Initial release of the **RACM600_I2C** library.
- Implements **I2C communication with PMBus commands**.
- Supports **reading voltage, current, power, and temperature**.
- Implements **fault detection and automatic detailed fault logging**.
- Provides a **simple API** for controlling power output.
- Compatible with the **Arduino ecosystem**.

## Upcoming Features
Planned improvements and future updates:
- 🔧 Add support for **writing voltage/current limits**.
- ⚙️ Implement **automatic power recovery logic** after a fault.
- 📘 Expand documentation with **more usage examples**.

## Need Help?
If you have any questions, feel free to **open an issue** or reach out!

