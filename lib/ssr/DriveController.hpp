#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "pins.hpp"

struct Vector2 {
  int x;
  int y;
};

enum AutoDriveStage { OFF, FORWARD, RIGHT, BACKWARD };

struct AutoDriveStageInfo {
  Vector2 vector;
  long duration;
};

class DriveController {
private:
  static constexpr int pwmChannel1 = 6;
  static constexpr int pwmChannel2 = 7;
  static constexpr int pwmChannel3 = 8;
  static constexpr int pwmFreq = 20000;
  static constexpr int pwmResolution = 8;

  double targetAngle = 0;

  Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);

  double getAngle();

    // 自律制御の段階
  AutoDriveStage autoDriveStage = OFF;

  long currentAutoDriveStartTime = 0;

  AutoDriveStageInfo getAutoDriveInfo();

public:
  DriveController();
  
  // 引数として受け取ったx,y方向の入力と9軸センサーの値を元にモーターを回す関数
  void drive(Vector2 vector);
  void changeTargetAngle(double range);

  void startAutoDrive();
  void forceStopAutoDrive();
};

#endif