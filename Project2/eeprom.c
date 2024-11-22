/********************************************************
 * eeprom.c
 *
 * SER486 Project 2
 * Fall 2024
 * Written By:  DaVonte Carter Vault
 *
 * this file defines the functions for accessing the
 * EEPROM on the ATMega328P. It provides 3 public
 * functions 
 * the functions are:
 * void eeprom_writebuf(unsigned int addr, unsigned char *buf, unsigned char size)
 *      - This function places the specified data into the write buffer for later 
 *        writing to the EEPROM. 
 * 
 * void eeprom_readbuf(unsigned int addr, unsigned char *buf, unsigned char size)
 *      - Reads a specified amount of data from the EEPROM starting at the specified
 *        address and stores it in the provided buffer.
 * void eeprom_is_busy()
 *      - Checks the write status of the EEPROM.
 * 
 */


#include "eeprom.h"
#include "uart.h"

/* 64 bytes / size of unsigned character*/
#define BUFSIZE ((512) / sizeof(unsigned char)) /* max buffer size*/
#define CACHE_SIZE 1

 /* Define relevant Registers */
#define EEARL (*((volatile unsigned char *)0x41)) /* Low Byte */
#define EEARH (*((volatile unsigned char *)0x42)) /* High Byte */
#define EEAR  (*(volatile unsigned int *)0x41)    /* (16-bit) */
#define EEDR (*((volatile unsigned char *)0x40))  /* Data Register */
#define EECR (*((volatile unsigned char *)0x3F))  /* Control Register */
#define EEMPE 2  /* Write Enable bit */
#define EEPE  1  /* Write Enable bit */
#define EERIE 3  /* Ready Interrupt Enable bit */
#define EERE  0  /* Read Enable bit */

#pragma GCC push_options
#pragma GCC optimize("Os")
/************************************************
 * eeprom_unlock
 * Unlock the eeprom for writing
 * Arguments ...
 * Returns ...
 * Changes ...
 */
void eeprom_unlock() {
  EECR |= (1 << EEMPE);  /* Write logical one to EEMPE */
  EECR |= (1 << EEPE); /* Start eeprom write by setting EEPE */
}
#pragma GCC pop_options


/* Internal State */
unsigned char writebuf[BUFSIZE]; /* Global EEPROM write buffer */
unsigned char bufidx; /* Global EEPROM write buffer index */
unsigned char writesize; /* Global EEPROM write buffer size */
unsigned int writeaddr; /* Global EEPROM write buffer target address */
volatile unsigned char write_busy; /* Global EEPROM write state */

/* declare private methods to enable and disable interrupt */
void enable_interrupt();
void disable_interrupt();
unsigned char cache_get(unsigned int addr, unsigned char *buf, unsigned char size);
void cache_del(unsigned int addr);
void cache_write(unsigned int addr, unsigned char *buf, unsigned char size);

/************************************************
 * __vector_22
 * Interrupt Service Routine (ISR) for EEPROM operations.
 * Description: This ISR is triggered when the write buffer is updated 
 *              with new data by the writebuf() function. It sends one 
 *              byte at a time to the EEPROM. 
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Sends data to EEPROM.
 *    - Disables EEPROM interrupts after the final byte is sent.
 ************************************************/
void __vector_22() {

  /* if you haven't written all characters in then keep writing */
  /* should we check to see if it is ready to write? */
  if (bufidx < writesize) {
    /* set high byte */
    EEARH = (writeaddr >> 8) & 0xFF;
    /* set low byte */
    EEARL = writeaddr & 0xFF;
    /* write data to the eeprom buffer */
    EEDR = writebuf[bufidx];
    /* unlock eeprom and start the write */
    eeprom_unlock();
    /* go to the next letter in buffer and write to the next address */
    bufidx++;
    writeaddr++;
  } else {

    /* disable interrupts */
    disable_interrupt();
    /* make ready to write another */
    write_busy = 0;
  }
}

/************************************************
 * eeprom_writebuf
 * Description: This function places the specified data 
 *              into the write buffer for later 
 *              writing to the EEPROM. 
 * 
 * Arguments:
 *    - addr: The starting address in EEPROM where the data will be written.
 *    - buf: Pointer to the buffer containing the data to be written.
 *    - size: Number of bytes to write from the buffer to the EEPROM.
 * 
 * Returns: None.
 * Changes:
 *    - Populates the write buffer with the provided data.
 ************************************************/
void eeprom_writebuf(unsigned int addr, unsigned char *buf,
                     unsigned char size) {
 /* check that write_busy is 0 */
  if (write_busy > 0) {
    return;
  }
  if ((addr + size) > 0x3FF) {
    return;
  }
  cache_write(addr, buf, size);
  /* the EEPROM is now writing */
  write_busy = 1;
  /* change the write address */
  writeaddr = addr;
  /* start reading from the beginning of the buffer */
  bufidx = 0;

  /* copy buf to writebuf and configure writesize */
  for (unsigned char i = 0; i < size; i++) {
    writebuf[i] = buf[i];
  }

  writesize = size;

  /* enable the EEPROM ready interrupts */
  enable_interrupt();
}

