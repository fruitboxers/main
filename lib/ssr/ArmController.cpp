#include "ArmController.hpp"

ArmController::ArmController() {
  // サーボの設定
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo1.attach(ARM_SERVO1_PIN, servoMinUs, servoMaxUs);
  servo2.attach(ARM_SERVO2_PIN, servoMinUs, servoMaxUs);

  // 昇降用モーターの設定
  pinMode(BELT_MOTOR_DIR_PIN, OUTPUT);

  // スイッチの設定
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // デフォルトの首振りの角度に設定
  servo2.write(armSwingAngle);
  servo1.write(120);
}

void ArmController::moveBelt(bool isUp) {
  Serial.println("moveBelt");
  bool switchIsPressed = digitalRead(SWITCH_PIN) == HIGH;

  // 上昇中かつスイッチが押されているときは昇降しない
  if (!(isUp && switchIsPressed)) {
    digitalWrite(BELT_MOTOR_DIR_PIN, isUp ? HIGH : LOW);
    analogWrite(BELT_MOTOR_PWM_PIN, 255);
  } else {
    stopBelt();
  }

  beltIsUp = isUp;
}

void ArmController::stopBelt() {
  analogWrite(BELT_MOTOR_PWM_PIN, 0);
}

void ArmController::loop() {
  bool switchIsPressed = digitalRead(SWITCH_PIN) == HIGH;
  if (beltIsUp && switchIsPressed) {
    stopBelt();
  }

  // 自動回収中の処理
  if (isAutoCollecting) {
    if (!switchIsPressed) {
      // 上まで上がりきってなかったら上昇させる
      moveBelt(true);
    } else if (armSwingAngle != autoCollectAngle) {
      // 首振り角度が目標角度と違ったら回転させる
      armSwingAngle = autoCollectAngle;
      servo2.write(armSwingAngle);
      delay(500);
    // } else {
      // 上記の条件が全て揃っていたら、アームを開いてボールをboxに入れる
      servo1.write(120);
      delay(1000);
      servo1.write(175);
      delay(500);
      // 首振りの向きを真ん中に戻す
      servo2.write(90);
      armSwingAngle = 90;
      delay(500);
      // ベルト下降
      moveBelt(false);
      delay(1500);
      servo1.write(120);
      delay(1500);
      stopBelt();
      isAutoCollecting = false;
    }
  }
}

void ArmController::openArm() {
  Serial.println("open arm");
  servo1.write(120);
  //delay(100);
}

void ArmController::closeArm() {
  Serial.println("close arm");
  servo1.write(175);
  //delay(100);
}

void ArmController::swingArmToLeft() {
  if (!leftIsPressed) {
    if (armSwingAngle > 0) {
      armSwingAngle -= 90;
    }
    servo2.write(armSwingAngle);
    leftIsPressed = true;
  }
}

void ArmController::swingArmToRight() {
  if (!rightIsPressed) {
    if (armSwingAngle < 180) {
      armSwingAngle += 90;
    }
    servo2.write(armSwingAngle);
    rightIsPressed = true;
  }
}

void ArmController::startAutoCollect(int angle) {
  isAutoCollecting = true;
  autoCollectAngle = angle;
}

void ArmController::forceStopAutoCollect() {
  if (isAutoCollecting) {
    stopBelt();
  }
  isAutoCollecting = false;
}