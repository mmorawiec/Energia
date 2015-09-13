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

#ifndef DHTSensor_H
#define DHTSensor_H

#ifndef __MSP430__
#error Platform not supported
#endif

#include "Energia.h"


class DHTSensor
{
  public:
    enum Model {
      DHT11,
      DHT22,
      AM2302 = DHT22
    };

    enum Result {
      SUCCESS = 0,
      ERROR_RESPONSE_SIGNAL = -1,
      ERROR_BIT_SIGNAL = -2,
      ERROR_CHECKSUM = -3
    };

    union Data {
      uint8_t raw[5];
      struct {
        uint8_t rh_high;
        uint8_t rh_low;
        uint8_t temp_high;
        uint8_t temp_low;
        uint8_t checksum;
      };
    };

    struct TData {
      int8_t  decimal;
      uint8_t fraction;  
    };
    struct RHData {
      uint8_t decimal;
      uint8_t fraction;  
    };


    DHTSensor(Model model, uint8_t pin);

    void setup();

    Result read(Data& data);
    Result read(TData& temp, RHData& rh);

  private:
    Model _model;
    uint8_t _pin;
};

#endif