/************************************************
 * eeprom_readbuf
 * Description: Reads a specified amount of data from the EEPROM 
 *              starting at the specified address and stores it in 
 *              the provided buffer.
 * 
 * Arguments:
 *    - addr: The starting address in the EEPROM to read from.
 *    - buf: Pointer to the buffer where the read data will be stored.
 *    - size: The number of bytes to read from the EEPROM.
 * 
 * Returns: None.
 * Changes:
 *    - Populates the provided buffer with data read from the EEPROM.
 ************************************************/
void eeprom_readbuf(unsigned int addr, unsigned char *buf, unsigned char size) {
  if (size > 64) {
    /* write error. buffer is too big */
    return;
  }
  while (eeprom_isbusy())
  {
    if (cache_get(addr, buf, size)) { return; }
  }
  for (unsigned char i = 0; i < size; i++) {
    /* set high byte */
    EEARH = (addr >> 8) & 0xFF;
    /* set low byte */
    EEARL = addr & 0xFF;
    /* set mode to read */
    EECR |= (1 << EERE);
    /* write from register to buffer */
    buf[i] = EEDR;
    addr++;
  }
}

/************************************************
 * eeprom_isbusy
 * Description: Checks the write status of the EEPROM.
 * 
 * Arguments: None.
 * 
 * Returns:
 *    - 0: EEPROM is not busy (write_busy == 0).
 *    - 1: EEPROM is busy (write_busy == 1).
 * 
 * Changes: None.
 ************************************************/
int eeprom_isbusy() {
  return  write_busy == 1;
}

/************************************************
 * enable_interrupt
 * Description: Enables EEPROM interrupt
 * 
 * Arguments: None.
 * 
 * Returns: None.
 * 
 * Changes:
 *    - Sets the interrupt enable flag or specific interrupt registers, 
 *      allowing the system to respond to interrupt events.
 ************************************************/
void enable_interrupt() { 
  EECR |= (1 << EERIE); 
}

/************************************************
 * disable_interrupt
 * Description: Disables EEPROM interrupt
 * 
 * Arguments: None.
 * 
 * Returns: None.
 * 
 * Changes:
 *    - Sets the interrupt disable flag or specific interrupt registers, 
 *      allowing the system to respond to interrupt events.
 ************************************************/
void disable_interrupt() { EECR &= ~(1 << EERIE); }


// do I prioritize memory or speed?
struct cache_struct {
    unsigned char index;
    unsigned int addr;
    unsigned char size;
    char data[BUFSIZE];
};


struct cache_struct cache[CACHE_SIZE];
unsigned char cache_counter = 0;
void cache_init() {
    for(int i =0; i < CACHE_SIZE; i++) 
    {
        /* set all cache to invalid state */
        cache[i].index = 0xFF;
    }
}

// add to cache
void cache_write(unsigned int addr, unsigned char *buf, unsigned char size) {
    if (size > BUFSIZE) {
        return;
    }
    /* create a variable to store available index */
    unsigned char available_idx = 0;
    unsigned char last_seen_idx = 0;
    
    // go through the list of cache items
    for(int i = 0; i < CACHE_SIZE; i++) 
    {
        // if the address exists already overwrite it
        if (cache[i].addr == addr) 
        {
            for (unsigned char j = 0; j < size; j++) 
            {
                cache[i].data[j] = buf[j];
            }
            cache[i].size = size;
            cache[i].index = cache_counter++;
            return;
        }

        // if current cache[i].index is < last_seen_idx
        if (cache[i].index < last_seen_idx || (cache[i].index == 0xFF)) 
        {
            last_seen_idx = cache[i].index;
            available_idx = i;
        }
    } // address not found
    
    // if the address wasn't found and there is space in the cache, write to the first available
    // if the address wasn't found and there is no space, delete the first one. first in first out
    cache[available_idx].addr = addr;
    cache[available_idx].index = cache_counter++;
    for (unsigned char i = 0; i < size; i++) 
    {
        cache[available_idx].data[i] = buf[i];
    }
}

// delete from cache
void cache_del(unsigned int addr) {
    // go through the list of cache items
    for(int i = 0; i < CACHE_SIZE; i++) 
    {
        // if the address exists already overwrite INDEX
        if (cache[i].addr == addr) 
        {
            cache[i].index = 0xFF;
            return;
        }
    } // address not found
}

// read from cache
unsigned char cache_get(unsigned int addr, unsigned char *buf, unsigned char size) {
    // go through the list of cache items
    for(int i = 0; i < CACHE_SIZE; i++) 
    {
        // if the address exists already overwrite INDEX
        if (cache[i].addr == addr) 
        {
            for (unsigned char j = 0; j < size; j++) 
            {
                buf[j] = cache[i].data[j]; 
            }
            return 1;
        }
    } // address not found
 return 0;
}
