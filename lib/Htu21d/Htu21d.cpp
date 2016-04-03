#include "Htu21d.h"
#if defined(__AVR__)
#include <util/delay.h>
#endif

const float ERROR_VALUE = 0.0;
Htu21d::Htu21d() {
}


boolean Htu21d::begin(void) {
  Wire.begin();

  reset();

  Wire.beginTransmission(HTU21DF_I2CADDR);
  Wire.write(HTU21DF_READREG);
  Wire.endTransmission();
  Wire.requestFrom(HTU21DF_I2CADDR, 1);
  return (Wire.read() == 0x2); // after reset should be 0x2
}

void Htu21d::reset(void) {
  Wire.beginTransmission(HTU21DF_I2CADDR);
  Wire.write(HTU21DF_RESET);
  Wire.endTransmission();
  delay(15);
}

boolean waitUntilAvailable(int watchDogMillis){
  if(watchDogMillis < 0){
    while (!Wire.available()) {}
  } else {
    unsigned long endMillis = millis() + watchDogMillis;
    while (!Wire.available() && millis() < endMillis) {}//Wait
  }
}

boolean waitUntilAvailable(){
  return waitUntilAvailable(-1);
}

float Htu21d::readTemperature(void) {
  Wire.beginTransmission(HTU21DF_I2CADDR);
  Wire.write(HTU21DF_READTEMP);
  Wire.endTransmission();
  delay(50); // add delay between request and actual read!
  Wire.requestFrom(HTU21DF_I2CADDR, 3);
  if(!waitUntilAvailable(1000)){
    return ERROR_VALUE;
  };
  uint16_t t = Wire.read();
  t <<= 8;
  t |= Wire.read();

  uint8_t crc = Wire.read();

  float temp = t;
  temp *= 175.72;
  temp /= 65536;
  temp -= 46.85;

  return temp;
}


float Htu21d::readHumidity(void) {
  // OK lets ready!
  Wire.beginTransmission(HTU21DF_I2CADDR);
  Wire.write(HTU21DF_READHUM);
  Wire.endTransmission();

  delay(50); // add delay between request and actual read!

  Wire.requestFrom(HTU21DF_I2CADDR, 3);
  if(!waitUntilAvailable(1000)){
    return ERROR_VALUE;
  };

  uint16_t h = Wire.read();
  h <<= 8;
  h |= Wire.read();

  uint8_t crc = Wire.read();

  float hum = h;
  hum *= 125;
  hum /= 65536;
  hum -= 6;

  return hum;
}
