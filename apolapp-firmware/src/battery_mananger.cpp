#include "battery_mananger.h"
namespace _10klab {
namespace battery {
#define BATTERY_PIN 4
#define BATTERY_ENABLE 23

void initializeBattery() {
  pinMode(BATTERY_PIN, INPUT);
  pinMode(BATTERY_ENABLE, OUTPUT);
  digitalWrite(BATTERY_ENABLE, LOW);
}

int readBatteryADC() {
  digitalWrite(BATTERY_ENABLE, HIGH);

  const int stabilization_delay = 500;

  delay(stabilization_delay);

  const int read_cycles = 10;
  int battery_read = 0;
  for (int i = 0; i < read_cycles; i++) {
    battery_read += analogRead(BATTERY_PIN);
  }

  digitalWrite(BATTERY_ENABLE, LOW);

  battery_read = battery_read / read_cycles;
  return battery_read;
}

float batteryLevel() {
  const int min_read = 960;
  const int max_read = 2800;

    float battery_level = readBatteryADC();
    battery_level = map(battery_level, min_read, max_read, 0, 100);
    
    if(battery_level < 0){
        battery_level = 0;
    }
    if(battery_level > 100){
        battery_level = 100;
    }

    Serial.println("battery level = " + String(battery_level) + "%");
    return battery_level;
}

} // namespace battery
} // namespace _10klab