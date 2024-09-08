#include <Arduino.h>
#include <ps5Controller.h>
#include "DriveController.h"
#include "ArmController.h"

DriveController driveController;
ArmController armController;

void setup() {
  Serial.begin(115200);

  // 足回りの設定
  Serial.println("Setting up wheel motors...");
  driveController.setup();

  // アームのサーボの設定
  Serial.println("Setting up servos...");
  armController.setup();

  // PS5コントローラーの設定
  Serial.println("Setting up PS5 controller...");
  ps5.begin("e8:47:3a:8d:36:9a");

  Serial.println("Setup complete.");
}

void loop() {
  while (ps5.isConnected() == true) {
    driveController.drive(ps5.LStickX() * 2, ps5.LStickY() * 2);
  }
}
