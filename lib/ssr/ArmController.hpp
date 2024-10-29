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

  // ベルトが上昇中かどうか
  bool beltIsUp = true;

  int autoCollectAngle = 0;
  TaskHandle_t autoCollectHandle;

  friend void autoCollectTask(void *pvParameters);

public:
  ArmController();

  void observeBeltState();
  void moveBelt(bool isUp);
  void stopBelt();
  void openArm();
  void closeArm();
  void swingArmToLeft();
  void swingArmToRight();

  void startAutoCollect(int angle);
  void forceStopAutoCollect();

  bool leftIsPressed = false;
  bool rightIsPressed = false;
};

#endif