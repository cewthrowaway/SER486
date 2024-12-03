#ifndef HTTPPARSER_H_INCLUDED
#define HTTPPARSER_H_INCLUDED

void httpparser_init();
void httpparser_update();

/************************************************
 * cmp_str
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
int cmp_str(const char *str1, const char *str2);

#endif // HTTPPARSER_H_INCLUDED
