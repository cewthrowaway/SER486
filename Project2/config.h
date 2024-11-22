/********************************************************
 * config.h
 *
 * This file defines the functions for managing the 
 * configuration data in the project. It stores data config
 * data in EEPROM and locally in config.
 *
 *
 * Author:   DaVonte Carter Vault
 * Date:     Fall 2024
 * Revision: 1.0
 *
 ********************************************************/
#if !defined(CONFIG_H)
#define CONFIG_H

struct config_struct {
  char token[4];
  unsigned int hi_alarm;
  unsigned int hi_warn;
  unsigned int lo_alarm;
  unsigned int lo_warn;
  char use_static_ip;
  unsigned char static_ip[4];
  unsigned char checksum;
};

/****** public variables *******/
/* create the config */
struct config_struct config;

/****** public methods *******/
void config_init();
void config_update();
void config_set_modified();


#endif // CONFIG_H


