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

void update_checksum(unsigned char *data, unsigned int size)
{
    unsigned int sum = 0;
    unsigned int i;

    /* Edge case: If size is zero or one, nothing to update */
    if (size <= 1)
    {
        return;
    }

    /* Sum all bytes except the last one (which is the checksum byte) */
    for (i = 0; i < size - 1; i++)
    {
        sum += (unsigned char)data[i];
    }

    /* Calculate the checksum so that the total sum modulo 256 is zero */
    data[size - 1] = (unsigned char)(-(sum & 0xFF));
}

int is_checksum_valid(unsigned char *data, unsigned int size)
{
    unsigned int sum = 0;
    unsigned int i;

    /* Edge case: If size is zero or one, checksum cannot be valid */
    if (size <= 1)
    {
        return 0;
    }

    /* Sum all bytes including the checksum byte */
    for (i = 0; i < size; i++)
    {
        sum += data[i];
    }

    /* Check if the total sum modulo 256 is zero */
    if ((sum & 0xFF) == 0)
    {
        return 1; /* Checksum is valid */
    }
    else
    {
        return 0; /* Checksum is invalid */
    }
}

int cmp_str(const char *str1, const char *str2)
{
  while (*str1 && *str2)
  {
    if (*str1 != *str2)
    {
      return 0;
    }
    str1++;
    str2++;
  }
  return *str1 == '\0' && *str2 == '\0';
}