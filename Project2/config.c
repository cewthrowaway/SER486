/********************************************************
 * config.c
 *
 * SER486 Project 2
 * Fall 2024
 * Written By:  DaVonte Carter Vault
 *
 * this file defines the functions for reading, writing, and managing
 * the config data on the ATMega328P. It provides 3 functions: 
 * void config_init()
 *      -
 * void config_update()
 *      -
 * void config_set_modified()
 *      -
 */

#include "eeprom.h"
#include "util.h"
#include "config.h"

/*********** Private variables ***********/
/* define the default */
struct config_struct config_defaults = {"ASU", 0x3FE, 0x3FE, 0x0000, 0x0001, 0, {192, 168, 1, 100}, 0};
/* set the modified as false */
unsigned char config_modified = 0;

/*********** Private declarations ***********/
int config_is_data_valid();
void config_write_defaults();
void config_read();

/*********** Public definitions ***********/
/************************************************
 * config_init
 * Description: initializes the system config and reads data 
 *              from the EEPROM and ensures it is valid
 * 
 * Arguments: None
 * 
 * Returns: None.
 * Changes:
 *    - Reads from EEPROM to local storage
 *    - update EEPROM with default data
 *    - sets config_modified to 0
 ************************************************/
void config_init()
{

  // wait for the eeprom to not be busy
  while (eeprom_isbusy())
    ;

  /* init config w/ eeprom read */
  eeprom_readbuf(0x0040, (unsigned char *)&config, sizeof(config));
  /* while the data is not valid */
  while (!config_is_data_valid())
  {
    /* fix the data in eeprom*/
    config_write_defaults();
    config_read();
  }
  config_modified = 0;
}
/************************************************
 * config_update
 * Description: Updatse the EEPROM with current configuration data if updates were made
 * 
 * Arguments: None
 * 
 * Returns: None.
 * Changes:
 *    - update checksum for the config
 *    - writes new config to EEPROM
 *    - modified_config is set 0
 ************************************************/
void config_update()
{
  /* if eeprom is busy or !modified just return */
  if (eeprom_isbusy() || !config_modified)
    return;
  /* update the checksum */
  update_checksum((unsigned char *)&config, sizeof(config));
  /* write config to eeprom */
  eeprom_writebuf(0x0040, (unsigned char *)&config, sizeof(config));
  /* clear the modified flag */
  config_modified = 0;
}
/************************************************
 * config_set_modified
 * Description: Marks the configuration as modified and ready to update
 * 
 * Arguments: None
 * 
 * Returns: None.
 * Changes:
 *    - sets config as modified and ready to update
 ************************************************/
void config_set_modified()
{
  config_modified = 1;
}

/*********** Private definitions ***********/
/************************************************
 * config_is_data_valid
 * Description: checks if configuration is currently in a valid state and returns 1 for yes 0 for no
 * 
 * Arguments: None
 * 
 * Returns: None.
 * Changes: None.
 ************************************************/
int config_is_data_valid()
{
  return cmp_str(config.token, "ASU") && is_checksum_valid((unsigned char *)&config, sizeof(config));
}

/************************************************
 * config_write_defaults
 * Description: Writes the default configuration values to the EEPROM 
 *              and ensures data integrity by updating the checksum
 * 
 * Arguments: None.
 * 
 * Returns: None.
 * Changes:
 *    - writes config to eeprim 0x0040
 ************************************************/
void config_write_defaults()
{
  /* upate_checksum() for defaults*/
  update_checksum((unsigned char *)&config_defaults, sizeof(config_defaults));
  /* write defaults to eeprom */
  eeprom_writebuf(0x0040, (unsigned char *)&config_defaults, sizeof(config_defaults));
}

/************************************************
 * config_read
 * Description: Reads the config data from EEPROM and loads it into config
 * 
 * Arguments: None.
 * 
 * Returns: None.
 * Changes:
 *    - Populates the provided buffer with data read from the EEPROM.
 ************************************************/
void config_read()
{
  eeprom_readbuf(0x0040, (unsigned char *)&config, sizeof(config));
}