struct Settings {
  bool debug = false;
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
    double calibration = 1.0;
  } capacity;

  struct {
    unsigned short liters = 0;
    unsigned long date = 0;
  } refueling;

  struct {
    double liters = 0;
    double m3ratio = 0.24;
  } counter;

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
    double liters = 0;
    double m3 = 0;
  } counter;

  struct {
    bool required = false;
    bool moisture = false;
    int8_t rssi = 0;
  } service;
} vars;

float rsCalibrationMap[4][22] = {
  {4, 9, 68,  135, 176,  210, 259, 329, 367, 412, 454, 496, 552, 609, 680, 765, 816, 874, 916, 955, 1011, 1020},
  {0, 5, 7.5, 10,  12.5, 15,  20,  30,  35,  40,  45,  50,  55,  60,  65,  70,  75,  80,  85,  90,  95,   96},

  {81, 180, 1714, 3166, 4125, 4888, 6032, 7687, 8548, 9448, 10305, 11334, 12568, 13841, 15468, 17300, 18559, 19620, 20590, 21601, 22798, 23933},
  {0,  5,   7.5,  10,   12.5, 15,   20,   30,   35,   40,   45,    50,    55,    60,    65,    70,    75,    80,    85,    90,    95,    96}
};

char buffer[120];