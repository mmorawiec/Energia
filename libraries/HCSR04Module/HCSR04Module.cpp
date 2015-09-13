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

int16_t HCSR04Module::read()
{
  uint16_t pulsetime;

  // Trigger signal: high 10us
  digitalWrite(_pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pinTrig, LOW);

  noInterrupts();

  // Echo signal: high pulse
  // - 116us: 2cm
  // - 29ms: 500cm
  pulsetime = pulseIn(_pinEcho, HIGH, 50000);

  interrupts();

  DEBUG_PRINT("Echo signal time: ")
  DEBUG_PRINT(pulsetime, DEC)
  DEBUG_PRINTLN(" us")

  if (pulsetime > 29000) {
      // No obstacle
      return 0;
  }
  else if (pulsetime >= 116) {
    // Valid distance
    return pulsetime / 58;
  }

  return -1;
}
