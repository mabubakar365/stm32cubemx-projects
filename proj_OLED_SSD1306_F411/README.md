# proj_OLED_SSD1306_F411

## Overview

`proj_OLED_SSD1306_F411` is a firmware project for STM32 Nucleo-F411RE designed to interface with an SSD1306 OLED display. The project demonstrates scanning I2C slave devices and running various tests on the OLED display, including text and graphics rendering. Debugging and test output is provided over UART2.

## Hardware

* **Microcontroller Board:** Nucleo-F411RE
* **Display:** SSD1306 OLED
* **Communication Interfaces:**

  * I2C1 for OLED communication
  * UART2 for debugging output

## Software

* **IDE:** STM32CubeIDE
* **Libraries:** STM32 HAL for I2C, UART, GPIO

## Features

* Scan and detect I2C slave devices
* Display text, shapes, and basic graphics on SSD1306 OLED
* UART debugging to monitor test results
* Modular code structure for easy addition of new tests

## Wiring / Pin Configuration

| Peripheral | Nucleo-F411RE Pin | OLED Pin / Notes          |
| ---------- | ----------------- | ------------------------- |
| I2C1 SCL   | PB6               | SCL of SSD1306            |
| I2C1 SDA   | PB7               | SDA of SSD1306            |
| UART2 TX   | PA2               | Connect to serial monitor |
| UART2 RX   | PA3               | Connect to serial monitor |

> Note: Adjust pins as needed based on your hardware setup.

## Usage

1. Open the project in **STM32CubeIDE**.
2. Build the project and flash it to the Nucleo-F411RE board.
3. Open a serial terminal at the configured UART2 baud rate (e.g., 115200) to view debugging output.
4. Observe OLED display behavior as tests run automatically.

## Folder Structure

```
proj_OLED_SSD1306_F411/
├─ Core/
│  ├─ Inc/      # Header files
│  └─ Src/      # Source files (main.c, i2c, oled driver, etc.)
├─ Drivers/    # HAL drivers, middleware
├─ .ioc        # STM32CubeMX project configuration
└─ README.md
```

## Future Enhancements

* Add more graphical tests (animations, custom fonts)
* Support multiple OLED resolutions
* Add menu interface for selecting test modes via buttons

## Author

Muhammad Abu Bakar
