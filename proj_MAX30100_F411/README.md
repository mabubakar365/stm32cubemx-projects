# proj_MAX30100_F411

## Overview

`proj_RTC_MAX30100_F411` is a firmware project for the STM32 Nucleo-F411RE and the **MAX30100 pulse oximeter / heart rate sensor**. The project demonstrates acquiring heart rate data from the MAX30100 sensor using I2C and interrupts.

---

## Hardware

* **Microcontroller:** Nucleo-F411RE
* **Sensor:** MAX30100 (Pulse Oximeter + Heart Rate)

### MAX30100 Pin Connections

| MAX30100 Pin | STM32 Pin | Notes                                   |
| ------------ | --------- | --------------------------------------- |
| VCC          | 3.3V      | Sensor supply voltage                   |
| GND          | GND       | Ground                                  |
| SDA          | PB7       | I2C1 SDA                                |
| SCL          | PB6       | I2C1 SCL                                |
| INT          | PA0       | Active-low interrupt, connected to EXTI |

> Ensure proper pull-up resistors on SDA/SCL lines if not onboard.

---

## Software

* **IDE:** STM32CubeIDE
* **Libraries:** STM32 HAL (I2C, GPIO, EXTI, UART)

---

## Features

* Read and display current time from RTC
* Acquire heart rate data from MAX30100
* MAX30100 uses **interrupt-driven data reading**
* UART debugging

---

## Function Usage

### MAX30100 Data Read

```c
// Call this function inside your main loop or interrupt handler
MAX30100_App_IT();
```

* This function reads available data from MAX30100 FIFO when INT goes low.
* Make sure **PA0 EXTI** is configured for falling-edge detection (active-low interrupt).

---

## Wiring Diagram

```
STM32 Nucleo-F411RE       MAX30100
--------------------      --------
3.3V                      VCC
GND                       GND
PB6 (I2C1_SCL)            SCL
PB7 (I2C1_SDA)            SDA
PA0                       INT
```

---

## Folder Structure

```
proj_RTC_MAX30100_F411/
├─ Core/
│  ├─ Inc/      # Header files
│  └─ Src/      # main.c, rtc driver, max30100 driver, i2c, uart
├─ Drivers/    # HAL drivers
├─ .ioc        # STM32CubeMX project configuration
└─ README.md
```

---

## Author

Muhammad Abu Bakar
