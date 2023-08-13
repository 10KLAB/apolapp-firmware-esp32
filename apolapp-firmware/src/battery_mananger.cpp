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

  const int stabilization_delay = 1000;

  delay(stabilization_delay);

  const int read_cycles = 10;
  int battery_read = 0;
  for (int i = 0; i < read_cycles; i++) {
    battery_read += analogRead(BATTERY_PIN);
    delay(10);
    // Serial.println("bat =" + String(battery_read));
  }

  digitalWrite(BATTERY_ENABLE, LOW);

  battery_read = battery_read / read_cycles;
  // Serial.println("bat prom =" + String(battery_read));
  return battery_read;
}
bool setBatteryTreshold(bool set, bool read){
  static bool treshold = true;
  if(set && !read){
    treshold = true;
  }
  if(!set && !read){
    treshold = false;
  }
  return treshold;
}

float batteryLevel() {
  const int min_read = 1565; // Minimum ADC reading for the battery level (1.6v real meassure)
  const int max_read = 4095; // Maximum ADC reading for the battery level (3.17v real meassure)

    // Read the battery level as an ADC value
    float battery_level = readBatteryADC();
    // Serial.println("bat-level" + String(battery_level));
    // Map the battery level from the ADC range to the percentage range (0-100)
    battery_level = map(battery_level, min_read, max_read, 0, 100);
    
    // Ensure the battery level is within the valid range of 0-100
    if(battery_level < 0){
        battery_level = 0;
    }
    if(battery_level > 100){
        battery_level = 100;
    }

    if(battery_level <= 1){
      setBatteryTreshold(false, false);
    }
    else{
      setBatteryTreshold(true, false);
    }

  // static int bat_test = 100;
  //   bat_test-=20;
  //   if(bat_test <= 0 ){
  //     bat_test = 100;
  //   }
        // Serial.println("battery level = " + String(bat_test) + "%");

    // Serial.println("battery level = " + String(battery_level) + "%");
    // battery_level = 10;
    return battery_level;
}


} // namespace battery
} // namespace _10klab