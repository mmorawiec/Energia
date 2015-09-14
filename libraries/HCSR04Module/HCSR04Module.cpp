/**
*  HCSR04Module library.
*
*  Library for reading HC-SR04 ultrasonic ranging module.
*  Created by Michal Morawiec, 11.09.2015.
*  Released into the public domain.
*
*  Based on the ITead studio Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module:
*  - http://wiki.iteadstudio.com/Ultrasonic_Ranging_Module_HC-SR04
*/

#include "HCSR04Module.h"

//#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__);
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__);
#else
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
#endif


HCSR04Module::HCSR04Module(uint8_t pinTrig, uint8_t pinEcho)
: _pinTrig(pinTrig), _pinEcho(pinEcho)
{
}

void HCSR04Module::setup()
{
  pinMode(_pinTrig, OUTPUT);
  digitalWrite(_pinTrig, LOW);
  pinMode(_pinEcho, INPUT);
}

uint16_t HCSR04Module::readTime()
{
  uint16_t pulsetime;

  // Trigger signal: high 10us
  digitalWrite(_pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pinTrig, LOW);

  noInterrupts();

  // Echo signal: high pulse
  pulsetime = pulseIn(_pinEcho, HIGH, 50000);

  interrupts();

  DEBUG_PRINT("Echo signal time: ")
  DEBUG_PRINT(pulsetime, DEC)
  DEBUG_PRINTLN(" us")

  return pulsetime;
}

int16_t HCSR04Module::readDistance(Unit unit)
{
  uint16_t time = readTime();

  // Echo pulse time
  // Valid distance: 2cm (116us) - 500cm (29ms)
  // No obstacle: 0
  // Invalid reading: -1
  if (time > 29000) {
    return 0;
  } else if (time >= 116) {
    if (unit == HCSR04Module::CM)
      return time / 58;
    else if (unit == HCSR04Module::INCH)
      return time / 148;
  }

  return -1;
}
