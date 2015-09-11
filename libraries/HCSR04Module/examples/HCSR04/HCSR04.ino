/**
*  Example for reading HC-SR04 ultrasonic ranging module using HCSR04Module library.
*  Created by Michal Morawiec, 11.09.2015.
*  Released into the public domain.
*/

#include "Energia.h"
#include "HCSR04Module.h"

#define PIN_HCSR04_TRIG P2_0
#define PIN_HCSR04_ECHO P2_1

HCSR04Module hcsr04(PIN_HCSR04_TRIG, PIN_HCSR04_ECHO);


void setup()
{
  Serial.begin(9600);
  hcsr04.setup();
}

void loop()
{
  int16_t distance = hcsr04.read();
  if (distance > 0) {
    Serial.print("Distance:\t");
    Serial.print(distance);
    Serial.println(" cm");
  } else if (distance == 0) {
    Serial.println("Distance:\tout of range");
  } else {
    Serial.println("HC-SR04 read error ");
  }

  // Reading period should be no less than 50ms
  delay(1000);
}
