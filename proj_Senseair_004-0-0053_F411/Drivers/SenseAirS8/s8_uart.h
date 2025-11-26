/*
 * s8_uart.h
 *
 *  Created on: Nov 27, 2025
 *      Author: M. Abu Bakar
 */

#ifndef SENSEAIRS8_S8_UART_H_
#define SENSEAIRS8_S8_UART_H_


#include "main.h"
#include "utils.h"


#define S8_BAUDRATE 9600         // Device to S8 Serial baudrate (should not be changed)
#define S8_TIMEOUT  5000ul       // Timeout for communication in milliseconds
#define S8_LEN_BUF_MSG  20       // Max length of buffer for communication with the sensor

#define S8_LEN_FIRMVER  10       // Length of software version


// Modbus
#define MODBUS_ANY_ADDRESS                  0XFE    // S8 uses any address
#define MODBUS_FUNC_READ_HOLDING_REGISTERS  0X03    // Read holding registers (HR)
#define MODBUS_FUNC_READ_INPUT_REGISTERS    0x04    // Read input registers (IR)
#define MODBUS_FUNC_WRITE_SINGLE_REGISTER   0x06    // Write single register (SR)


// Input registers for S8
#define MODBUS_IR1             0x0000  // MeterStatus
#define MODBUS_IR2             0x0001  // AlarmStatus
#define MODBUS_IR3             0x0002  // OutputStatus
#define MODBUS_IR4             0x0003  // Space CO2
#define MODBUS_IR22            0x0015  // PWM Output
#define MODBUS_IR26            0x0019  // Sensor Type ID High
#define MODBUS_IR27            0x001A  // Sensor Type ID Low
#define MODBUS_IR28            0x001B  // Memory Map version
#define MODBUS_IR29            0x001C  // FW version Main.Sub
#define MODBUS_IR30            0x001D  // Sensor ID High
#define MODBUS_IR31            0x001E  // Sensor ID Low


// Holding registers for S8
#define MODBUS_HR1             0x0000  // Acknowledgement Register
#define MODBUS_HR2             0x0001  // Special Command Register
#define MODBUS_HR32            0x001F  // ABC Period


// Meter status
#define S8_MASK_METER_FATAL_ERROR                    0x0001   // Fatal error
#define S8_MASK_METER_OFFSET_REGULATION_ERROR        0x0002   // Offset regulation error
#define S8_MASK_METER_ALGORITHM_ERROR                0x0004   // Algorithm error
#define S8_MASK_METER_OUTPUT_ERROR                   0x0008   // Output error
#define S8_MASK_METER_SELF_DIAG_ERROR                0x0010   // Self diagnostics error
#define S8_MASK_METER_OUT_OF_RANGE                   0x0020   // Out of range
#define S8_MASK_METER_MEMORY_ERROR                   0x0040   // Memory error
#define S8_MASK_METER_ANY_ERROR                      0x007F   // Mask to detect the previous errors (fatal error ... memory error)

// Output status
#define S8_MASK_OUTPUT_ALARM                         0x0001   // Alarm output status (inverted due to Open Collector)
#define S8_MASK_OUTPUT_PWM                           0x0002   // PWM output status (=1 -> full output)


// Acknowledgement flags
#define S8_MASK_CO2_BACKGROUND_CALIBRATION   0x0020   // CO2 Background calibration performed = 1
#define S8_MASK_CO2_NITROGEN_CALIBRATION     0x0040   // CO2 Nitrogen calibration performed = 1

// Calibration definitions for special command
#define S8_CO2_BACKGROUND_CALIBRATION        0x7C06   // CO2 Background calibration
#define S8_CO2_ZERO_CALIBRATION              0x7C07   // CO2 Zero calibration


struct S8_sensor {
	char firm_version[S8_LEN_FIRMVER + 1];
	int16_t co2;
	int16_t abc_period;
	int16_t ack;
	int16_t meter_status;
	int16_t alarm_status;
	int16_t output_status;
	int16_t pwm_output;
	int32_t sensor_type_id;
	int32_t sensor_id;
	int16_t map_version;
};

/* Information about the sensor */
void S8_UART_get_firmware_version(char firmwver[]);                             // Get firmware version
int32_t S8_UART_get_sensor_type_ID();                                           // Get sensor type ID
int32_t S8_UART_get_sensor_ID();                                                // Get sensor ID
int16_t S8_UART_get_memory_map_version();                                       // Get memory map version

/* Commands to get CO2 value */
int16_t S8_UART_get_co2();                                                      // Get CO2 value in ppm
int16_t S8_UART_get_PWM_output();                                               // Get PWM output

/* Automatic calibration */
int16_t S8_UART_get_ABC_period();                                               // Get ABC period in hours
bool S8_UART_set_ABC_period(int16_t period);                                    // Set ABC period (4 - 4800 hours, 0 to disable)

/* Manual calibration */
bool S8_UART_manual_calibration();                                              // Start a manual calibration (it clears acknowledgement flags and it calls to
																		// send_special_command with background calibration command)
																		// (go to outdoors, wait 5 minutes o more and then you call this command)

/* Bits information */
int16_t S8_UART_get_acknowledgement();                                          // Get acknowledgement flags
bool S8_UART_clear_acknowledgement();                                           // Clear acknowledgement flags
int16_t S8_UART_get_meter_status();                                             // Get meter status
int16_t S8_UART_get_alarm_status();                                             // Get alarm status
int16_t S8_UART_get_output_status();                                            // Get output status

/* To execute special commands (ex: manual calibration) */
bool S8_UART_send_special_command(int16_t command);                             // Send special command
void system_init(void);
void app_run(void);

#endif /* SENSEAIRS8_S8_UART_H_ */
