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
  Serial.begin(115200);
  Serial.println("start");
  _10klab::BLE::initializeBLEService();
  _10klab::battery::initializeBattery();
  _10klab::temperature::initializeTempSensors();
  _10klab::fan::initializeFanControl();
}

void loop() {
  static float battery_level = _10klab::battery::batteryLevel();
  static float dog_temperature =
      _10klab::temperature::readSensorTemperature('D');
  static float fan_temperature =
      _10klab::temperature::readSensorTemperature('F');

  const int sender_delay = 5000; // Time delay between data sending
  static unsigned long sender_timer = 0; // Timer to track data sending
  // Check if the specified time has elapsed to send data
  if (millis() > sender_timer + sender_delay) {
    Serial.println("------------Temps-------------");
    dog_temperature = _10klab::temperature::readSensorTemperature('D'); // Read dog temperature
    Serial.println("Dog temperature = " + String(dog_temperature));
    fan_temperature = _10klab::temperature::readSensorTemperature('F'); // Read fan temperature
    Serial.println("Fan temperature = " + String(fan_temperature));
    _10klab::BLE::compactAndSendData(dog_temperature, fan_temperature,
                                     battery_level); // Send data via BLE
    
    sender_timer = millis();
  }

  const int battery_read_delay = 10000; // Time delay between battery level measurements
  static unsigned long battery_measurement_timer = 0; // Timer to track battery level measurements
  // Check if the specified time has elapsed to measure the battery level
  if (millis() > battery_measurement_timer + battery_read_delay) {
    Serial.println("------------Bat-------------");
    battery_level = _10klab::battery::batteryLevel(); // Read battery level
    battery_measurement_timer = millis();
  }
  
}
