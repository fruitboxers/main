#ifndef ARM_CONTROLLER_H
#define ARM_CONTROLLER_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "pins.h"

// サーボのパルス幅
#define servoMinUs 500
#define servoMaxUs 2400

class ArmController {
public:
  ArmController();
  void setup();
};

#endif