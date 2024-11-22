/********************************************************
 * vpd.h
 *
 * This file defines the structure and declarations for 
 * managing the Vital Product data. 
 *
 * Author:   DaVonte Carter Vault
 * Date:     Fall 2024
 * Revision: 1.0
 *
 ********************************************************/

#ifndef VPD_H
#define VPD_H
struct vpd_struct {
  char token[4];
  char model[12];
  char manufacturer[12];
  char serial_number[12];
  unsigned long manufacture_date;
  unsigned char mac_address[6];
  char county_of_origin[4];
  unsigned char checksum;
};

struct vpd_struct vpd;


void vpd_init();
int vpd_is_data_valid(); 
void vpd_write_defaults();

#endif