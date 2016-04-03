#include "Htu21d.h"
#if defined(__AVR__)
#include <util/delay.h>
#endif


void sendCommand(byte command){
  Wire.beginTransmission(HTU21D_I2CADDR);
  Wire.write(command);
  Wire.endTransmission();
}

uint16_t readWordFromWire(){
  uint16_t t = Wire.read();
  t <<= 8;
  t |= Wire.read();
  return t;
}

boolean waitUntilAvailable(int watchDogMillis){
  if(watchDogMillis < 0){
    while (!Wire.available()) {}  // Wait, forever
  } else {
    unsigned long endMillis = millis() + watchDogMillis;
    while (!Wire.available() && millis() < endMillis) {}  // Wait a bit
  }
}

boolean waitUntilAvailable(){
  return waitUntilAvailable(-1);
}

uint16_t requestRawValue(byte command, byte delayMs){
  sendCommand(command);
  delay(delayMs); // add delay between request and actual read!
  Wire.requestFrom(HTU21D_I2CADDR, 3);
  if(waitUntilAvailable(HTU21D_WATCHDOG_DELAY)){
    uint16_t rawValue = readWordFromWire();
    uint8_t crc = Wire.read();
    return rawValue;
  } else {
    return HTU21D_ERROR_VALUE_RAW;
  };
}

float convert(uint16_t input, float shifter, float multer, float diver){
  if(input == HTU21D_ERROR_VALUE_RAW){
    return HTU21D_ERROR_VALUE;
  }
  float output = input;
  output *= multer;
  output /= diver;
  output += shifter;
  return output;
}

float convertToCelsius(uint16_t rawTemp){
  return convert(rawTemp,-48.85, 175.72, 65536);;
}

float convertToRelativeHumidity(uint16_t rawRh){
  return convert(rawRh, -6, 125, 65536);;
}

Htu21d::Htu21d() {
}

boolean Htu21d::begin(void) {
  Wire.begin();
  reset();
  sendCommand(HTU21D_READREG);
  Wire.requestFrom(HTU21D_I2CADDR, 1);
  return (Wire.read() == 0x2); // after reset should be 0x2
}

void Htu21d::reset(void) {
  sendCommand(HTU21D_RESET);
  delay(HTU21D_RESET_DELAY);
}

float Htu21d::readTemperature(void) {
  return convertToCelsius(
    requestRawValue(HTU21D_READTEMP, HTU21D_READTEMP_DELAY)
  );
}

float Htu21d::readHumidity(void) {
  return convertToRelativeHumidity(
    requestRawValue(HTU21D_READHUM, HTU21D_READHUM_DELAY)
  );
}
