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

void ArmController::observeBeltState() {
  bool switchIsPressed = digitalRead(SWITCH_PIN) == HIGH;
  if (beltIsUp && switchIsPressed) {
    stopBelt();
  }
}

void autoCollectTask(void *pvParameters) {
  ArmController *aC = static_cast<ArmController*>(pvParameters);

  while (digitalRead(SWITCH_PIN) == LOW) {
    // 上まで上がりきってなかったら上昇させる
    aC->moveBelt(true);
  }

  aC->armSwingAngle = aC->autoCollectAngle;
  aC->servo2.write(aC->armSwingAngle);
  delay(500);
  // アームを開いてボールをboxに入れる
  aC->servo1.write(120);
  delay(1000);
  aC->servo1.write(175);
  delay(500);
  // 首振りの向きを真ん中に戻す
  aC->servo2.write(90);
  aC->armSwingAngle = 90;
  delay(500);
  // ベルト下降
  aC->moveBelt(false);
  delay(1500);
  aC->servo1.write(120);
  delay(1500);
  aC->stopBelt();

  vTaskDelete(NULL);
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
  // isAutoCollecting = true;
  autoCollectAngle = angle;
  xTaskCreateUniversal(autoCollectTask, "autoCollect", 4096, this, 0, &autoCollectHandle, APP_CPU_NUM);
}

void ArmController::forceStopAutoCollect() {
  bool isAutoCollecting = eTaskGetState(autoCollectHandle) != eDeleted;
  if (isAutoCollecting) {
    stopBelt();
  }
  vTaskDelete(autoCollectHandle);
}