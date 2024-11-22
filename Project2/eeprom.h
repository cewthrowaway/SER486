/********************************************************
 * eeprom.h
 *
 * This file provides function declarations for the SER486
 * EEPROM library functions. These functions enable reading, 
 * writing, and checking the status of the EEPROM on the 
 * ATMega328P microcontroller.
 *
 *
 * Author:   Davonte Carter Vault
 * Date:     Fall 2024
 * Revision: 1.0
 *
 *
 */
#if !defined(EEPROM_H)
#define EEPROM_H


/* this function places the data (specified buf and size) into the write buffer
 * for later writing to the EEPROM. The addr parameter specifies the location to
 * write the data to. */
/* This function should not be called when another write is in progress. */
void eeprom_writebuf(unsigned int addr, unsigned char *buf, unsigned char size);
/* this function reads a specified amount of data(size) from the EEPROM starting
 * at a specified address(addr) and places it in the specified buffer(buf) */
void eeprom_readbuf(unsigned int addr, unsigned char *buf, unsigned char size);
/* this functiosn returns 0 if write_busy is 0, otherwise, returns 1 */
int eeprom_isbusy();

void __vector_22(void) __attribute__ ((signal, used, externally_visible));


void cache_init();

#endif // EEPROM_H


