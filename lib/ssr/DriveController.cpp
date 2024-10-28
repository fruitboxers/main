#include "DriveController.hpp"

DriveController::DriveController() {
  // モーター類の設定
  pinMode(WHEEL_MOTOR1_PWM_PIN, OUTPUT);
  pinMode(WHEEL_MOTOR1_DIR_PIN, OUTPUT);
  pinMode(WHEEL_MOTOR2_PWM_PIN, OUTPUT);
  pinMode(WHEEL_MOTOR2_DIR_PIN, OUTPUT);
  pinMode(WHEEL_MOTOR3_PWM_PIN, OUTPUT);
  pinMode(WHEEL_MOTOR3_DIR_PIN, OUTPUT);

  ledcSetup(pwmChannel1, pwmFreq, pwmResolution);
  ledcSetup(pwmChannel2, pwmFreq, pwmResolution);
  ledcSetup(pwmChannel3, pwmFreq, pwmResolution);
  ledcAttachPin(WHEEL_MOTOR1_PWM_PIN, pwmChannel1);
  ledcAttachPin(WHEEL_MOTOR2_PWM_PIN, pwmChannel2);
  ledcAttachPin(WHEEL_MOTOR3_PWM_PIN, pwmChannel3);

  // BNO055の設定
  if(!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    // BNO055と接続できなかったらここでプログラム停止
    while(1);
  }

  bno.setExtCrystalUse(true);
}

double DriveController::PID(double error, double pGain, double iGain, double dGain) {
  if (error > 180) {
    error -= 360;
  } else if (error < -180) {
      error += 360;
  }
  errorSum += error;
  double errorDiff = error - preError;
  preError = error;
  return (pGain * error) + (iGain * errorSum) + (dGain * errorDiff);
}

void DriveController::drive(Vector2 vector) {
  // BNO055から角度を取得
  double currentAngle = getAngle();
  double rotateSpeed = -1 * PID(targetAngle - currentAngle, 2, 0.01, 0.1);

  if (autoDriveStage != OFF) {
    // 自律制御がオンの時
    AutoDriveStageInfo info = getAutoDriveInfo();

    long currentAutoDriveDuration = millis() - currentAutoDriveStartTime;
    if (currentAutoDriveDuration > info.duration) {
      // currentAutoDriveStartTimeをリセットし、次のステージに移行
      autoDriveStage = static_cast<AutoDriveStage>((autoDriveStage + 1) % 4);
      currentAutoDriveStartTime = millis();
    } else {
      // コントローラーの入力を自律制御の情報で上書き
      vector = info.vector;
    }
  }

  double radiusRatio = 1.2; // 中心から前輪までの距離に対する、中心から後輪までの距離の比

  // 1:右前輪, 2:左前輪, 3:後輪
  int speed1 = (-0.5 * vector.x) + (0.86602540378 * vector.y) + rotateSpeed;
  int speed2 = (-0.5 * vector.x) - (0.86602540378 * vector.y) + rotateSpeed;
  int speed3 = vector.x + (rotateSpeed * radiusRatio);

  digitalWrite(WHEEL_MOTOR1_DIR_PIN, speed1 > 0 ? HIGH : LOW);
  digitalWrite(WHEEL_MOTOR2_DIR_PIN, speed2 > 0 ? HIGH : LOW);
  digitalWrite(WHEEL_MOTOR3_DIR_PIN, speed3 > 0 ? HIGH : LOW);
  ledcWrite(pwmChannel1, abs(speed1));
  ledcWrite(pwmChannel2, abs(speed2));
  ledcWrite(pwmChannel3, abs(speed3));

  Serial.print("x: ");
  Serial.print(vector.x);
  Serial.print(", y: ");
  Serial.println(vector.y);

  Serial.print("currentAngle: ");
  Serial.print(currentAngle);
  Serial.print(", targetAngle: ");
  Serial.print(targetAngle);
  Serial.print(", rotateSpeed: ");
  Serial.print(rotateSpeed);
  Serial.print(", speed1: ");
  Serial.print(speed1);
  Serial.print(", speed2: ");
  Serial.print(speed2);
  Serial.print(", speed3: ");
  Serial.println(speed3);
}

double DriveController::getAngle() {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  return euler.x();
}

void DriveController::changeTargetAngle(double range) {
  targetAngle += range;
  // 0~360度に収める
  targetAngle = fmod(targetAngle, 360);
  if (targetAngle < 0) {
    targetAngle += 360;
  }
}

void DriveController::startAutoDrive() {
  Serial.println("Auto drive started");
  autoDriveStage = FORWARD;
}

void DriveController::forceStopAutoDrive() {
  Serial.println("Auto drive stopped");
  autoDriveStage = OFF;
}

AutoDriveStageInfo DriveController::getAutoDriveInfo() {
  // TODO: もっと速度上げてもいいかも
  switch (autoDriveStage) {
    case FORWARD:
      Serial.println("forward");
      return {{0, 100}, 1800};
    case RIGHT:
      Serial.println("right");
      return {{100, 0}, 2500};
    case BACKWARD:
      Serial.println("backward");
      return {{0, -100}, 15000};
    default:
      return {{0, 0}, 0};
  }
}