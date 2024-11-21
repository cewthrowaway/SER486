#include "eeprom.h"
#include "util.h"
#include "vpd.h"

struct vpd_struct vpd;
struct vpd_struct defaults = {"SER", "DaVonte", "Carter", "super1337", 0, {'D'&0xFE,'a','V','C','a','r'}, "USA", 0};

/********** private declarations **********/
/* read manufacturer data from eeprom */
void read_vpd() {
}

int cmp_str(const char *str1, const char *str2) ;


/********** public definitions **********/
void vpd_init() {
  // wait for the eeprom to free up
  while(eeprom_isbusy());
  
  /* read all data from the EEPROM at address 0x000 */

  /* if the data was invalid then write defaults and try to reinitialize */
  while(!vpd_is_data_valid()) {
    // initialize data from defaults
    // read defaults
  }
}

void vpd_write_defaults() {
  /* update checksum for defaults */
  
  /* write the defaults */


}

int vpd_is_data_valid() {
  return cmp_str(vpd.token, "SER");
    // return (vpd.token == "SER") && is_checksum_valid();

}

/********** private definitions **********/

int cmp_str(const char *str1, const char *str2) {
  while (*str1 && *str2) {
    if (*str1 != *str2) {
      return 0;
    }
    str1++;
    str2++;
  }
  return *str1 == '\0' && *str2 == '\0';
}