#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

#define HTU21DF_I2CADDR       0x40
#define HTU21DF_READTEMP      0xE3
#define HTU21DF_READHUM       0xE5
#define HTU21DF_WRITEREG       0xE6
#define HTU21DF_READREG       0xE7
#define HTU21DF_RESET       0xFE



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
