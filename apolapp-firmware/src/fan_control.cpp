#include "fan_control.h"
namespace _10klab {
namespace fan {
#define frequency 5000
#define pwm_channel 0
#define pwm_resolution 8
#define fan_one 18
#define fan_two 5

void controlFanSpeed(int duty_cycle);

void initializeFanControl() {
  ledcSetup(pwm_channel, frequency, pwm_resolution);
  ledcAttachPin(fan_one, pwm_channel);
  ledcAttachPin(fan_two, pwm_channel);
  controlFanSpeed(0);
}

void controlFanSpeed(int duty_cycle) {
  duty_cycle = map(duty_cycle, 0, 100, 0, 255);
  if (duty_cycle < 0) {
    duty_cycle = 0;
  }
  if (duty_cycle > 255) {
    duty_cycle = 255;
  }
  Serial.println(duty_cycle);
  ledcWrite(pwm_channel, duty_cycle);
}
} // namespace fan
} // namespace _10klab