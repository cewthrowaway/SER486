#include "wdt.h"

/* Watch Dog Timer Control Register */
#define WDTCSR (*((volatile unsigned char *)0x60))
/* WDT control bits */
#define WDIE 6
#define WDCE 4
#define WDE 3
#define WDP2 2
#define WDP0 0

#pragma GCC push_options
#pragma GCC optimize("Os")
void wdt_disable_interrupt() 
{
    // TODO: Clear this bit correctly lol i dont feel like thinking
    WDTCSR |= (0<<WDE) | (0<<WDIE);
}

void wdt_enable_interrupt()
{
    // TODO: Clear this bit correctly lol i dont feel like thinking
    WDTCSR |= (0<<WDE) | (1<<WDIE);
}
#pragma GCC pop_options

/* add the config stuff for the vector */
void __vector_6(void) __attribute__ ((signal, used, externally_visible)) 
{
    /* turn on the status led */
    led_on();
    /* write the event to log */
    log_add_record(EVENT_WDT);
    /* try to save the log before restart */
    log_update_noisr();
    /* try to save the config before restart */
    config_updatE_noisr();
}

/* initialize the watchdog timer for a 2 second timeout and interrupt+reset mode */
void wdt_init() 
{
    wdt_disable_interrupt();
    wdt_reset();
    /* start timed sequence */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    /* adjust timeout value = 64k cycles (0.5 s) */
    WDTCSR |= (1<<WDE) | (1<<WDP2) | (1<<WDP0);
    wdt_enable_interrupt();
    
}

/* reset the watchdog timer so that it does not time out. */
void wdt_reset()
{
    __built_in_avr_wdr();
}

/* force a system restart (reset only, no interrupt) by disabling the watchdog
* interupt and waiting for a watchdog timeout.
*/
void wdt_force_restart()
{
    /* turn off WDT */
    wdt_disable_interrupt();
}
