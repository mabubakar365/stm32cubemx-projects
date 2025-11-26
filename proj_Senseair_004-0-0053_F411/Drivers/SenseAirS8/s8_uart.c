/*
 * s8_uart.c
 *
 *  Created on: Nov 27, 2025
 *      Author: M. Abu Bakar
 */
#include "s8_uart.h"
#include "modbus_crc.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"

static uint8_t buf_msg[S8_LEN_BUF_MSG];                                              // Buffer for communication messages with the sensor

static void S8_UART_serial_write_bytes(uint8_t size);                                        // Send bytes to sensor
static uint8_t S8_UART_serial_read_bytes(uint8_t max_bytes, uint32_t timeout_seconds);  // Read received bytes from sensor
static bool S8_UART_valid_response(uint8_t func, uint8_t nb);                                // Check if response is valid according to sent command
static bool S8_UART_valid_response_len(uint8_t func, uint8_t nb, uint8_t len);               // Check if response is valid according to sent command and checking expected total length
static void S8_UART_send_cmd(uint8_t func, uint16_t reg, uint16_t value);                    // Send command

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}

/* Get firmware version */
void S8_UART_get_firmware_version(char firmver[]) {

    if (firmver == NULL) {
        return;
    }

    strcpy(firmver, "");

    // Ask software version
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR29, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {
        snprintf(firmver, S8_LEN_FIRMVER, "%0u.%0u", buf_msg[3], buf_msg[4]);
        printf("Firmware version: %s\r\n", firmver);

    } else {
        printf("Firmware version not available!\r\n");
    }

}


/* Get CO2 value in ppm */
int16_t S8_UART_get_co2() {

    int16_t co2 = 0;

    // Ask CO2 value
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR4, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {
        co2 = ((buf_msg[3] << 8) & 0xFF00) | (buf_msg[4] & 0x00FF);
//        printf("CO2 value = %d ppm\r\n", co2);

    } else {
    	printf("Error getting CO2 value!\r\n");
    }

    return co2;
}


/* Read ABC period */
int16_t S8_UART_get_ABC_period() {

    int16_t period = 0;

    // Ask ABC period
    S8_UART_send_cmd(MODBUS_FUNC_READ_HOLDING_REGISTERS, MODBUS_HR32, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_HOLDING_REGISTERS, nb, 7)) {
        period = ((buf_msg[3] << 8) & 0xFF00) | (buf_msg[4] & 0x00FF);
        printf("ABC period = %d hours\r\n", period);

    } else {
    	printf("Error getting ABC period!");
    }

    return period;
}


/* Setup ABC period, default 180 hours (7.5 days) */
bool S8_UART_set_ABC_period(int16_t period) {
    uint8_t buf_msg_sent[8];
    bool result = false;

    if (period >= 0 && period <= 4800) {   // 0 = disable ABC algorithm

        // Ask set ABC period
    	S8_UART_send_cmd(MODBUS_FUNC_WRITE_SINGLE_REGISTER, MODBUS_HR32, period);

        // Save bytes sent
        memcpy(buf_msg_sent, buf_msg, 8);

        // Wait response
        memset(buf_msg, 0, S8_LEN_BUF_MSG);
        S8_UART_serial_read_bytes(8, S8_TIMEOUT);

        // Check response
        if (memcmp(buf_msg_sent, buf_msg, 8) == 0) {
            result = true;
            printf("Successful setting of ABC period\r\n");

        } else {
        	printf("Error in setting of ABC period!\r\n");
        }

    } else {
    	printf("Invalid ABC period!\r\n");
    }

    return result;
}


/* Read acknowledgement flags */
int16_t S8_UART_get_acknowledgement() {

    int16_t flags = 0;

    // Ask acknowledgement flags
    S8_UART_send_cmd(MODBUS_FUNC_READ_HOLDING_REGISTERS, MODBUS_HR1, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_HOLDING_REGISTERS, nb, 7)) {
        flags = ((buf_msg[3] << 8) & 0xFF00) | (buf_msg[4] & 0x00FF);
        printf("Acknowledgement flags = %d b\r\n", flags);

    } else {
    	printf("Error getting acknowledgement flags!\r\n");
    }

    return flags;
}


