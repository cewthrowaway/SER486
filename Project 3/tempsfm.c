#include "tempsfm.h"

#define TCRIT_HI 150
#define TWARN_HI 140
#define TWARN_LO 0
#define TCRIT_LO -40

enum TEMPFSM_STATES {
NORM_1, NORM_2, NORM_3, WARN_HI_1, WARN_HI_2, CRIT_HI, WARN_LO_1, WARN_LO_2, CRIT_LO
};

TEMPFSM_STATES tempfsm_state = NORM_1;

/* initialize the finite state machine */
void tempfsm_init()
{
  //TODO: set all of the config temperatures
  tempfsm_state = NORM_1;
}

void set_warn_led() {
led_set_blink("-");
}
void set_crit_led() {
led_set_blink(".");
}
void set_normal_led() {
led_set_blink(" ");
}

/* update the state of the temperature sensor finite state machine (provides
* hysteresis).  Sends alarms and updates the led blink based on state transitions */
void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn)
{
  switch(tempfsm_state) {
    case NORM_1:
      if (current <= lowarn) {
        alarm_send(EVENT_LO_WARN);
        set_warn_led();
        tempfsm_state = WARN_LO_1;
      }
      else if (current >= hiwarn) {
        alarm_send(EVENT_HI_WARN);
        set_warn_led();
        tempfsm_state = WARN_HI_1;
      }
    break;
    
    case NORM_2:
      /* current >= hiwarn */
      if (current >= hiwarn) {
        alarm_send(EVENT_HI_WARN);
        set_warn_led();
        tempfsm_state = WARN_HI_1;
      }
      /* current <= lowarn */
      else if (current <= lowarn) {
        tempfsm_state = WARN_LO_1;
      }
    break;
    
    case NORM_3:
      /* current <= lowarn */
      if (current <= lowarn) {
        alarm_send(EVENT_LO_WARN);
        set_warn_led();
        tempfsm_state = WARN_LO_1;
      }
      /* current >= hiwarn */
      else if (current >= hiwarn) {
        tempfsm_state = WARN_HI_1;
      }
    break;
      
    case WARN_HI_1:
      /* current >= hicrit */
        if (current >= hicrit) {
          alarm_send(EVENT_HI_ALARM);
          set_crit_led();
          tempfsm_state = CRIT_HI;
        }
      /* current < hiwarn */
      else if (current < hiwarn) {
        tempfsm_state = NORM_3;
      }
    break;
    
    case WARN_HI_2:
      /* current >= hicrit */
      if (current >= hicrit) {
        tempfsm_state = CRIT_HI;
      }
        
      /* current < hiwarn */
      else if (current < hiwarn) {
        tempfsm_state = NORM_3;
      }
    break;
    
    case CRIT_HI:
      /* current <hicrit */
      if (current < hicrit) {
        tempfsm_state = WARN_HI_2;
      }
    break;
    
    case WARN_LO_1:
      /* current <=locrit */
      if (current <= locrit) {
        alarm_send(EVENT_LO_ALARM);
        led_set_blink(".");
        tempfsm_state = CRIT_LO;
      }
      /* current > lowarn */
      else if (current > lowarn) {
        tempfsm_state = NORM_2;
      }
    break;
    
    case WARN_LO_2:
      /* current <= locrit */
      if (current <= locrit) {
        tempfsm_state = CRIT_LO;
      }
      
      /* currnt > lowarn */
      else if (current > lowarn) {
        tempfsm_state = NORM_2;
      }
    break;
    
    case CRIT_LO:
      /* current > locrit */
      if (current > locrit) {
        tempfsm_state = WARN_LO_2;
      }
    break;
    
    default:
      /* move to a valid state */
      tempfsm_state = NORM_1;
    break;
  }

}

/* reset the state machine to the initial state (normal) */
void tempfsm_reset()
{
  tempfsm_state = NORM_1;
}


