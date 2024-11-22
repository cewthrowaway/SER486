/********************************************************
 * main.c
 *
 * SER486 Project 2
 * Fall 2024
 * Written By: DaVonte Carter Vault
 *
 * this file defines implements uart, eeprom, led, log, utility,
 * config, rtc, and vpd functionality.
 * 
 * 
 */
#include "uart.h"
#include "eeprom.h"
#include "led.h"
#include "log.h"
#include "util.h"
#include "config.h"
#include "rtc.h"
#include "vpd.h"

#define SREG (*(volatile unsigned char *)0x5F)
#define I_BIT 7

void enable_global_interrupts()
{
  SREG |= (1 << I_BIT);  /* Set the I bit to enable interrupts */
}

int main()
{
  enable_global_interrupts();
  /* variable to track if the eeprom has been dumped */
  int dumped = 0;
  /*  init uart */
  uart_init();
  /*  init config */
  config_init();
  /*  init led */
  led_init();
  /*  init log */
  log_init();
  /*  init rtc */
  rtc_init();
  /*  init vpd */
  vpd_init();

  /*  led blink pattern "--- -.-" */
  led_set_blink("--- -.-");
  /*  set RTC date/time to "01/01/2019 00:00:00" */
  rtc_set_by_datestr("01/01/2019 00:00:00");

  /*  write the mandatory SER line and name to uart \n */
  uart_writestr("SER 486 Project 2 – DaVonte Carter vault\n\r");
  /*  read/Write the model to uart \n */
  uart_writestr(vpd.model);
  uart_writestr("\n\r");
  /*  read/Write the manufactuer to uart \n */
  uart_writestr(vpd.manufacturer);
  uart_writestr("\n\r");
  /*  read/write the token to uart \n */
  uart_writestr(vpd.token);
  uart_writestr("\n\r");

  /*  set the config_use_static_ip to 1 */
  config.use_static_ip = 1;
  /*  set the config modified state */
  config_set_modified();
  /*  clear the event log */
  log_clear();

  /*  add 3 recods to event log values: 0xaa, 0xbb, and 0xc */
  log_add_record(0xaa);
  log_add_record(0xbb);
  log_add_record(0xcc);

  while (1)
  {
    /*  update the blink fsm every */
    led_update();

    /*  if (log has changed && eeprom is not busy) */
    /*  update the log in EEPROM */
    if (!eeprom_isbusy())
    {
      log_update();
    }

    /*  if (config has changed && eeprom is not busy) */
    /*  update config in EEPROM */
    if (!eeprom_isbusy())
    {
      config_update();
    }
    
    /* dump memory once and only once*/
    if ((!eeprom_isbusy()) && (!dumped))
    {
      dump_eeprom(0, 0x100);
      dumped = 1;
    }
  }
}