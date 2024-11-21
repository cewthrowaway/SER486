struct vpd_struct {
  char token[4];
  char model[12];
  char manufacturer[12];
  char serial_number[12];
  unsigned long manufacture_date;
  unsigned char mac_address[6];
  char county_of_origin[4];
  unsigned char checksum;
}

void vpd_init();
int vpd_is_data_valid(); 
void vpd_write_defaults();