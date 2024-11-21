#include "uart.h"
#include "submission/eeprom.h"


#include <assert.h>
#include <stdio.h>

// Function to test EEPROM operations
void test_eeprom_operations() {
    printf("Starting EEPROM Unit Test...\n");

    // Test buffer and parameters
    unsigned char write_buffer[64];
    unsigned char read_buffer[64];
    unsigned int test_address = 0x10;
    unsigned char test_size = 16;

    // Fill the write buffer with test data
    for (unsigned char i = 0; i < test_size; i++) {
        write_buffer[i] = i + 1;  // Test data: 1, 2, 3, ..., 16
    }

    // Test 1: Write to EEPROM
    eeprom_writebuf(test_address, write_buffer, test_size);
    // Ensure EEPROM is busy during the write
    assert(eeprom_isbusy() == 1);

    // Simulate the completion of the write
    while (eeprom_isbusy()) {
        // Simulated EEPROM interrupt-driven write occurs here
    }

    // Ensure EEPROM is no longer busy
    assert(eeprom_isbusy() == 0);

    // Test 2: Read back the data from EEPROM
    eeprom_readbuf(test_address, read_buffer, test_size);

    // Validate that the read data matches the written data
    for (unsigned char i = 0; i < test_size; i++) {
        assert(read_buffer[i] == write_buffer[i]);
    }

    printf("EEPROM Unit Test Passed!\n");
}


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