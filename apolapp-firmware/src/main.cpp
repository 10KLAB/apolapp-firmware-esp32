#include "BLE_mananger.h"
#include "battery_mananger.h"
#include "fan_control.h"
#include "temperature_sensor.h"
#include <Arduino.h>

// FAN2 = 5
// SENSOR_2 = 17
// FAN_1 = 18
// BAT = 4
// LED = 21
// SENSOR_1 = 22
// EN_BAT =23




void setup() {
  _10klab::fan::initializeFanControl();
  Serial.begin(115200);
  Serial.println("start");
  _10klab::BLE::initializeBLEService();
  _10klab::battery::initializeBattery();
  _10klab::temperature::initializeTempSensors();
}

void loop() {
  static int battery_level = _10klab::battery::batteryLevel();
  static int dog_temperature = _10klab::temperature::readSensorTemperature('D');
  static int fan_temperature = _10klab::temperature::readSensorTemperature('F');
  static int fan_speed = _10klab::BLE::StorageFanSpeed(false, false);
  static int prev_dog_temperature = dog_temperature;
  static int prev_fan_temperature = fan_temperature;
  static int prev_battery_level = battery_level;
  static int prev_fan_speed = fan_speed;


/////////////////////////////////////////////////////////////////////////////////////////////////
fan_speed = _10klab::BLE::StorageFanSpeed(false, false);
if((dog_temperature != prev_dog_temperature) || (fan_temperature != prev_fan_temperature) || (battery_level != prev_battery_level) || fan_speed != prev_fan_speed){
  Serial.println("data changed");
  _10klab::BLE::compactAndSendData(dog_temperature, fan_temperature, battery_level);
  prev_dog_temperature = dog_temperature;
  prev_fan_temperature = fan_temperature;
  prev_battery_level = battery_level;
  prev_fan_speed = fan_speed;
}

  /////////////////////////////////////////////// variables reading
  const int temp_read_delay = 5000;
  static unsigned long temp_timmer = millis() + temp_read_delay;
  if (millis() >= temp_timmer + temp_read_delay) {
    dog_temperature = _10klab::temperature::readSensorTemperature('D'); // Read dog temperature
    fan_temperature = _10klab::temperature::readSensorTemperature('F'); // Read fan temperature
    battery_level = _10klab::battery::batteryLevel(); // Read battery level
    // battery_level = random(20,100);
    temp_timmer = millis();
  }

/////////////////////////////////////////////////send data when phone connect

  if(_10klab::BLE::newConnection(false, false)){
    Serial.println("on sender by connection");
    _10klab::BLE::compactAndSendData(dog_temperature, fan_temperature, battery_level);
    _10klab::BLE::newConnection(false, true);
  }

  _10klab::fan::trunOffFanByLowBattery();
}
