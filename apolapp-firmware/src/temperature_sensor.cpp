#include <DallasTemperature.h>
#include <OneWire.h>

namespace _10klab {
namespace temperature {
#define DOG_SENSOR_PIN 22
#define FAN_SENSOR_PIN 17
OneWire oneWireDog(DOG_SENSOR_PIN);
OneWire oneWireFan(FAN_SENSOR_PIN);

DallasTemperature dogSensor(&oneWireDog);
DallasTemperature fanSensor(&oneWireFan);

void initializeTempSensors() {
  dogSensor.begin();
  fanSensor.begin();
}

float readSensorTemperature(char sensor) {
  const int read_cycles = 2;
  const int read_delay = 100;
  float temperature = 0;

  for (int i = 0; i < read_cycles; i++) {
    switch (sensor) {
        case 'D': 
            dogSensor.requestTemperatures();
            temperature += dogSensor.getTempCByIndex(0);
        break;

        case 'F': 
            fanSensor.requestTemperatures();
            temperature += fanSensor.getTempCByIndex(0);
        break;
        
        default:
            return -1;
        }
        delay(read_delay);
  }
  temperature = temperature / read_cycles;
  // temperature = 20.5; // just for debugg
  return temperature;
}
} // namespace temperature
} // namespace _10klab