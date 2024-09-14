#ifndef ARM_CONTROLLER_H
#define ARM_CONTROLLER_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "pins.h"

class ArmController {
private:
  Servo servo1;
  Servo servo2;
  // パルス幅の最小値
  static constexpr int servoMinUs = 500;
  // パルス幅の最大値
  static constexpr int servoMaxUs = 2400;

public:
  ArmController();
  void setup();
};

#endif