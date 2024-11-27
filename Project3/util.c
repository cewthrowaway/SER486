/********************************************************
 * util.c
 *
 * this file provides function implementation for SER486
 * project 3 utility functions.
 *
 * Author:   DaVonte Carter Vault
 * Date:     11/27/2024
 * Revision: 1.0
 */
#include "util.h"
#include "process_packet.h"
#include "config.h"
/************************************************
 * update_tcrit_hi
 * Description: This function updates the tcrit_hi limit
 * 
 * Arguments: 
 *     - value - the new tcrit_hi limit
 * Returns: 
 *     - 0 if successful
 *     - 1 if unsuccessful
 * Changes:
 *    - Updates the tcrit_hi limit
 *    - Sets the config modified flag
 ************************************************/
int update_tcrit_hi(int value)
{
  if ((value > config.hi_warn) && (value <= 0x3FF)) {
    config.hi_alarm = value;
    config_set_modified();
    return 0;
  }
  return 1;
}

/************************************************
 * update_twarn_hi
 * Description: This function updates the twarn_hi limit
 * 
 * Arguments: 
 *     - value - the new twarn_hi limit
 * Returns: 
 *     - 0 if successful
 *     - 1 if unsuccessful
 * Changes:
 *    - Updates the twarn_hi limit
 *    - Sets the config modified flag
 ************************************************/
int update_twarn_hi(int value)
{
  if ((value > config.lo_warn) && (value < config.hi_alarm)) {
    config.hi_warn = value;
    config_set_modified();
    return 0;
  }
  return 1;
}

/************************************************
 * update_tcrit_lo
 * Description: This function updates the tcrit_lo limit
 * 
 * Arguments: 
 *    - value - the new tcrit_lo limit
 * Returns: 
 *    - 0 if successful
 *    - 1 if unsuccessful
 * Changes:
 *    - Updates the tcrit_lo limit
 *    - Sets the config modified flag
 ************************************************/
int update_tcrit_lo(int value)
{
  if (value < config.lo_warn) {
    config.lo_alarm = value;
    config_set_modified();
    return 0;
  }
return 1;
}

/************************************************
 * update_twarn_lo
 * Description: This function updates the twarn_lo limit
 * 
 * Arguments: 
 *     - value - the new twarn_lo limit
 * Returns: 
 *     - 0 if successful
 *     - 1 if unsuccessful
 * Changes:
 *    - Updates the twarn_lo limit
 *    - Sets the config modified flag
 ************************************************/
int update_twarn_lo(int value)
{
  if ((value > config.lo_alarm) && (value < config.hi_warn)) {
    config.lo_warn = value;
    config_set_modified();
    return 0;
  }
  return 1;
}
