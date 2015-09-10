/**
*  Example for reading DHT22 sensor using DHTSensor library.
*  Created by Michal Morawiec, 09.09.2015.
*  Released into the public domain.
*/

#include "Energia.h"
#include "DHTSensor.h"

#define PIN_DHT22_DATA P1_4

DHTSensor dht22(DHTSensor::DHT22, PIN_DHT22_DATA);


void setup()
{
  Serial.begin(9600);
  dht22.setup();
}

void loop()
{
  DHTSensor::TData temperature;
  DHTSensor::RHData humidity;

  DHTSensor::Result result = dht22.read(temperature, humidity);
  if (result == DHTSensor::SUCCESS) {
    Serial.print("Relative Humidity:\t");
    Serial.print(humidity.decimal);
    Serial.print(".");
    Serial.print(humidity.fraction);
    Serial.println(" %");

    Serial.print("Temperature:\t\t");
    Serial.print(temperature.decimal);
    Serial.print(".");
    Serial.print(temperature.fraction);
    Serial.println(" C");
  } else {
    Serial.print("DHT read error ");
    Serial.println(result, DEC);
  }

  // Min DHT22 sampling period is 2 seconds
  delay(2000);
}
