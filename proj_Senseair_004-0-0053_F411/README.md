# proj_Senseair_004-0-0053_F411

## Overview

This project interfaces the **Senseair CO₂ sensor (model 004-0-0053)** with the **STM32 Nucleo-F411RE** using **UART communication**. The firmware initializes the sensor and continuously reads CO₂ measurements, printing the results via UART for debugging.

---

## Hardware

* **MCU:** STM32 Nucleo-F411RE
* **Sensor:** Senseair 004-0-0053 CO₂ Sensor

### Pin Connections

| Sensor Pin | STM32 Pin | Function                                     |
| ---------- | --------- | -------------------------------------------- |
| G+         | 5V        | Power Supply                                 |
| G0         | GND       | Ground                                       |
| UART_RxD   | PA9       | USART1_TX (MCU receives sensor data)         |
| UART_TxD   | PA10      | USART1_RX (MCU transmits commands to sensor) |

> Ensure a **level shift** if required, as sensor uses 5V logic and MCU uses 3.3V.

---

## Software

* **IDE:** STM32CubeIDE
* **Communication:** USART1 (PA9/PA10)
* **Debug Output:** UART2
* **Libraries:** STM32 HAL UART

---

## Functions

### Sensor Initialization

```c
system_init();
```

* Call once at the start of the program to initialize the sensor.

### Main Loop Function

```c
app_run();
```

* Call repeatedly in the main loop to read CO₂ data and process it.

---

## Example Usage

```c
int main(void)
{
    system_init();  // Initialize Senseair sensor

    while (1)
    {
        app_run();      // Read sensor data continuously
    }
}
```

---

## Folder Structure

```
proj_Senseair_004-0-0053_F411/
├─ Core/
│  ├─ Inc/       # Header files
│  └─ Src/       # main.c, sensor driver, uart
├─ Drivers/      # HAL drivers
├─ .ioc          # STM32CubeMX configuration
└─ README.md
```

---

## Author

**Muhammad Abu Bakar**
