/********************************************************
 * util.c
 *
 * SER486 Project 2
 * Fall 2024
 * Written By:  DaVonte Carter Vault
 *
 * this file defines the functions for accessing the
 * EEPROM on the ATMega328P. It provides 3 public
 * functions 
 * the functions are:
 * 
 */
#include "util.h"

/************************************************
 * update_checksum
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
void update_checksum(unsigned char *data, unsigned int size)
{
    unsigned int sum = 0;
    unsigned int i;

    /* Sum all bytes except the last one */
    for (i = 0; i < size - 1; i++)
    {
        sum += (unsigned char)data[i];
    }

    /* Calculate the checksum  */
    data[size - 1] = (unsigned char)(-(sum & 0xFF));
}
/************************************************
 * is_checksum_valid
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
int is_checksum_valid(unsigned char *data, unsigned int size)
{
    unsigned int sum = 0;
    unsigned int i;

    /* Sum all bytes including the checksum byte */
    for (i = 0; i < size; i++)
    {
        sum += data[i];
    }

    /* if the sum is 0 then valid */
    if ((sum & 0xFF) == 0)
    {
        return 1; 
    }
    else
    {
        return 0; 
    }
}
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
int cmp_str(const char *str1, const char *str2)
{
    /* while strings are valid */
  while (*str1 && *str2)
  {
    /* if the values are ever not the same, then stop */
    if (*str1 != *str2)
    {
      return 0;
    }
    /* go to the next character */
    str1++;
    str2++;
  }
  return *str1 == '\0' && *str2 == '\0';
}