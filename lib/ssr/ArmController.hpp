#ifndef ARM_CONTROLLER_H
#define ARM_CONTROLLER_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "pins.hpp"

class ArmController {
private:
  Servo servo1;
  Servo servo2;
  // パルス幅の最小値
  static constexpr int servoMinUs = 500;
  // パルス幅の最大値
  static constexpr int servoMaxUs = 2400;

  int armSwingAngle = 90;

  bool beltIsUp = true;

  bool isAutoCollecting = false;
  int autoCollectAngle = 0;

public:
  ArmController();
  void setup();
  void loop();

  void moveBelt(bool isUp);
  void stopBelt();
  void openArm();
  void closeArm();
  void swingArmToLeft();
  void swingArmToRight();
  void resetArmSwing();

  void startAutoCollect(int angle);
  void forceStopAutoCollect();

  bool leftIsPressed = false;
  bool rightIsPressed = false;
};

#endif