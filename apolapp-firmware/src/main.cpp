#include <Arduino.h>
#include "BLE_mananger.h"
#include "fan_control.h"


void setup()
{
  Serial.begin(115200);
  Serial.println("start");
  _10klab::BLE::initializeBLEService();
  Serial.println("stop");


}

void loop()
{

  delay(1000);
}


