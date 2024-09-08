#include "DriveController.h"

DriveController::DriveController() {
}

void DriveController::setup() {
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
}

void DriveController::drive(int x, int y) {
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(", y: ");
  Serial.println(y);

  // 1:右前輪, 2:左前輪, 3:後輪
  int wheel1_speed = (-0.5 * x) + (0.86602540378 * y);
  int wheel2_speed = (-0.5 * x) - (0.86602540378 * y);
  int wheel3_speed = x;

  digitalWrite(WHEEL_MOTOR1_DIR_PIN, wheel1_speed > 0 ? HIGH : LOW);
  digitalWrite(WHEEL_MOTOR2_DIR_PIN, wheel2_speed > 0 ? HIGH : LOW);
  digitalWrite(WHEEL_MOTOR3_DIR_PIN, wheel3_speed > 0 ? HIGH : LOW);
  ledcWrite(pwmChannel1, abs(wheel1_speed));
  ledcWrite(pwmChannel2, abs(wheel2_speed));
  ledcWrite(pwmChannel3, abs(wheel3_speed));

  Serial.print("wheel1_speed: ");
  Serial.print(wheel1_speed);
  Serial.print(", wheel2_speed: ");
  Serial.print(wheel2_speed);
  Serial.print(", wheel3_speed: ");
  Serial.println(wheel3_speed);
}