/* Read acknowledgement flags */
bool S8_UART_clear_acknowledgement() {
    uint8_t buf_msg_sent[8];
    bool result = false;

    // Ask clear acknowledgement flags
    S8_UART_send_cmd(MODBUS_FUNC_WRITE_SINGLE_REGISTER, MODBUS_HR1, 0x0000);

    // Save bytes sent
    memcpy(buf_msg_sent, buf_msg, 8);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    S8_UART_serial_read_bytes(8, S8_TIMEOUT);

    // Check response
    if (memcmp(buf_msg_sent, buf_msg, 8) == 0) {
        result = true;
        printf("Successful clearing acknowledgement flags\r\n");

    } else {
    	printf("Error clearing acknowledgement flags!\r\n");
    }

    return result;
}


/* Start a manual calibration (go to outdoors, wait 5 minutes o more and then you issue this command) */
bool S8_UART_manual_calibration() {
    bool result = S8_UART_clear_acknowledgement();

    if (result) {
        result = S8_UART_send_special_command(S8_CO2_BACKGROUND_CALIBRATION);

        if (result) {
        	printf("Manual calibration in background has started\r\n");

        } else {
        	printf("Error starting manual calibration!\r\n");
        }

    }

    return result;
}


/* Send special command (high = command, low = parameter) */
/*
   Command = 0x7C,
   Parameter = 0x06 CO2 background calibration
   Parameter = 0x07 CO2 zero calibration
*/
bool S8_UART_send_special_command(int16_t command) {
    uint8_t buf_msg_sent[8];
    bool result = false;

    // Ask set user special command
    S8_UART_send_cmd(MODBUS_FUNC_WRITE_SINGLE_REGISTER, MODBUS_HR2, command);

    // Save bytes sent
    memcpy(buf_msg_sent, buf_msg, 8);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    S8_UART_serial_read_bytes(8, S8_TIMEOUT);

    // Check response
    if (memcmp(buf_msg_sent, buf_msg, 8) == 0) {
        result = true;
        printf("Successful setting user special command\r\n");

    } else {
    	printf("Error in setting user special command!\r\n");
    }

    return result;
}


/* Read meter status */
int16_t S8_UART_get_meter_status() {

    int16_t status = 0;

    // Ask meter status
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR1, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {
        status = ((buf_msg[3] << 8) & 0xFF00) | (buf_msg[4] & 0x00FF);
        printf("Meter status = %d b\r\n", status);

    } else {
    	printf("Error getting meter status!\r\n");
    }

    return status;
}


/* Read alarm status */
int16_t S8_UART_get_alarm_status() {

    int16_t status = 0;

    // Ask alarm status
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR2, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {
        status = ((buf_msg[3] << 8) & 0xFF00) | (buf_msg[4] & 0x00FF);
        printf("Alarm status = %d b\r\n", status);

    } else {
    	printf("Error getting alarm status!\r\n");
    }

    return status;
}


/* Read output status */
int16_t S8_UART_get_output_status() {

    int16_t status = 0;

    // Ask output status
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR3, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {
        status = ((buf_msg[3] << 8) & 0xFF00) | (buf_msg[4] & 0x00FF);
        printf("Output status = %d b\r\n", status);

    } else {
    	printf("Error getting output status!\r\n");
    }

    return status;
}


/* Read PWM output (0x3FFF = 100%)
    Raw PWM output to ppm: (raw_PWM_output / 16383.0) * 2000.0)
    2000.0 is max range of sensor (2000 ppm for normal version, extended version is 10000 ppm)
*/
int16_t S8_UART_get_PWM_output() {

    int16_t pwm = 0;

    // Ask PWM output
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR22, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {
        pwm = ((buf_msg[3] << 8) & 0xFF00) | (buf_msg[4] & 0x00FF);
        printf("PWM output (raw) = %d\r\n", pwm);
        printf("PWM output (to ppm, normal version) = %0.3f ppm \r\n", (pwm / 16383.0) * 2000.0);
        //LOG_DEBUG_INFO("PWM output (to ppm, extended version) = ", (pwm / 16383.0) * 10000.0, " ppm");

    } else {
        printf("Error getting PWM output!");
    }

    return pwm;
}


