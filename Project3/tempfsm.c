/********************************************************
 * tempfsm.c
 *
 * this file provides function implementation for SER486
 * project 3 temperature finite state machine code.
 *
 * Author:   DaVonte Carter Vault
 * Date:     11/27/2024
 * Revision: 1.0
 */
#include "tempfsm.h"
#include "alarm.h"
#include "led.h"
#include "log.h"
#include "process_packet.h"
#define TCRIT_HI 150
#define TWARN_HI 140
#define TWARN_LO 0
#define TCRIT_LO -40

/* declare the states of the temperature finite state machine */
enum TEMPFSM_STATES {
NORM_1, NORM_2, NORM_3, WARN_HI_1, WARN_HI_2, CRIT_HI, WARN_LO_1, WARN_LO_2, CRIT_LO
};

/* declare the current state of the temperature finite state machine */
enum TEMPFSM_STATES tempfsm_state = NORM_1;

/************************************************
 * tempfsm_init
 * Description: This function initializes the temperature finite state machine
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Sets the default configuration temperatures
 *    - Sets the initial state to NORM_1
 ************************************************/
void tempfsm_init()
{
  /* set the default configuration temperatures */
  update_tcrit_hi(TCRIT_HI);
  update_twarn_hi(TWARN_HI);
  update_twarn_lo(TWARN_LO);
  update_tcrit_lo(TCRIT_LO);
  /* set the initial state */
  tempfsm_state = NORM_1;
}

/************************************************
 * set_warn_led
 * Description: This function sets the warning LED
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Sets the LED message to "-"
 ************************************************/
void set_warn_led() {
  led_set_blink("-");
}

/************************************************
 * set_crit_led
 * Description: This function sets the critical LED
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Sets the LED message to "."
 ************************************************/
void set_crit_led() {
  led_set_blink(".");
}

/************************************************
 * set_normal_led
 * Description: This function sets the normal LED
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Sets the LED message to " "
 ************************************************/
void set_normal_led() {
  led_set_blink(" ");
}

/************************************************
 * tempfsm_update
 * Description: This function updates the state of the temperature sensor finite state machine
 * 
 * Arguments: 
 *     - current - the current temperature
 *     - hicrit - the critical high temperature
 *     - hiwarn - the warning high temperature
 *     - locrit - the critical low temperature
 *     - lowarn - the warning low temperature
 * Returns: Nothing 
 * Changes:
 *    - Updates the state transitions
 ************************************************/
void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn)
{
  /* handle the state transitions */
  switch(tempfsm_state) {
    case NORM_1:
      if (current <= lowarn) {
        alarm_send(EVENT_LO_WARN);
        set_warn_led();
        log_add_record(EVENT_LO_WARN);
        tempfsm_state = WARN_LO_1;
      }
      else if (current >= hiwarn) {
        alarm_send(EVENT_HI_WARN);
        set_warn_led();
        log_add_record(EVENT_HI_WARN);
        tempfsm_state = WARN_HI_1;
      }
    break;
    
    case NORM_2:
      /* current >= hiwarn */
      if (current >= hiwarn) {
        alarm_send(EVENT_HI_WARN);
        set_warn_led();
        log_add_record(EVENT_HI_WARN);
        tempfsm_state = WARN_HI_1;
      }
      /* current <= lowarn */
      else if (current <= lowarn) {
        set_warn_led();
        tempfsm_state = WARN_LO_1;
      }
    break;
    
    case NORM_3:
      /* current <= lowarn */
      if (current <= lowarn) {
        alarm_send(EVENT_LO_WARN);
        set_warn_led();
        log_add_record(EVENT_LO_WARN);
        tempfsm_state = WARN_LO_1;
      }
      /* current >= hiwarn */
      else if (current >= hiwarn) {
        set_warn_led();
        tempfsm_state = WARN_HI_1;
      }
    break;
      
    case WARN_HI_1:
      /* current >= hicrit */
        if (current >= hicrit) {
          alarm_send(EVENT_HI_ALARM);
          set_crit_led();
          log_add_record(EVENT_HI_ALARM);
          tempfsm_state = CRIT_HI;
        }
      /* current < hiwarn */
      else if (current < hiwarn) {
        set_normal_led();
        tempfsm_state = NORM_3;
      }
    break;
    
    case WARN_HI_2:
      /* current >= hicrit */
      if (current >= hicrit) {
        set_crit_led();
        tempfsm_state = CRIT_HI;
      }
        
      /* current < hiwarn */
      else if (current < hiwarn) {
        set_normal_led();
        tempfsm_state = NORM_3;
      }
    break;
    
    case CRIT_HI:
      /* current <hicrit */
      if (current < hicrit) {
        set_warn_led();
        tempfsm_state = WARN_HI_2;
      }
    break;
    
    case WARN_LO_1:
      /* current <=locrit */
      if (current <= locrit) {
        alarm_send(EVENT_LO_ALARM);
        set_crit_led();
        tempfsm_state = CRIT_LO;
        log_add_record(EVENT_LO_ALARM);
      }
      /* current > lowarn */
      else if (current > lowarn) {
        set_normal_led();
        tempfsm_state = NORM_2;
      }
    break;
    
    case WARN_LO_2:
      /* current <= locrit */
      if (current <= locrit) {
        set_crit_led();
        tempfsm_state = CRIT_LO;
      }
      
      /* currnt > lowarn */
      else if (current > lowarn) {
        set_normal_led();
        tempfsm_state = NORM_2;
      }
    break;
    
    case CRIT_LO:
      /* current > locrit */
      if (current > locrit) {
        set_warn_led();
        tempfsm_state = WARN_LO_2;
      }
    break;
    
    default:
      /* move to a valid state */
      set_normal_led();
      tempfsm_state = NORM_1;
    break;
  }

}

/************************************************
 * tempfsm_reset
 * Description: This function resets the temperature finite state machine to the initial state
 * 
 * Arguments: None.
 * Returns: Nothing 
 * Changes:
 *    - Resets the state machine to NORM_1
 ************************************************/
void tempfsm_reset()
{
  tempfsm_state = NORM_1;
}


