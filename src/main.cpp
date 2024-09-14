#include <Arduino.h>
#include "DriveController.hpp"
#include "ArmController.hpp"
#include "InputController.hpp"

DriveController driveController;
ArmController armController;
InputController inputController;

void setup() {
  Serial.begin(115200);
  // 子機との通信用
  Serial2.begin(115200);

  // 足回りの設定
  Serial.println("Setting up wheel motors...");
  driveController.setup();

  // アームのサーボの設定
  Serial.println("Setting up servos...");
  armController.setup();

  // PS5コントローラーの設定
  Serial.println("Setting up PS5 controller...");
  inputController.setup();

  Serial.println("Setup complete.");
}

void loop() {
  while (ps5.isConnected() == false) {
    Serial.println("PS5 controller not found");
  }

  while (ps5.isConnected() == true) {
    int rStickX = inputController.roundedRStickX();
    double gain = 0.005;
    driveController.changeTargetAngle(rStickX * gain);

    int lStickX = inputController.roundedLStickX();
    int lStickY = inputController.roundedLStickY();
    driveController.drive(lStickX * 2, lStickY * 2);

    if (ps5.L1()) {
      Serial2.println("rotate_box_motor1");
    }
    if (ps5.R1()) {
      Serial2.println("rotate_box_motor2");
    }
  }
}
