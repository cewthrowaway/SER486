#include "eeprom.h"

/* For debug mode define some macros to easily make logs to console*/
#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#define LOG_ERROR(msg) printf("Error: %s\n", msg);
#else
#define LOG_ERROR(msg) do { } while (0);
#endif

/* 64 bytes / size of unsigned character*/
#define BUFSIZE ((512) / sizeof(unsigned char))

// Internal State
unsigned char writebuf[BUFSIZE];
unsigned char bufidx;
unsigned char writesize;
unsigned int writeaddr;
unsigned char write_busy;

// Define relevant Registers
/* 7: 0 */
#define EEARL (*((volatile unsigned char *)0x41))
/* 1:0 */
#define EEARH (*((volatile unsigned char *)0x42))
/* 7:0 */
#define EEDR (*((volatile unsigned char *)0x40))
#define EECR (*((volatile unsigned char *)0x3F))
#define EEMPE 2
#define EEPE 1
#define EERIE 3
#define EERE 0
#define EE_RDY (*((volatile unsigned char *)0x2C))

/* declare private methods to enable and disable interrupt */
void enable_interrupt();
void disable_interrupt();


/* Enabled when writebuf() places new data in the write buffer, this ISR sends
 * one byte at a time to the EEPROM. When the last byte is sent, it disables
 * futher EERPOM interrupts */
/* should this bee vector 22 or 23?? Documents say 23 for EE ready */
void __vector__23(){
  // if (EECR & (1<<EEPE)) { return; } /* not ready. Don't block */

  /* if you haven't written all characters in then keep writing */
  /* should we check to see if it is ready to write? */
  if (bufidx < writesize) {
    /* set low byte */
    EEARL = writeaddr & 0xFF; 
    /* set high byte */
    EEARH = (writeaddr >> 8) & 0xFF;
    EEDR = writebuf[bufidx++];
  }
  else if(bufidx >= writesize) {
    /* take EEPROM out of write mode */
    EECR &= ~(1<<EEMPE);
    EECR &= ~(1<<EEPE);

    /* disable interrupts */
    disable_interrupt();
    /* make ready to write another */
    write_busy = 0;
  }
}

/* this function places the data (specified buf and size) into the write buffer
 * for later writing to the EEPROM. The addr parameter specifies the location to
 * write the data to. */
/* This function should not be called when another write is in progress. */
void eeprom_writebuf(unsigned int addr, unsigned char *buf, unsigned char size) {
  // check that write_busy is 0
  if (write_busy > 0) {
    // write to the console
    LOG_ERROR("Tried to write while EEPROM was busy")
    return;
  }
  if (size > 64) {
    /* write error. buffer is too big */
    LOG_ERROR("Tried to write too much data to buffer")
    return;
  }
  if ((addr + size) > 0x3FF) {
    LOG_ERROR("Address out of range")
    return;
  }
  /* the EEPROM is now writing */
  write_busy = 1;
  /* change the write address */
  writeaddr = addr;
  /* start reading from the beginning of the buffer */
  bufidx = 0;

  /* copy buf to writebuf and configure writesize */
  memcpy(writebuf, buf, size);

  writesize = size;

  /* set the EEPROM to write mode */
  EECR |= (1<<EEMPE);
  EECR |= (1<<EEPE);
  
  /* enable the EEPROM ready interrupts */
  enable_interrupt();
}

/* this function reads a specified amount of data(size) from the EEPROM starting
 * at a specified address(addr) and places it in the specified buffer(buf) */
void eeprom_readbuf(unsigned int addr, unsigned char *buf, unsigned char size) {

  // check that write_busy is 0
  if (write_busy > 0) {
    // write to the console
    LOG_ERROR("Tried to read while eeprom was busy")
    return;
  }
  if (size > 64) {
    /* write error. buffer is too big */
    LOG_ERROR("Tried to write too much data to buffer")
    return;
  }

  for (unsigned char i = 0; i < size; i++) {
    /* set the address */
    /* set low byte */
    EEARL = addr & 0xFF; 

    /* set high byte */
    EEARH = (addr >> 8) & 0xFF;

    /* set mode to read */
    EECR |= (1<<EERE);
    /* write from register to buffer */
    buf[i] = EEDR;

    addr++;
  }
  
}

/* this functiosn returns 0 if write_busy is 0, otherwise, returns 1 */
int eeprom_isbusy() { return write_busy==1; }

void enable_interrupt() {
  EECR |= (1<<EERIE);
}

void disable_interrupt() {
  EECR &= ~(1<<EERIE);
}


