/*
 * max30100.c
 *
 *  Created on: Nov 18, 2025
 *      Author: M. Abu Bakar
 */
#include "max30100.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t newValue = 0;

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

uint16_t IR = 0;      // Last IR reflectance datapoint
uint16_t RED = 0;     // Last Red reflectance datapoint

static void    I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data);
static uint8_t I2CreadByte(uint8_t address, uint8_t subAddress);
static void    I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count);
static long meanDiff(int M);

static void I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
    uint8_t buf[2] = { subAddress, data };
    if(HAL_I2C_Master_Transmit(&hi2c1, address << 1, buf, 2, HAL_MAX_DELAY) != HAL_OK)
    {
    	printf("I2C Write Error\r\n");
    }
}


static uint8_t I2CreadByte(uint8_t address, uint8_t subAddress)
{
    uint8_t data = 0;

    // Tell sensor which register to read
    if(HAL_I2C_Master_Transmit(&hi2c1, address << 1, &subAddress, 1, HAL_MAX_DELAY) != HAL_OK)
    {
    	return 0;
    }

    // Read 1 byte
    if(HAL_I2C_Master_Receive(&hi2c1, address << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK)
    {
    	return 0;
    }

    return data;
}

static void I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t *dest, uint8_t count)
{
    // Select starting register
    if(HAL_I2C_Master_Transmit(&hi2c1, address << 1, &subAddress, 1, HAL_MAX_DELAY) != HAL_OK)
    {
    	printf("I2C Write Error\r\n");
    }

    // Read sequential bytes
    if(HAL_I2C_Master_Receive(&hi2c1, address << 1, dest, count, HAL_MAX_DELAY) != HAL_OK)
    {
    	printf("I2C Read Error\r\n");
    }
}

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

void MAX30100_setLEDs(pulseWidth pw, ledCurrent red, ledCurrent ir){
  uint8_t reg = I2CreadByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG);
  reg = reg & 0xFC; // Set LED_PW to 00
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, reg | pw);     // Mask LED_PW
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_LED_CONFIG, (red<<4) | ir); // write LED configs
}

void MAX30100_setSPO2(sampleRate sr){
  uint8_t reg = I2CreadByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG);
  reg = reg & 0xE3; // Set SPO2_SR to 000
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, reg | (sr<<2)); // Mask SPO2_SR
  reg = I2CreadByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG);
  reg = reg & 0xf8; // Set Mode to 000
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, reg | 0x03); // Mask MODE
}

void MAX30100_setInterrupt(interruptSource intsrc){
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_INT_ENABLE, ((intsrc + 1)<<4));
  I2CreadByte(MAX30100_ADDRESS, MAX30100_INT_STATUS);
}

int MAX30100_getNumSamp(void){
    uint8_t wrPtr = I2CreadByte(MAX30100_ADDRESS, MAX30100_FIFO_WR_PTR);
    uint8_t rdPtr = I2CreadByte(MAX30100_ADDRESS, MAX30100_FIFO_RD_PTR);
    return (abs( 16 + wrPtr - rdPtr ) % 16);
}

void MAX30100_readSensor(void){
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
  IR = (temp[0]<<8) | temp[1];    // Combine values to get the actual number
  RED = (temp[2]<<8) | temp[3];   // Combine values to get the actual number
}

void MAX30100_shutdown(void){
  uint8_t reg = I2CreadByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG);  // Get the current register
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, reg | 0x80);   // mask the SHDN bit
}

void MAX30100_reset(void){
  uint8_t reg = I2CreadByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG);  // Get the current register
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, reg | 0x40);   // mask the RESET bit
}

void MAX30100_startup(void){
  uint8_t reg = I2CreadByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG);  // Get the current register
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, reg & 0x7F);   // mask the SHDN bit
}

int MAX30100_getRevID(void){
  return I2CreadByte(MAX30100_ADDRESS, MAX30100_REV_ID);
}

int MAX30100_getPartID(void){
  return I2CreadByte(MAX30100_ADDRESS, MAX30100_PART_ID);
}

void MAX30100_begin_custom(pulseWidth pw, ledCurrent ir, sampleRate sr){
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, 0x02); // Heart rate only
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_LED_CONFIG, ir);
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, (sr<<2)|pw);

  printf("MAX30100 System Initialized Successfully...\r\n");
}

void MAX30100_begin_default()
{
	MAX30100_begin_custom(pw1600, i50, sr100);
}

void MAX30100_printRegisters(void){
	uint8_t val = I2CreadByte(MAX30100_ADDRESS, MAX30100_INT_STATUS);
	printf("INT_STATUS: 0x%02X\r\n", val);
	printf("INT_STATUS (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_INT_ENABLE);
	printf("MAX30100_INT_ENABLE: 0x%02X\r\n", val);
	printf("MAX30100_INT_ENABLE (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_FIFO_WR_PTR);
	printf("MAX30100_FIFO_WR_PTR: 0x%02X\r\n", val);
	printf("MAX30100_FIFO_WR_PTR (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_OVRFLOW_CTR);
	printf("MAX30100_OVRFLOW_CTR: 0x%02X\r\n", val);
	printf("MAX30100_OVRFLOW_CTR (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_FIFO_RD_PTR);
	printf("MAX30100_FIFO_RD_PTR: 0x%02X\r\n", val);
	printf("MAX30100_FIFO_RD_PTR (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_FIFO_DATA);
	printf("MAX30100_FIFO_DATA: 0x%02X\r\n", val);
	printf("MAX30100_FIFO_DATA (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG);
	printf("MAX30100_MODE_CONFIG: 0x%02X\r\n", val);
	printf("MAX30100_MODE_CONFIG (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG);
	printf("MAX30100_SPO2_CONFIG: 0x%02X\r\n", val);
	printf("MAX30100_SPO2_CONFIG (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_LED_CONFIG);
	printf("MAX30100_LED_CONFIG: 0x%02X\r\n", val);
	printf("MAX30100_LED_CONFIG (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_TEMP_INTG);
	printf("MAX30100_TEMP_INTG: 0x%02X\r\n", val);
	printf("MAX30100_TEMP_INTG (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_TEMP_FRAC);
	printf("MAX30100_TEMP_FRAC: 0x%02X\r\n", val);
	printf("MAX30100_TEMP_FRAC (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_REV_ID);
	printf("MAX30100_REV_ID: 0x%02X\r\n", val);
	printf("MAX30100_REV_ID (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));

	val = I2CreadByte(MAX30100_ADDRESS, MAX30100_PART_ID);
	printf("MAX30100_PART_ID: 0x%02X\r\n", val);
	printf("MAX30100_PART_ID (BIN): "BYTE_TO_BINARY_PATTERN"\r\n", BYTE_TO_BINARY(val));
}

static long meanDiff(int M) {
  #define LM_SIZE 15
  static int LM[LM_SIZE];      // LastMeasurements
  static uint8_t index = 0;
  static long sum = 0;
  static uint8_t count = 0;
  long avg = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  avg = sum / count;
  return avg - M;
}

void MAX30100_App()
{
	MAX30100_readSensor();
	printf("%ld\r\n", meanDiff(IR));
	HAL_Delay(10);
}

void MAX30100_App_IT()
{
	if(newValue == 1)
	{
		MAX30100_readSensor();
		printf("%ld\r\n", meanDiff(IR));
		newValue = 0;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		newValue = 1;
	}
}
