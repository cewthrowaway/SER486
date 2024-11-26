#include "wdt.h"
#include "log.h"
#include "led.h"
#include "config.h"
#include "uart.h"

/* Watch Dog Timer Control Register */
#define WDTCSR (*((volatile unsigned char *)0x60))
/* WDT control bits */
#define WDIE 6
#define WDP3 5
#define WDCE 4
#define WDE 3
#define WDP2 2
#define WDP1 1
#define WDP0 0

#define SREG (*((volatile unsigned char *)0x5F))

#pragma GCC push_options
#pragma GCC optimize("Os")
void wdt_disable_interrupt() 
{
    /* Save current interrupt state */
    unsigned char sreg = SREG;
    /* Disable interrupts */
    SREG &= ~(0<<7);
    
    /* Start timed sequence */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    /* Clear WDE and WDIE within 4 cycles */
    WDTCSR = 0x00;
    
}

void wdt_enable_interrupt()
{
    /* Save current interrupt state */
    unsigned char sreg = SREG;
    /* Disable interrupts */
    SREG &= ~(1<<7);
    
    /* Start timed sequence */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    /* Set WDE and WDIE within 4 cycles */
    WDTCSR = (1<<WDE) | (1<<WDIE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);

}
#pragma GCC pop_options

/* add the config stuff for the vector */
void __vector_6(void) __attribute__((signal, used, externally_visible)); 
void __vector_6() 
{
    uart_writestr("WDT interrupt\n");
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
    /* Save current interrupt state */
    unsigned char sreg = SREG;
    /* Disable interrupts */
    SREG &= ~(1<<7);

    wdt_disable_interrupt();
    wdt_reset();
    /* start timed sequence */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    
    wdt_enable_interrupt();
        /* Restore interrupt state */
    SREG = sreg;
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
    /* disable the watchdog interrupt */
    wdt_disable_interrupt();
    /* reset the watchdog timer */
    wdt_reset();

    while(1);  /* Wait for reset */
}
