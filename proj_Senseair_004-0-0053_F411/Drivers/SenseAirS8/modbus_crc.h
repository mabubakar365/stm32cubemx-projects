/*
 * modbus_crc.h
 *
 *  Created on: Nov 27, 2025
 *      Author: M. Abu Bakar
 */

#ifndef SENSEAIRS8_MODBUS_CRC_H_
#define SENSEAIRS8_MODBUS_CRC_H_

    /* The function returns the CRC as a unsigned short type
        puchMsg  -> message to calculate CRC upon
        usDataLen -> quantity of bytes in message */
    uint16_t modbus_CRC16 (uint8_t *puchMsg, uint16_t usDataLen );

#endif /* SENSEAIRS8_MODBUS_CRC_H_ */
