# proj_RTC_DS1307_F411

## Overview

`proj_RTC_DS1307_F411` is a firmware project for the STM32 Nucleo-F411RE that interfaces with the DS1307 real-time clock (RTC) module over I2C. The project demonstrates reading and writing time/date and printing the results over UART for debugging.

## Hardware

* **Microcontroller:** Nucleo-F411RE
* **RTC Module:** DS1307
* **Interfaces:**

  * **I2C1** → DS1307
  * **UART2** → Debug output

## Software

* **IDE:** STM32CubeIDE
* **Libraries:** HAL I2C, HAL UART

## Features

* Read current time/date from DS1307
* Write/update RTC time/date
* UART printout for debugging

## Wiring

| STM32 Pin | Function | DS1307 Pin |
| --------- | -------- | ---------- |
| PB6       | I2C1_SCL | SCL        |
| PB7       | I2C1_SDA | SDA        |
| PA2       | UART2_TX | USB-TTL RX |
| PA3       | UART2_RX | USB-TTL TX |

> Make sure pull-up resistors exist on SCL/SDA (usually onboard on DS1307 modules).

## Usage

1. Open the project in **STM32CubeIDE**.
2. Build and flash the firmware.
3. Open a serial terminal (115200 baud).
4. View real-time output from the DS1307.

## Example Output

```c
    printf("%04d-%02d-%02d %02d:%02d:%02d %s\r\n",
            B2D(year)+2000,
            B2D(month & 0x1F),
            B2D(date & 0x3F),
            B2D(hours & 0x07),
            B2D(minutes & 0x7F),
            B2D(seconds & 0x7F),
            day_of_week);  // Print the day of the week

```

## Folder Structure

```
proj_RTC_DS1307_F411/
├─ Core/
│  ├─ Inc/
│  └─ Src/
├─ Drivers/
├─ .ioc
└─ README.md
```

## Author

Muhammad Abu Bakar
