#include "ArmController.h"

// PWMチャンネル0,1を使用中
Servo servo1;
Servo servo2;

ArmController::ArmController() {
}

void ArmController::setup() {
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo1.attach(ARM_SERVO1_PIN, servoMinUs, servoMaxUs);
  servo2.attach(ARM_SERVO2_PIN, servoMinUs, servoMaxUs);
}