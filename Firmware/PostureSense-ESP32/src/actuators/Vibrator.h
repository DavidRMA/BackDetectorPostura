#pragma once
#include <Arduino.h>

class Vibrator {
public:
  explicit Vibrator(uint8_t pin) : _pin(pin) {}
  void begin();
  void off();
  void patternAlert();   // patrón estándar de vibración

private:
  uint8_t _pin;
};