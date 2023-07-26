#include <Arduino.h>
#ifndef FAN_CONTROL_H_
#define FAN_CONTROL_H_

namespace _10klab{
    namespace fan{
        void initializeFanControl();
        void controlFanSpeed(int duty_cycle);

    }
}

#endif