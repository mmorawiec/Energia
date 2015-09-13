/**
*  DHTSensor library.
*
*  Library for reading DHT/AM temperature and humidity sensor family.
*  Created by Michal Morawiec, 09.09.2015.
*  Released into the public domain.
*
*  Based on the Adafruit DHT library (MIT license):
*  - https://learn.adafruit.com/dht
*  - https://github.com/adafruit/DHT-sensor-library
*
*  Based on https://github.com/MORA99/Stokerbot/tree/master/Libraries/dht (Released into the public domain - http://unlicense.org)
*/

#include "DHTSensor.h"

//#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__);
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__);
#else
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
#endif


DHTSensor::DHTSensor(Model model, uint8_t pin)
: _model(model), _pin(pin)
{
}

void DHTSensor::setup()
{
  pinMode(_pin, INPUT);
}

DHTSensor::Result DHTSensor::read(Data& data)
{
  int8_t byteno, bitno, pulsetime;

  data.raw[0] = data.raw[1] = data.raw[2] = data.raw[3] = data.raw[4] = 0;

  // Start signal:
  // - preceding high
  // - low DHT11 >=18ms, DHT22>=500us
  // - high (via pullup)
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  _model == DHT11 ? delay(18) : delayMicroseconds(500);
  pinMode(_pin, INPUT);

  // Response signal:
  // - preceding high 20-40us
  // - low 80us
  // - high 80us
  // - low
  if (pulseIn(_pin, HIGH, 40+80+80) == 0)
    return DHTSensor::ERROR_RESPONSE_SIGNAL;

  noInterrupts();

  for (byteno = 0; byteno < sizeof(data.raw); byteno++) {
    for (bitno = 7; bitno >= 0; bitno--) {
      // Bit signal:
      // - low 50us
      // - high bit 0: 26-28us, bit 1: 70us
      // - low
      pulsetime = pulseIn(_pin, HIGH, 50+70);
      if (pulsetime > 28) {
        bitSet(data.raw[byteno], bitno);
      } else if (pulsetime == 0) {
        interrupts();
        return DHTSensor::ERROR_BIT_SIGNAL;
      }
    }
  }

  interrupts();

  DEBUG_PRINT("Raw data: ")
  for (byteno = 0; byteno < sizeof(data.raw); byteno++) {
    DEBUG_PRINT(data.raw[byteno], HEX)
    DEBUG_PRINT(" ")
  }
  DEBUG_PRINTLN()

  // Checksum is the LSB of the sum of all data bytes
  return (uint8_t)(data.raw[0] + data.raw[1] + data.raw[2] + data.raw[3]) == data.checksum ?
          DHTSensor::SUCCESS : DHTSensor::ERROR_CHECKSUM;
}

DHTSensor::Result DHTSensor::read(TData& temp, RHData& rh)
{
  Data data;

  Result result = read(data);
  if (result == DHTSensor::SUCCESS) {
    if (_model == DHT11) {
      // Humidity = 1st byte
      rh.decimal = data.rh_high;
      rh.fraction = 0;

      // Temperature = 3rd byte
      temp.decimal = data.temp_high;
      temp.fraction = 0;
    } else {
      // Humidity x10 = 1st byte * 256 + 2nd byte
      uint16_t value = data.rh_high << 8 | data.rh_low;
      rh.decimal = value / 10;
      rh.fraction = value % 10;

      // Temperature x10 = 3rd byte * 256 + 4th byte; sign is most significant bit
      value = data.temp_high << 8 | data.temp_low;
      temp.decimal = (value & 0x7fff) / 10;
      if (value & 0x8000)
        temp.decimal *= -1;
      temp.fraction = (value & 0x7fff) % 10;
    }
  }

  return result;
}