/* Read sensor type ID */
int32_t S8_UART_get_sensor_type_ID() {

    int32_t sensorType = 0;

    // Ask sensor type ID (high)
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR26, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {

        // Save sensor type ID (high)
        sensorType = (((int32_t)buf_msg[4] << 16) & 0x00FF0000);

        // Ask sensor type ID (low)
        S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR27, 0x0001);

        // Wait response
        memset(buf_msg, 0, S8_LEN_BUF_MSG);
        nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

        // Check response and get data
        if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {

            sensorType |= ((buf_msg[3] << 8) & 0x0000FF00) | (buf_msg[4] & 0x000000FF);
            printf("Sensor Type: 0x%08lX\n", (unsigned long)sensorType);

        } else {
        	printf("Error getting sensor type ID (low)!\r\n");
        }

    } else {
    	printf("Error getting sensor type ID (high)!\r\n");
    }

    return sensorType;
}


/* Read sensor ID */
int32_t S8_UART_get_sensor_ID() {

    int32_t sensorID = 0;

    // Ask sensor ID (high)
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR30, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {

        // Save sensor ID (high)
        sensorID = (((int32_t)buf_msg[3] << 24) & 0xFF000000) | (((int32_t)buf_msg[4] << 16) & 0x00FF0000);

        // Ask sensor ID (low)
        S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR31, 0x0001);

        // Wait response
        memset(buf_msg, 0, S8_LEN_BUF_MSG);
        nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

        // Check response and get data
        if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {

            sensorID |= ((buf_msg[3] << 8) & 0x0000FF00) | (buf_msg[4] & 0x000000FF);
            printf("Sensor ID = 0x%08lX\r\n", (unsigned long) sensorID);

        } else {
        	printf("Error getting sensor ID (low)!\r\n");
        }

    } else {
    	printf("Error getting sensor ID (high)!\r\n");
    }

    return sensorID;
}


/* Read memory map version */
int16_t S8_UART_get_memory_map_version() {

    int16_t mmVersion = 0;

    // Ask memory map version
    S8_UART_send_cmd(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_IR28, 0x0001);

    // Wait response
    memset(buf_msg, 0, S8_LEN_BUF_MSG);
    uint8_t nb = S8_UART_serial_read_bytes(7, S8_TIMEOUT);

    // Check response and get data
    if (S8_UART_valid_response_len(MODBUS_FUNC_READ_INPUT_REGISTERS, nb, 7)) {
        mmVersion = ((buf_msg[3] << 8) & 0xFF00) | (buf_msg[4] & 0x00FF);
        printf("Memory map version = %d\r\n", mmVersion);

    } else {
    	printf("Error getting memory map version!\r\n");
    }

    return mmVersion;
}


/* Check valid response and length of received message */
bool S8_UART_valid_response_len(uint8_t func, uint8_t nb, uint8_t len) {
    bool result = false;

    if (nb == len) {
        result = S8_UART_valid_response(func, nb);

    } else {
    	printf("Unexpected length!\r\n");
    }

    return result;
}


/* Check if it is a valid message response of the sensor */
bool S8_UART_valid_response(uint8_t func, uint8_t nb) {

    uint16_t crc16;
    bool result = false;

    if (nb >= 7) {
        crc16 = modbus_CRC16(buf_msg, nb-2);
        if ((buf_msg[nb-2] == (crc16 & 0x00FF)) && (buf_msg[nb-1] == ((crc16 >> 8) & 0x00FF))) {

            if (buf_msg[0] == MODBUS_ANY_ADDRESS && (buf_msg[1] == MODBUS_FUNC_READ_HOLDING_REGISTERS || buf_msg[1] == MODBUS_FUNC_READ_INPUT_REGISTERS) && buf_msg[2] == nb-5) {
//            	printf("Valid response\r\n");
                result = true;

            } else {
            	printf("Unexpected response!\r\n");
            }

        } else {
        	printf("Checksum/length is invalid!\r\n");
        }

    } else {
    	printf("Invalid length!\r\n");
    }

    return result;
}


