#include <Arduino.h>
#ifndef BLE_MANANGER_H_
#define BLE_MANANGER_H_

namespace _10klab{
    namespace BLE{
        void initializeBLEService();
        void sendMessage();
        String readDeviceName();
        String compactData(float dog_sensor_temp, float fan_sensor_temp, float battery_level);
    }
}





#endif