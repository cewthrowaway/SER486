#include "uart.h"
#include "eeprom.h"
#include "led.h"
#include "util.h"


void test_eeprom_uart() {
    // Define test data
    const char *test_string = "Hello, EEPROM!";
    unsigned char write_buffer[32];  // Use a buffer sized for the actual test string
    unsigned char read_buffer[32];   // Separate buffer for reading
    unsigned int test_address = 0x00; // Example EEPROM start address
    unsigned char test_size = 0;

    // Manually copy test_string into write_buffer
    for (test_size = 0; test_string[test_size] != '\0'; test_size++) {
        write_buffer[test_size] = test_string[test_size];
    }
    write_buffer[test_size] = '\0';  // Include null terminator
    test_size++;  // Add 1 for the null terminator

    // Write test data to EEPROM
    eeprom_writebuf(test_address, write_buffer, test_size);

    // Simulate EEPROM write completion
    while (eeprom_isbusy()) {
        // Simulated EEPROM ISR writes data
    }

    // Read data back from EEPROM
    eeprom_readbuf(test_address, read_buffer, test_size);

    // Null-terminate the read buffer for safety
    read_buffer[test_size - 1] = '\0';

    // Output the read data via UART
    uart_writestr("Read from EEPROM: ");
    uart_writestr((char *)read_buffer);
    uart_writestr("\n");
}

#define SREG (*(volatile unsigned char *)0x5F)
#define I_BIT 7

void enable_global_interrupts() {
    SREG |= (1 << I_BIT);  // Set the I bit to enable interrupts
}

int main() {
  enable_global_interrupts();
  // variable to track if the eeprom has been dumped
  int dumped = 0;
  test_eeprom_uart();
  // init uart
  uart_init();
  // init conig
  // init led
  led_init();
  // init log
  // init rtc
  // init vpd

  // led blink pattern "--- -.-"
  led_set_blink("--- -.-"); 
  // set RTC date/time to "01/01/2019 00:00:00"
  
  // write the mandatory SER line and name to uart \n
  // read/Write the model to uart \n
  // read/Write the manufactuer to uart \n
  // read/write the token to uart \n

  // set the config_use_static_ip to 1
  // set the config modified state
  // clear the event log
  // add 3 recods to event log values: 0xaa, 0xbb, and 0xc

  while(1) {
    // update the blink fsm every
    led_update();

    // if (log has changed && eeprom is not busy) 
      // update the log in EEPROM

    // if (config has changed && eeprom is not busy)
      // update config in EEPROM

    if ((!eeprom_isbusy()) && (!dumped)) {
          dump_eeprom(0,0x100);
          dumped=1;
    }

  }
}