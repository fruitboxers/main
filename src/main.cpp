#include <Arduino.h>
#include "DriveController.hpp"
#include "ArmController.hpp"
#include "InputController.hpp"

DriveController* driveController;
ArmController* armController;
InputController* inputController;

bool l1IsPressed = false;
bool r1IsPressed = false;

void setup() {
  Serial.begin(115200);
  // 子機との通信用
  Serial2.begin(115200);

  // 足回りの設定
  Serial.println("Setting up wheel motors...");
  driveController = new DriveController();

  // アームのサーボの設定
  Serial.println("Setting up servos...");
  armController = new ArmController();

  // PS5コントローラーの設定
  Serial.println("Setting up PS5 controller...");
  inputController = new InputController();

  Serial.println("Setup complete.");
}

void loop() {
  while (ps5.isConnected() == false) {
    Serial.println("PS5 controller not found");

    // 最初はピンの状態がおかしくなってる可能性があるので、
    // コントローラー接続前からジャイロによる角度補正を働かせておく
    driveController->drive({0, 0});
  }

  while (ps5.isConnected() == true) {
    // 右スティックで回転
    int rStickX = inputController->roundedRStickX();
    double gain = 0.005;
    driveController->changeTargetAngle(rStickX * gain);

    // 左スティックで並行移動
    int lStickX = inputController->roundedLStickX();
    int lStickY = inputController->roundedLStickY();
    driveController->drive({lStickX * 2, lStickY * 2});

    // L1、R1を押すと子機経由で射出機構が動く
    if (ps5.L1() && !l1IsPressed) {
      Serial2.println("motor1");
      Serial.println("send motor1");
      //delay(20);
      l1IsPressed = true;
    }
    if (ps5.R1() && !r1IsPressed) {
      Serial2.println("motor2");
      Serial.println("send motor2");
      //delay(20);
      r1IsPressed = true;
    }
    if (!ps5.L1() && l1IsPressed) {
      l1IsPressed = false;
    }
    if (!ps5.R1() && r1IsPressed) {
      r1IsPressed = false;
    }

    // 丸、三角ボタンでアームの開閉
    if (ps5.Circle()) {
      armController->openArm();
    }
    if (ps5.Triangle()) {
      armController->closeArm();
    }

    // 四角ボタンで自律走行開始
    if (ps5.Square()) {
      driveController->startAutoDrive();
    }

    // バツボタンで自律走行停止、昇降機構停止、自動回収停止
    if (ps5.Cross()) {
      driveController->forceStopAutoDrive();
      armController->stopBelt();
      armController->forceStopAutoCollect();
    }

    // 上下ボタンでアームの上下（昇降機構）
    if (ps5.Up()) {
      armController->moveBelt(true);
    }
    if (ps5.Down()) {
      armController->moveBelt(false);
    }

    // 左右ボタンでアームの首振り
    if (ps5.Left()) {
      armController->swingArmToLeft();
    }
    if (ps5.Right()) {
      armController->swingArmToRight();
    }
    if (!ps5.Left() && armController->leftIsPressed) {
      armController->leftIsPressed = false;
    }
    if (!ps5.Right() && armController->rightIsPressed) {
      armController->rightIsPressed = false;
    }

    armController->observeBeltState();

    // シェア、オプションボタンで左右それぞれに格納する自動回収を開始
    if (ps5.Share()) {
      armController->startAutoCollect(0);
    }
    if (ps5.Options()) {
      armController->startAutoCollect(180);
    }
  }
}
