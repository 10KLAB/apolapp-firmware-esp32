#include <Arduino.h>
#ifndef BATTERY_MANANGER_H_
#define BATTERY_MANANGER_H_

namespace _10klab{
    namespace battery{
        void initializeBattery();
        float batteryLevel();

    }
}
#endif