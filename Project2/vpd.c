/********************************************************
 * util.c
 *
 * SER486 Project 2
 * Fall 2024
 * Written By:  DaVonte Carter Vault
 *
 * this file defines the utility functions that are
 * common to multiple functions throughout the project
 * and provides 3 functions.
 * the functions are:
 * 
 */
#include "eeprom.h"
#include "util.h"
#include "vpd.h"

struct vpd_struct defaults = {"SER", "DaVonte", "Carter", "super1337", 0, {'D' & 0xFE, 'a', 'V', 'C', 'a', 'r'}, "USA", 0};

/********** private declarations **********/
/* read manufacturer data from eeprom */
void read_vpd();

/********** public definitions **********/
void vpd_init()
{
  // wait for the eeprom to free up
  while (eeprom_isbusy())
    ;

  /* read all data from the EEPROM at address 0x000 */
  read_vpd();

  /* if the data was invalid then write defaults and try to reinitialize */
  while (!vpd_is_data_valid())
  {
    // initialize data from defaults
    vpd_write_defaults();
    // read defaults
    read_vpd();
  }
}

void vpd_write_defaults()
{
  /* update checksum for defaults */
  update_checksum((unsigned char *)&defaults, sizeof(defaults));
  /* write the defaults */
  eeprom_writebuf(0x0000, (unsigned char *)&defaults, sizeof(defaults));
}

int vpd_is_data_valid()
{
  return cmp_str(vpd.token, "SER") && is_checksum_valid((unsigned char *)&vpd, sizeof(vpd));
}

/********** private definitions **********/

void read_vpd()
{
  eeprom_readbuf(0x0000, (unsigned char *)&vpd, sizeof(vpd));
}