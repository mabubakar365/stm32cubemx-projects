/*
 * ds1307.c
 *
 *  Created on: Nov 18, 2025
 *      Author: M. Abu Bakar
 */
#include "ds1307.h"
#include "main.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

#define RTC_ADDRESS (0x68 << 1)

uint8_t data[8];
uint8_t seconds, minutes, hours, days, date, month, year;
char msgBuffer[19]; //2017-11-18 02:46:30

static uint8_t B2D(uint8_t);
static const char* get_day_of_week(uint8_t day);

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}

void I2C_Scan(void)
{
    printf("Scanning I2C...\r\n");

    for (uint8_t addr = 1; addr < 128; addr++)
    {
        if (HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 1, 10) == HAL_OK)
        {
            printf("Found device at 0x%02X\r\n", addr);
        }
    }

    printf("Scan done.\r\n");
}

void ds1307_init()
{
	/* Set RTC 2025-03-02 Saturday 01:02 */
	data[0] = 0x00;       //address
	data[1] = 0x30;       //seconds
	data[2] = 0x46;       //minutes
	data[3] = 0x02;       //hours
	data[4] = 0x03;       //days (Saturday)
	data[5] = 0x18;       //date
	data[6] = 0x11;       //month
	data[7] = 0x25;       //year

	HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDRESS, data, 8, 50);
	printf("DS1307 System Initialized Successfully...\r\n");
}

void ds1307_test()
{
    data[0] = 0x00;     //register address
    HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDRESS, data, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, RTC_ADDRESS|0x01, &seconds, 1, 50);

    data[0] = 0x01;
    HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDRESS, data, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, RTC_ADDRESS|0x01, &minutes, 1, 50);

    data[0] = 0x02;
    HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDRESS, data, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, RTC_ADDRESS|0x01, &hours, 1, 50);

    data[0] = 0x03;
    HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDRESS, data, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, RTC_ADDRESS|0x01, &days, 1, 50);

    data[0] = 0x04;
    HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDRESS, data, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, RTC_ADDRESS|0x01, &date, 1, 50);

    data[0] = 0x05;
    HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDRESS, data, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, RTC_ADDRESS|0x01, &month, 1, 50);

    data[0] = 0x06;
    HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDRESS, data, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, RTC_ADDRESS|0x01, &year, 1, 50);

    const char* day_of_week = get_day_of_week(days); // Get the day name from the day number

    printf("%04d-%02d-%02d %02d:%02d:%02d %s\r\n",
            B2D(year)+2000,
            B2D(month & 0x1F),
            B2D(date & 0x3F),
            B2D(hours & 0x07),
            B2D(minutes & 0x7F),
            B2D(seconds & 0x7F),
            day_of_week);  // Print the day of the week

    HAL_Delay(1000);
}

static uint8_t B2D(uint8_t byte)
{
  uint8_t low, high;
  low = byte & 0x0F;
  high = ((byte >> 4) & 0x0F) * 10;
  return high + low;
}

static const char* get_day_of_week(uint8_t day)
{
    switch (day) {
        case 1: return "Sunday";
        case 2: return "Monday";
        case 3: return "Tuesday";
        case 4: return "Wednesday";
        case 5: return "Thursday";
        case 6: return "Friday";
        case 7: return "Saturday";
        default: return "Unknown";
    }
}