/* Send command */
void S8_UART_send_cmd( uint8_t func, uint16_t reg, uint16_t value) {

    uint16_t crc16;

    if (((func == MODBUS_FUNC_READ_HOLDING_REGISTERS || func == MODBUS_FUNC_READ_INPUT_REGISTERS) && value >= 1) || (func == MODBUS_FUNC_WRITE_SINGLE_REGISTER)) {
        buf_msg[0] = MODBUS_ANY_ADDRESS;                // Address
        buf_msg[1] = func;                              // Function
        buf_msg[2] = (reg >> 8) & 0x00FF;               // High-input register
        buf_msg[3] = reg & 0x00FF;                      // Low-input register
        buf_msg[4] = (value >> 8) & 0x00FF;             // High-word to read or setup
        buf_msg[5] = value & 0x00FF;                    // Low-word to read or setup
        crc16 = modbus_CRC16(buf_msg, 6);
        buf_msg[6] = crc16 & 0x00FF;
        buf_msg[7] = (crc16 >> 8) & 0x00FF;
        S8_UART_serial_write_bytes(8);
    }
}


/* Send bytes to sensor */
void S8_UART_serial_write_bytes(uint8_t size) {

//    printf("Bytes to send: %s %d\r\n", (char *)buf_msg, size);

    HAL_UART_Transmit(&huart1, buf_msg, size, HAL_MAX_DELAY);
}


/* Read answer of sensor */
uint8_t S8_UART_serial_read_bytes(uint8_t max_bytes, uint32_t timeout_ms) {

    uint32_t start_t = HAL_GetTick();
    uint32_t end_t = start_t;
    bool readed = false;

    uint8_t nb = 0;
    if (max_bytes > 0 && timeout_ms > 0) {

        while (((end_t - start_t) <= timeout_ms) && !readed) {
            if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
            	HAL_UART_Receive(&huart1, buf_msg, max_bytes, HAL_MAX_DELAY);
                nb = max_bytes;
                readed = true;
            }
            end_t = HAL_GetTick();
        }

        if (readed) {
            if (nb > 0) {
//                printf("Bytes received: %s %d\r\n", (char *)buf_msg, nb);

            } else {
            	printf("Unexpected reading serial port!\r\n");
            }

        } else {
        	printf("Timeout reading serial port!\r\n");
        }

    } else {
    	printf("Invalid parameters!\r\n");
    }

    return nb;
}

struct S8_sensor sensor;

void system_init(void)
{
  // Check if S8 is available
  S8_UART_get_firmware_version(sensor.firm_version);
  int len = strlen(sensor.firm_version);
  int n_tries = 1;
  while (len == 0 && n_tries < 120) {
	  printf(".");
	  HAL_Delay(250);
	  S8_UART_get_firmware_version(sensor.firm_version);
	  len = strlen(sensor.firm_version);
	  n_tries++; // give up after 120 tries
  }

  // Show basic S8 sensor info
  if (len == 0) printf("N/A\r\n");
  else printf("%s\r\n", sensor.firm_version);
  sensor.sensor_id = S8_UART_get_sensor_ID();

  printf("Sensor ID: 0x%08lX\r\n", (unsigned long)sensor.sensor_id);

  printf("Setup done!\r\n");

  printf("System Initialized\r\n");
}

int co2_value;
char buffer[17]; // to hold formated print

void app_run(void)
{
  // Get and display CO2 measure
  sensor.co2 = S8_UART_get_co2() * 1.0f;
  sprintf(buffer, "%4d", sensor.co2);
  printf("CO2 Value: %s ppm\r\n", buffer);

  // Convert the formatted string to an integer
  co2_value = atoi(buffer);

  // Print the integer value
  printf("CO2 value as integer: %d\r\n", co2_value);

  HAL_Delay(1000);
}
