#include "util.h"
#include "process_packet.h"
#include "config.h"
/* update the configuration tcrit_hi limit with the specified value
* This function is called by the packet command parser.
*/
int update_tcrit_hi(int value)
{
  if ((value > config.hi_warn) && (value <= 0x3FF)) {
    config.hi_alarm = value;
    config_set_modified();
    return 0;
  }
  return 1;
}

/* Update the configuration twarn_hi limit with the specified value
* This function is called by the packet command parser.
*/
int update_twarn_hi(int value)
{
  if ((value > config.lo_warn) && (value < config.hi_alarm)) {
    config.hi_warn = value;
    config_set_modified();
    return 0;
  }
  return 1;
}

/* Update the configuration tcrit_lo limit with the specified value
* This function is called by the packet command parser.
*/
int update_tcrit_lo(int value)
{
  if (value < config.lo_warn) {
    config.lo_alarm = value;
    config_set_modified();
    return 0;
  }
return 1;
}

/* Update the configuration twarn_lo limit with the specified value
* This function is called by the packet command parser.
*/
int update_twarn_lo(int value)
{
  if ((value > config.lo_alarm) && (value < config.hi_warn)) {
    config.lo_warn = value;
    config_set_modified();
    return 0;
  }
  return 1;
}
