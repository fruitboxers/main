#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H

#include <Arduino.h>
#include "pins.h"

class DriveController {
private:
  static constexpr int pwmChannel1 = 6;
  static constexpr int pwmChannel2 = 7;
  static constexpr int pwmChannel3 = 8;
  static constexpr int pwmFreq = 20000;
  static constexpr int pwmResolution = 8;

public:
  DriveController();
  void setup();
  void drive(int x, int y);
};

#endif