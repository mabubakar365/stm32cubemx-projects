# proj_BMP280_F411_SPI

## Overview

This project interfaces the **BMP280** sensor with the **Nucleo-F411RE** using **SPI communication**. The sensor data (temperature & pressure) is read and printed via UART for debugging.

---

## Hardware

* **MCU:** STM32 Nucleo-F411RE
* **Sensor:** Bosch BMP280 (SPI mode)

### BMP280 SPI Pin Connections

| BMP280 Pin | STM32 Pin | Function                 |
| ---------- | --------- | ------------------------ |
| VCC        | 3.3V      | Power                    |
| GND        | GND       | Ground                   |
| SCK        | PB10      | SPI Clock                |
| MOSI       | PC3       | Master Out               |
| MISO       | PC2       | Master In                |
| CSB        | PC4       | Chip Select (active low) |

> Ensure SPI mode is correctly configured (MODE 0: CPOL=0, CPHA=0).

---

## Software

* **IDE:** STM32CubeIDE
* **Communication:** SPI1 (or configured SPI peripheral)
* **Debug via:** UART2 @ 115200 baud
* **Libraries:** HAL SPI, HAL UART

---

## Function to Read Data

```c
bmp280_app_run();
```

Call this function in your main loop to fetch temperature and pressure.

---

## Example Usage

```c
while (1)
{
    bmp280_app_run();   // Read BMP280 via SPI
}
```

---

## Folder Structure

```
proj_BMP280_F411_SPI/
├─ Core/
│  ├─ Inc/
│  └─ Src/
├─ Drivers/           # HAL + BMP280 SPI driver
├─ .ioc               # STM32CubeMX configuration
└─ README.md
```

---

## Notes

* SPI speed should be below **10 MHz**.
* CSB must remain low during SPI transaction.
* Ensure correct register addressing for SPI mode (MSB must be 1 for reads).

---

## Author

**Muhammad Abu Bakar**
