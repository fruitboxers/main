#include "inputController.hpp"

InputController::InputController() {
  ps5.begin("e8:47:3a:8d:36:9a");
}

int InputController::roundFraction(int value) {
  int limit = 10;
  // プラマイlimit以下なら0にする
  return (value < limit && value > -limit) ? 0 : value;
}

int InputController::roundedLStickX() {
  return roundFraction(ps5.LStickX());
}

int InputController::roundedLStickY() {
  return roundFraction(ps5.LStickY());
}

int InputController::roundedRStickX() {
  return roundFraction(ps5.RStickX());
}

int InputController::roundedRStickY() {
  return roundFraction(ps5.RStickY());
}