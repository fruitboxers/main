#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <Arduino.h>
#include <ps5Controller.h>
#include "pins.h"

class InputController {
private:
  int roundFraction(int value);

public:
  InputController();
  void setup();

  int roundedLStickX();
  int roundedLStickY();
  int roundedRStickX();
  int roundedRStickY();
};

#endif