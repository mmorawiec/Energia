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

#ifndef HCSR04Module_H
#define HCSR04Module_H

#ifndef __MSP430__
#error Platform not supported
#endif

#include "Energia.h"


class HCSR04Module
{
  public:
    HCSR04Module(uint8_t pinTrig, uint8_t pinEcho);

    void setup();
    int16_t read();

  private:
    uint8_t _pinTrig;
	uint8_t _pinEcho;
};

#endif
