#include "eeprom.h"
#include "util.h"
#include "config.h"



/*********** Private variables ***********/
/* define the default */
struct config_struct config_defaults {"ASU", 0x3FE, 0x3FE, 0x0000, 0x0001, 0, {192,168,1,100}, 0};
/* set the modified as false */
unsigned char config_modified = 0;

/*********** Private declarations ***********/
int is_data_valid();
void write_defaults();

/*********** Public definitions ***********/
void config_init() {

  // wait for the eeprom to not be busy

  /* init config w/ eeprom read */

  /* while the data is not valid */

  /* write defaults */
  /* init config with defaults */
  /* clear the modified flag */
}
void config_update() {
  /* if eeprom is busy or !modified just return */

  /* update the checksum */
  /* write config to eeprom */
  /* clear the modified flag */
}
void config_set_modified() {
  config_modified = 1;
}


/*********** Private definitions ***********/
int is_data_valid() {
  
}
void write_defaults() {
  /* upate_checksum() for defaults*/

  /* write defaults to eeprom */
}