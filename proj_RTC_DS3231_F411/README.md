# proj_RTC_DS3231_F411

## Overview

`proj_RTC_DS3231_F411` is a firmware project for the STM32 Nucleo-F411RE that interfaces with the DS3231 real-time clock (RTC) module over I2C. The project demonstrates reading and writing the current time and date, along with printing it over UART for debugging.

## Hardware

* **Microcontroller Board:** Nucleo-F411RE
* **RTC Module:** DS3231
* **Communication Interfaces:**

  * I2C1 for RTC communication
  * UART2 for debugging output

## Software

* **IDE:** STM32CubeIDE
* **Libraries:** STM32 HAL for I2C, UART, GPIO

## Features

* Read current time and date from DS3231
* Write/update time and date
* UART debug output of RTC data
* Easy-to-use `_RTC` structure for time/date management

## Wiring / Pin Configuration

| Peripheral | Nucleo-F411RE Pin | DS3231 Pin / Notes    |
| ---------- | ----------------- | --------------------- |
| I2C1 SCL   | PB6               | SCL of DS3231         |
| I2C1 SDA   | PB7               | SDA of DS3231         |
| UART2 TX   | PA2               | Serial monitor output |
| UART2 RX   | PA3               | Serial monitor input  |

> Note: Ensure proper pull-up resistors (typically 4.7kΩ) on I2C lines if not onboard.

## Usage

1. Open the project in **STM32CubeIDE**.
2. Build and flash the firmware to the Nucleo-F411RE board.
3. Connect a serial terminal at UART2 baud rate (e.g., 115200).
4. Observe printed time, date and temperature from the DS3231.

## Example Code

```c
_RTC rtc;
if (DS3231_GetTime(&rtc)) {
    printf("Time: %02d:%02d:%02d, Date: %02d/%02d/20%02d, DayOfWeek: %d\n",
           rtc.Hour, rtc.Min, rtc.Sec,
           rtc.Date, rtc.Month, rtc.Year,
           rtc.DaysOfWeek);
} else {
    printf("Failed to read time from DS3231.\n");
}
```

## Folder Structure

```
proj_RTC_DS3231_F411/
├─ Core/
│  ├─ Inc/      # Header files
│  └─ Src/      # Source files (main.c, rtc driver, i2c, uart)
├─ Drivers/    # HAL drivers
├─ .ioc        # STM32CubeMX project configuration
└─ README.md
```

## Author

Muhammad Abu Bakar
