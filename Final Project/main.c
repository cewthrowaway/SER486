/*************************************************************************
* main.c
*
* This is template code for the SER486 final exam.  Students may use this
* as a starting point their final project.
*/
#include "config.h"
#include "delay.h"
#include "dhcp.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "spi.h"
#include "uart.h"
#include "vpd.h"
#include "temp.h"
#include "socket.h"
#include "alarm.h"
#include "wdt.h"
#include "tempfsm.h"
#include "eeprom.h"
#include "ntp.h"
#include "w51.h"
#include "signature.h"

#define HTTP_PORT       8080	/* TCP port for HTTP */
#define SERVER_SOCKET   0

int current_temperature = 75;

int main(void)
{
	uart_init();
	led_init();
	vpd_init();
	config_init();
	log_init();
	rtc_init();
	spi_init();
	temp_init();
	W51_init();
	tempfsm_init();

    /* sign the assignment
    * Asurite is the first part of your asu email (before the @asu.edu
    */
    signature_set("DaVonte","Carter Vault","dcarterv");

    /* configure the W51xx ethernet controller prior to DHCP */
    unsigned char blank_addr[] = {0,0,0,0};
    W51_config(vpd.mac_address, blank_addr, blank_addr, blank_addr);

    /* loop until a dhcp address has been gotten */
    while (!dhcp_start(vpd.mac_address, 60000UL, 4000UL)) {}
    uart_writestr("local ip: ");uart_writeip(dhcp_getLocalIp());

    /* configure the MAC, TCP, subnet and gateway addresses for the Ethernet controller*/
    W51_config(vpd.mac_address, dhcp_getLocalIp(), dhcp_getGatewayIp(), dhcp_getSubnetMask());

	/* add a log record for EVENT_TIMESET prior to synchronizing with network time */
	log_add_record(EVENT_TIMESET);

    /* synchronize with network time */
    ntp_sync_network_time(5);

    /* add a log record for EVENT_NEWTIME now that time has been synchronized */
    log_add_record(EVENT_NEWTIME);

    /* start the watchdog timer */
    wdt_init();

    /* log the EVENT STARTUP and send and ALARM to the Master Controller */
    log_add_record(EVENT_STARTUP);
    alarm_send(EVENT_STARTUP);

    /* request start of test if 'T' key pressed - You may run up to 3 tests per
     * day.  Results will be e-mailed to you at the address asurite@asu.edu
     */
    check_for_test_start();

    /* start the first temperature reading and wait 5 seconds before reading again
    * this long delay ensures that the temperature spike during startup does not
    * trigger any false alarms.
    */
    temp_start();
    delay_set(1,5000);


    while(1) {
       /* reset  the watchdog timer every loop */
       wdt_reset();

       /* update the led state based on the blink code */
       led_update();

       /* if the temperature sensor delay is done, update the current temperature
       * from the temperature sensor, update the temperature sensor finite state
       * machine (which provides hysteresis) and send any temperature sensor
       * alarms (from FSM update).
       */
       if (delay_isdone(1)) {
          /* read the temperature sensor */
          current_temperature = temp_get();

          tempfsm_update(current_temperature,config.hi_alarm,config.hi_warn,config.lo_alarm,config.lo_warn);

          /* restart the temperature sensor delay to trigger in 1 second */
          temp_start();
          delay_set(1,1000);
       } else if (connected == 0) {
          /* if the connection state is 0 (unconnected), wait for the socket to connect */
          if (uartsocket_is_connected()) {
             /* socket is now connected - print a message to the uart debug port and update our connection status */
             uart_writestr("Connection Established\r\n");
             connected = 1;
          }
       } else if (uartsocket_is_packet_available()) {
           /* here if we are connected and a packet is available for processing.
           * Process the packet, update the device state and send the appropriate response to the client
           */
           parse_and_send_response();
           if (!uartsocket_is_connected()) {
              /* if connection is lost, print a debug message and update the connection state */
              uartsocket_writestr("Connection Lost\r\n");
              connected = 0;
           }
       } else {
          /* update any pending write backs */
          if (!eeprom_isbusy()) log_update();
          if (!eeprom_isbusy()) config_update();
       }
    }
	return 0;
}
