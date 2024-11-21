#include "uart.h"
#include "submission/eeprom.h"

int main() {
  // variable to track if the eeprom has been dumped
  int dumped = 0;
  
  // init uart
  // init conig
  // init led
  // init log
  // init rtc
  // init vpd

  // led blink pattern "--- -.-"
  // set RTC date/time to "01/01/2019 00:00:00"
  
  // write the mandatory SER line and name to uart \n
  // read/Write the model to uart \n
  // read/Write the manufactuer to uart \n
  // read/write the token to uart \n

  // set the config_use_static_ip to 1
  // set the config modified state
  // clear the event log
  // add 3 recods to event log values: 0xaa, 0xbb, and 0xc
  test_eeprom_operations();
  
  while(1) {
    // update the blink fsm every

    // if (log has changed && eeprom is not busy) 
      // update the log in EEPROM

    // if (config has changed && eeprom is not busy)
      // update config in EEPROM

    // if ((!eeprom_isbusy()) && (!dumped)) {
    //       dump_eeprom(0,0x100);
    //       dumped=1;
    // }

  }
}