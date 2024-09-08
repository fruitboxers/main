#include <Arduino.h>
#include <ps5Controller.h>
#include <ESP32Servo.h>

// ピン配置をまとめて定義
#define wheel_motor1_pwm 32
#define wheel_motor1_dir 33
#define wheel_motor2_pwm 25
#define wheel_motor2_dir 26
#define wheel_motor3_pwm 27
#define wheel_motor3_dir 14
#define photo_sensor_1 12
#define photo_sensor_2 13
#define photo_sensor_3 15
#define bno055_sda 21
#define bno055_scl 22
#define arm_servo1 17
#define arm_servo2 16
#define belt_motor_1 19
#define belt_motor_2 18
#define switch 23

// 足回りのPWM設定
#define WHEEL_PWM_CHANNEL_0 6
#define WHEEL_PWM_CHANNEL_1 7
#define WHEEL_PWM_CHANNEL_2 8
#define WHEEL_PWM_FREQ 20000
#define WHEEL_PWM_RESOLUTION 8

// アームのサーボの設定
// PWMチャンネル0,1を使用中
Servo servo1;
Servo servo2;
// サーボのパルス幅
#define servoMinUs 500
#define servoMaxUs 2400

// x,y: -256~256
void move(int x, int y) {
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(", y: ");
  Serial.println(y);

  // 1:右前輪, 2:左前輪, 3:後輪
  int wheel1_speed = (-0.5 * x) + (0.86602540378 * y);
  int wheel2_speed = (-0.5 * x) - (0.86602540378 * y);
  int wheel3_speed = x;

  digitalWrite(wheel_motor1_dir, wheel1_speed > 0 ? HIGH : LOW);
  digitalWrite(wheel_motor2_dir, wheel2_speed > 0 ? HIGH : LOW);
  digitalWrite(wheel_motor3_dir, wheel3_speed > 0 ? HIGH : LOW);
  ledcWrite(WHEEL_PWM_CHANNEL_0, abs(wheel1_speed));
  ledcWrite(WHEEL_PWM_CHANNEL_1, abs(wheel2_speed));
  ledcWrite(WHEEL_PWM_CHANNEL_2, abs(wheel3_speed));

  Serial.print("wheel1_speed: ");
  Serial.print(wheel1_speed);
  Serial.print(", wheel2_speed: ");
  Serial.print(wheel2_speed);
  Serial.print(", wheel3_speed: ");
  Serial.println(wheel3_speed);
}

void setup() {
  Serial.begin(115200);

  // 足回りの設定
  Serial.println("Setting up wheel motors...");
  pinMode(wheel_motor1_pwm, OUTPUT);
  pinMode(wheel_motor1_dir, OUTPUT);
  pinMode(wheel_motor2_pwm, OUTPUT);
  pinMode(wheel_motor2_dir, OUTPUT);
  pinMode(wheel_motor3_pwm, OUTPUT);
  pinMode(wheel_motor3_dir, OUTPUT);
  ledcSetup(WHEEL_PWM_CHANNEL_0, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
  ledcSetup(WHEEL_PWM_CHANNEL_1, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
  ledcSetup(WHEEL_PWM_CHANNEL_2, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
  ledcAttachPin(wheel_motor1_pwm, WHEEL_PWM_CHANNEL_0);
  ledcAttachPin(wheel_motor2_pwm, WHEEL_PWM_CHANNEL_1);
  ledcAttachPin(wheel_motor3_pwm, WHEEL_PWM_CHANNEL_2);

  // アームのサーボの設定
  Serial.println("Setting up servos...");
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo1.attach(arm_servo1, servoMinUs, servoMaxUs);
  servo2.attach(arm_servo2, servoMinUs, servoMaxUs);

  // PS5コントローラーの設定
  Serial.println("Setting up PS5 controller...");
  ps5.begin("e8:47:3a:8d:36:9a");

  Serial.println("Setup complete.");
}

void loop() {
  while (ps5.isConnected() == true) {
    move(ps5.LStickX() * 2, ps5.LStickY() * 2);
  }
}
