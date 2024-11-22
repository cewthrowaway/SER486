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
 * void vpd_init()
 *      - this function initializes the vpd and writes  default values if the eeprom is incorrect
 * void vpd_write_defaults()
 *      - updates the vpd defautls in EEPROM
 * int vpd_is_data_valid()
 *      - checks that the data in the EEPROM is valid
 */

#include "eeprom.h"
#include "util.h"
#include "vpd.h"

/* define the defaults for vpd */
struct vpd_struct defaults = {"SER", "DaVonte", "Carter", "super1337", 0, {'D' & 0xFE, 'a', 'V', 'C', 'a', 'r'}, "USA", 0};

/********** private declarations **********/
/* read manufacturer data from eeprom */
void read_vpd();

/********** public definitions **********/

/************************************************
 * vpd_init
 * Description: initializes the vpd and writes default values
 *              if the eeprom is incorrect.
 * 
 * Arguments: None.
 * 
 * Returns: None.
 * Changes:
 *    - writes defaults to EEPROM
 *    - copies eeprom vpd to struct
 ************************************************/
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

/************************************************
 * vpd_write_defaults
 * Description: Updates the checksum of the vpd and 
 *              writes data to eeprom at 0x0000
 * 
 * Arguments:
 *    - addr: The starting address in the EEPROM to read from.
 *    - size: The number of bytes of data type
 * 
 * Returns: None.
 * Changes:
 *    - writes defaults for vpd to eeprom 0x0000
 ************************************************/
void vpd_write_defaults()
{
  /* update checksum for defaults */
  update_checksum((unsigned char *)&defaults, sizeof(defaults));
  /* write the defaults */
  eeprom_writebuf(0x0000, (unsigned char *)&defaults, sizeof(defaults));
}

/************************************************
 * vpd_is_data_valid
 * Description: checks if vpd token is set to "SER" and 
 *              that the checksum is valid.
 * 
 * Arguments:
 *    - addr: The starting address in the EEPROM to read from.
 *    - size: The number of bytes of data structure.
 * 
 * Returns: None.
 * Changes:
 *    None
 ************************************************/
int vpd_is_data_valid()
{
  return cmp_str(vpd.token, "SER") && is_checksum_valid((unsigned char *)&vpd, sizeof(vpd));
}

/********** private definitions **********/
/************************************************
 * read_vpd
 * Description: Helper function to read data from the
 *              EEPROM.
 * 
 * Arguments:
 *    - addr: The starting address in the EEPROM to read from.
 *    - size: The number of bytes of data structure.
 * 
 * Returns: None.
 * Changes:
 *    None
 ************************************************/
void read_vpd()
{
  eeprom_readbuf(0x0000, (unsigned char *)&vpd, sizeof(vpd));
}