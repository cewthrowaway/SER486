#include "wdt.h"
#include "log.h"
#include "led.h"
#include "config.h"

/* Watch Dog Timer Control Register */
#define WDTCSR (*((volatile unsigned char *)0x60))
/* WDT control bits */
#define WDIE 6
#define WDP3 5
#define WDCE 4
#define WDE 3
#define WDP2 2
#define WDP0 0

#pragma GCC push_options
#pragma GCC optimize("Os")
void wdt_disable_interrupt() 
{
    /* clear WDE and WDIE to disable interrupts */
    WDTCSR &= ~(1<<WDE);
    WDTCSR &= ~(1<<WDIE);
}

void wdt_enable_interrupt()
{
    /* Interrupt, then go to System Reset Mode */
    WDTCSR |= (1<<WDE) | (1<<WDIE);
}
#pragma GCC pop_options

/* add the config stuff for the vector */
void __vector_6(void) __attribute__((signal, used, externally_visible)); 
void __vector_6() 
{
    /* turn on the status led */
    led_on();
    /* write the event to log */
    log_add_record(EVENT_WDT);
    /* try to save the log before restart */
    log_update_noisr();
    /* try to save the config before restart */
    config_update_noisr();
}

/* initialize the watchdog timer for a 2 second timeout and interrupt+reset mode */
void wdt_init() 
{
    wdt_disable_interrupt();
    wdt_reset();
    /* start timed sequence */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    /* adjust timeout value = 256k cycles (0.5 s) */
    WDTCSR &= ~(1<<WDP3); /* clear the WDP3 bit */
    WDTCSR |= (1<<WDP2) | (1<<WDP3) | (1<<WDP0);
    wdt_enable_interrupt();
    
}

/* reset the watchdog timer so that it does not time out. */
void wdt_reset()
{
    __builtin_avr_wdr();
}

/* force a system restart (reset only, no interrupt) by disabling the watchdog
* interupt and waiting for a watchdog timeout.
*/
void wdt_force_restart()
{
    /* turn off WDT */
    wdt_disable_interrupt();
}
