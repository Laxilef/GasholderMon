struct Settings {
  bool debug = true;
  char hostname[80] = "gasholdermon";

  struct {
    char server[80];
    int port = 1883;
    char user[32];
    char password[32];
    char prefix[80] = "gasholder";
    unsigned int interval = 5000;
  } mqtt;

  struct {
    unsigned short liters = 4950;
    short calibration = 0;
  } capacity;

  struct {
    unsigned short liters = 0;
    unsigned long date = 0;
  } refueling;

  struct {
    unsigned short interval = 180;
    unsigned long date = 0;
  } service;
} settings;

struct Variables {
  struct {
    bool ready = false;
    double liters = 0;
    double percentage = 0;
  } remaining;

  struct {
    bool process = false;
  } refueling;

  struct {
    bool required = false;
  } service;
} vars;