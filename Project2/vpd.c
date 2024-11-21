#include "eeprom.h"
#include "util.h"
#include "vpd.h"

struct vpd_struct vpd;
struct vpd_struct defaults = {"SER", "DaVonte", "Carter", "super1337", 0, {'D'&FE,'a','V','C','a','r'}, "USA", 0}

/********** private declarations **********/
/* read manufacturer data from eeprom */
void read_vpd();


/********** public definitions **********/
void vpd_init() {
  // wait for the eeprom to free up
  while(isbusy());
  
  /* read all data from the EEPROM at address 0x000 */

  /* if the data was invalid then write defaults and try to reinitialize */
  while(!is_data_valid()) {
    // initialize data from defaults
    // read defaults
  }
}

void vpd_write_defaults() {
  /* update checksum for defaults */
  
  /* write the defaults */


}

void vpd_is_data_valid() {
  return (vpd.token == "SER") && is_checksum_valid();
}

/********** private definitions **********/

