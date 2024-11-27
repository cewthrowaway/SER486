/********************************************************
 * wdt.c
 *
 * this file provides function implementation for SER486
 * project 3 watchdog timer code.
 *
 * Author:   DaVonte Carter Vault
 * Date:     11/27/2024
 * Revision: 1.0
 */
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
/* Global Interrupt Enable Register */
#define SREG (*((volatile unsigned char *)0x5F))
#define I_BIT 7

#pragma GCC push_options
#pragma GCC optimize("Os")
/************************************************
 * wdt_disable_interrupt
 * Description: This function disables the watchdog timer interrupt
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Disables the watchdog timer interrupt
 ************************************************/
void wdt_disable_interrupt() 
{
    /* Start timed sequence */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    /* Clear WDE and WDIE within 4 cycles */
    WDTCSR = 0x00;
    
}
/************************************************
 * wdt_enable_interrupt
 * Description: This function enables the watchdog timer interrupt
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Enables the watchdog timer interrupt
 ************************************************/
void wdt_enable_interrupt()
{
    /* Start timed sequence */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    /* Set WDE and WDIE within 4 cycles */
    WDTCSR = (1<<WDE) | (1<<WDIE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
    

}
#pragma GCC pop_options

/************************************************
 * __vector_    6
 * Interrupt Service Routine (ISR) for watchdog timer operations.
 * Description: This ISR is triggered when the watchdog timer times out.
 *              It turns on the status led, writes the event to the log,
 *              and then restarts the system.
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Turns on the status led
 *    - Writes the event to the log
 *    - Restarts the system
 ************************************************/
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

/************************************************
 * wdt_init
 * Description: This function initializes the watchdog timer
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Temporarily Disables Global interrupts
 *    - Sets the watchdog timer for a 2 second timeout and interrupt+reset mode
 *    - Enables the watchdog timer interrupt
 ************************************************/
void wdt_init() 
{
    /* Save current interrupt state */
    unsigned char sreg = SREG;
    /* Disable interrupts */
    SREG &= ~(1<<I_BIT);

    wdt_disable_interrupt();
    wdt_reset();
    /* start timed sequence */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    /* adjust timeout value = 256k cycles (2.0 s) */
    WDTCSR = (1<<WDE) | (1<<WDIE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
    wdt_enable_interrupt();

    /* Restore interrupt state */
    SREG = sreg;
}

/************************************************
 * wdt_reset
 * Description: This function resets the watchdog timer
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Resets the watchdog timer
 ************************************************/
void wdt_reset()
{
    __builtin_avr_wdr();
}

/************************************************
 * wdt_force_restart
 * Description: This function forces a restart of the system
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Disables the watchdog interrupt
 *    - Resets the watchdog timer
 *    - Calls ISR and saves log and config
 *    - Waits for a watchdog timeout
 ************************************************/
void wdt_force_restart()
{
    /* disable the watchdog interrupt */
    WDTCSR |= (1<<WDCE);
    /* reset the watchdog timer */
    wdt_reset();

    while(1);  /* Wait for reset */
}
