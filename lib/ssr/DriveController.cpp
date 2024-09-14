#include "DriveController.h"

DriveController::DriveController() {
}

void DriveController::setup() {
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
    while(1);
  }

  bno.setExtCrystalUse(true);
}

void DriveController::drive(int x, int y) {
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(", y: ");
  Serial.println(y);

  // BNO055から角度を取得
  double currentAngle = getAngle();
  double diff = currentAngle - targetAngle;
  if (diff > 180) {
    diff -= 360;
  } else if (diff < -180) {
      diff += 360;
  }
  double gain = 2; // 角度補正をどのくらい影響させるか

  // 1:右前輪, 2:左前輪, 3:後輪
  int speed1 = (-0.5 * x) + (0.86602540378 * y) + (gain * diff);
  int speed2 = (-0.5 * x) - (0.86602540378 * y) + (gain * diff);
  int speed3 = x + (gain * diff);

  digitalWrite(WHEEL_MOTOR1_DIR_PIN, speed1 > 0 ? HIGH : LOW);
  digitalWrite(WHEEL_MOTOR2_DIR_PIN, speed2 > 0 ? HIGH : LOW);
  digitalWrite(WHEEL_MOTOR3_DIR_PIN, speed3 > 0 ? HIGH : LOW);
  ledcWrite(pwmChannel1, abs(speed1));
  ledcWrite(pwmChannel2, abs(speed2));
  ledcWrite(pwmChannel3, abs(speed3));

  Serial.print("currentAngle: ");
  Serial.print(currentAngle);
  Serial.print(", targetAngle: ");
  Serial.print(targetAngle);
  Serial.print(", diff: ");
  Serial.print(diff);
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