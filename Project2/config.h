

struct config_struct {
  char token[4];
  unsigned int hi_alarm;
  unsigned int hi_warn;
  unsigned int lo_alarm;
  unsigned int lo_warn;
  char use_static_ip;
  unsigned char[4] static_ip;
  unsigned char checksum;
}

/****** public variables *******/
/* create the config */
struct config_struct config;

/****** public methods *******/
void config_init();
void config_update();
void config_set_modified();
