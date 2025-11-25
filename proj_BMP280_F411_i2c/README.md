# proj_BMP280_F411

## Overview

This project demonstrates interfacing the **BMP280** pressure & temperature sensor with the **Nucleo-F411RE** using **I2C communication**. The data is read and displayed over UART.

---

## Hardware

* **MCU**: STM32 Nucleo-F411RE
* **Sensor**: Bosch BMP280 (I2C mode)

### BMP280 Pin Connections

| BMP280 Pin | STM32 Pin | Notes    |
| ---------- | --------- | -------- |
| VCC        | 3.3V      | Power    |
| GND        | GND       | Ground   |
| SDA        | PB7       | I2C1 SDA |
| SCL        | PB6       | I2C1 SCL |

> External pull-up resistors on SDA/SCL are recommended if not provided by the module.

---

## Software

* **IDE:** STM32CubeIDE
* **Libraries:** HAL I2C, HAL UART
* **Communication:** I2C1
* **Debug Output:** UART2 (115200 baud)

---

## Function to Read Sensor Data

```c
bmp280_app_run();
```

Call this function inside `main()` or periodically using a timer/task. It reads pressure & temperature and sends data via UART.

---

## Example Usage

```c
while (1)
{
    bmp280_app_run();  // Read BMP280 data
}
```

---

## Folder Structure

```
proj_BMP280_F411/
├─ Core/
│  ├─ Inc/
│  └─ Src/
├─ Drivers/        # HAL + BMP280 driver
├─ .ioc            # STM32CubeMX configuration
└─ README.md
```

---

## Author

Muhammad Abu Bakar
