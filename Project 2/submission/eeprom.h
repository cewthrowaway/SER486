
/* this function places the data (specified buf and size) into the write buffer
 * for later writing to the EEPROM. The addr parameter specifies the location to
 * write the data to. */
/* This function should not be called when another write is in progress. */
void eeprom_writebuf(unsigned int, unsigned char *, unsigned char);
/* this function reads a specified amount of data(size) from the EEPROM starting
 * at a specified address(addr) and places it in the specified buffer(buf) */
void eeprom_readbuf(unsigned int, unsigned char *, unsigned char);
/* this functiosn returns 0 if write_busy is 0, otherwise, returns 1 */
int eeprom_isbusy();
