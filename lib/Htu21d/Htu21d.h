#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

#define HTU21D_I2CADDR         0x40
#define HTU21D_READTEMP        0xE3
#define HTU21D_READTEMP_DELAY  60
#define HTU21D_READHUM         0xE5
#define HTU21D_READHUM_DELAY   20
#define HTU21D_WRITEREG        0xE6
#define HTU21D_READREG         0xE7
#define HTU21D_RESET           0xFE
#define HTU21D_RESET_DELAY     15
#define HTU21D_ERROR_VALUE_RAW 0xFFFF
#define HTU21D_ERROR_VALUE     0.f
#define HTU21D_WATCHDOG_DELAY  100



class Htu21d {
 public:
  Htu21d();
  boolean begin(void);
  float readTemperature(void);
  float readHumidity(void);
  void reset(void);
 private:
  boolean readData(void);
  float humidity, temp;
};
