# fruitboxers-main

SSR杯本番用のメインプログラム

## 構造

- `main.cpp`
  - メインプログラム
- `DriveController.*`
  - 足回りのモーター3つと、BNO055（9軸センサー）を管理するクラス
- `ArmController.*`
  - アームのサーボモーター2つと昇降用のDCモーターを管理するクラス
- `InputController.*`
  - PS5コントローラーの入力を管理するクラス
- `pins.h`
  - ピンの割り当てを定義してあるファイル