#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H

#include <Arduino.h>
#include "pins.h"

// 足回りのPWM設定
#define WHEEL_PWM_CHANNEL_1 6
#define WHEEL_PWM_CHANNEL_2 7
#define WHEEL_PWM_CHANNEL_3 8
#define WHEEL_PWM_FREQ 20000
#define WHEEL_PWM_RESOLUTION 8

class DriveController {
public:
  DriveController();
  void setup();
  void drive(int x, int y);
};

#endif