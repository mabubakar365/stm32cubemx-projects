/*
 * utils.c
 *
 *  Created on: Nov 27, 2025
 *      Author: M. Abu Bakar
 */
#include "utils.h"
#include <stdio.h>

/* Print nibble as hex */
void printNibble(char byte) {
    char value = byte & 0xF;  // Cast to nibble

    if (value >= 10) {
        value += 7;
    }
    printf("%c\r\n", 48 + value);
}


/* Print a byte as hex */
void printByte(char byte) {
    printNibble(byte >> 4);  // high value
    printNibble(byte);       // low value
}


/* Print several bytes as hex */
void printHex(char *bytes, int size, bool space) {
    for (int i=0; i<size; i++) {
        printByte(bytes[i]);
        if (space)
            printf(" ");
    }

}


/* Print an integer as hexadecimal value */
void printIntToHex(int32_t value, int size) {
    if (size > 3)
        printByte(value >> 24);
    if (size > 2)
        printByte(value >> 16);
    if (size > 1)
        printByte(value >> 8);
    printByte(value);
}


/* Show a number in binary */
void printBinary(int16_t number) {
    int16_t k;

    for (int8_t c = 15; c >= 0; c--)
    {
        k = number >> c;

        if (k & 1) {
            printf("1");
        } else {
            printf("0");
        }

    }
}

