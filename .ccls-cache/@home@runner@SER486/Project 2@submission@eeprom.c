#include "eeprom.h"

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

/* Enabled when writebuf() places new data in the write buffer, this ISR sends
 * one byte at a time to the EEPROM. When the last byte is sent, it disables
 * futher EERPOM interrupts */
void __vector__22(){
  
}

/* this function places the data (specified buf and size) into the write buffer
 * for later writing to the EEPROM. The addr parameter specifies the location to
 * write the data to. */
/* This function should not be called when another write is in progress. */
void writebuf(unsigned int addr, unsigned char *buf, unsigned char size) {
  // check that write_busy is 0
  if (write_busy > 0) {
    // write to the console

    return;
  }
  if (size > 64) {
    /* write error. buffer is too big */

    return;
  }
  /* the EEPROM is now writing */
  write_busy = 1;
  /* change the write address */
  writeaddr = addr;
  /* start reading from the beginning of the buffer */
  bufidx = 0;
  /* copy the information to write */
  /* copy buf to writebuf */

  /* copy the size */
  writesize = size;

  /* enable the EEPROM ready interrupts */
  
}

/* this function reads a specified amount of data(size) from the EEPROM starting
 * at a specified address(addr) and places it in the specified buffer(buf) */
void readbuf(unsigned int addr, unsigned char *buf, unsigned char size) {}

/* this functiosn returns 0 if write_busy is 0, otherwise, returns 1 */
int isbusy() { return write_busy; }
