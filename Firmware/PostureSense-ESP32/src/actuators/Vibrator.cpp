#include "Vibrator.h"

void Vibrator::begin() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void Vibrator::off() {
  digitalWrite(_pin, LOW);
}

void Vibrator::patternAlert() {
  // Ejemplo: ON 500ms / OFF 500ms
  if (millis() % 1000 < 500) {
    digitalWrite(_pin, HIGH);
  } else {
    digitalWrite(_pin, LOW);
  }
}