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
void config_init()
{

  // wait for the eeprom to not be busy
  while (!eeprom_isbusy())
    ;

  /* init config w/ eeprom read */
  eeprom_readbuf(0x0040, (unsigned char*)&config, sizeof(config));
  /* while the data is not valid */
  while (!config_is_data_valid())
  {
    config_write_defaults();
    config_read();
  }
  config_modified = 0;
}
void config_update()
{
  /* if eeprom is busy or !modified just return */
  if (eeprom_isbusy() || !config_modified)
    return;
  /* update the checksum */
  update_checksum((unsigned char*)&config, sizeof(config));
  /* write config to eeprom */
  eeprom_writebuf(0x0040, (unsigned char*)&config, sizeof(config));
  /* clear the modified flag */
  config_modified = 0;
}
void config_set_modified()
{
  config_modified = 1;
}

/*********** Private definitions ***********/
int config_is_data_valid()
{
  return cmp_str(config.token,"ASU") && is_checksum_valid((unsigned char*)&config,sizeof(config));
}
void config_write_defaults()
{
  /* upate_checksum() for defaults*/
  update_checksum((unsigned char*)&config_defaults, sizeof(config_defaults));
  /* write defaults to eeprom */
  eeprom_writebuf(0x0040, (unsigned char*)&config_defaults, sizeof(config_defaults));
}
void config_read()
{
  eeprom_readbuf(0x0040, (unsigned char*)&config, sizeof(config));
}