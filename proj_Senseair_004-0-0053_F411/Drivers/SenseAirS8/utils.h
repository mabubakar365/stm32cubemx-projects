/*
 * utils.c
 *
 *  Created on: Nov 27, 2025
 *      Author: M. Abu Bakar
 */

#ifndef SENSEAIRS8_UTILS_C_
#define SENSEAIRS8_UTILS_C_

#include <stdbool.h>
#include <stdint.h>

/* Print nibble as hex */
void printNibble(char byte);

/* Print a byte as hex */
void printByte(char byte);

/* Print several bytes as hex */
void printHex(char *bytes, int size, bool space);

/* Print an integer as hexadecimal value */
void printIntToHex(int32_t value, int size);

/* Show a number in binary */
void printBinary(int16_t number);

#endif /* SENSEAIRS8_UTILS_C_ */
