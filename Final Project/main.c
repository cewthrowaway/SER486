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
	/* Initialize the hardware devices
	 * uart
	 * led
	 * vpd
	 * config
     * log
     * rtc
     * spi
     * temp sensor
	 * W51 Ethernet controller
     * tempfsm
     */

    /* sign the assignment
    * Asurite is the first part of your asu email (before the @asu.edu
    */
    signature_set("firstname","lastname","asurite");

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


    while (1) {
        /* reset  the watchdog timer every loop */

        /* update the LED blink state */

        /* if the temperature sensor delay is done, update the current temperature
        * from the temperature sensor, update the temperature sensor finite state
        * machine (which provides hysteresis) and send any temperature sensor
        * alarms (from FSM update).
        */
        {
            /* read the temperature sensor */

            /* update the temperature fsm and send any alarms associated with it */

            /* restart the temperature sensor delay to trigger in 1 second */

        } /*if the server socket is closed */ {
            /* if socket is closed, open it in passive (listen) mode */

        } /* if there is input to process */ {
            /* parse and process any pending commands */
        } /* otherwise... */ {
          /* update any pending log write backs */

          /* update any pending config write backs */
       }
    }
	return 0;
}
