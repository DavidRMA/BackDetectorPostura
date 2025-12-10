#include "PostureEvaluator.h"
#include <math.h>

void PostureEvaluator::setAge(int age) {
  _age = age;
  _thresholdDeg = computeThresholdForAge(age);
}

void PostureEvaluator::setThreshold(float thresholdDeg) {
  _thresholdDeg = thresholdDeg;
}

void PostureEvaluator::setOffsets(float offX, float offY) {
  _offsetX = offX;
  _offsetY = offY;
}

PostureStatus PostureEvaluator::evaluate(float rawX, float rawY) {
  PostureStatus s;

  // 1. Aplicar offsets de calibración
  s.angleX = rawX - _offsetX;
  s.angleY = rawY - _offsetY;

  // 2. Magnitud del vector de inclinación (recomendado):
  //    tilt = sqrt( X^2 + Y^2 )
  float tilt = sqrt(s.angleX * s.angleX + s.angleY * s.angleY);

  s.maxAngle   = tilt;          // Interpretamos maxAngle como "tilt total"
  s.threshold  = _thresholdDeg;
  s.isBadPosture = (tilt > _thresholdDeg);

  return s;
}

// Ejemplo sencillo de umbrales por edad
float PostureEvaluator::computeThresholdForAge(int age) {
  if (age <= 12) {
    // Niños: más estrictos
    return 10.0;
  } else if (age <= 59) {
    // Adultos: intermedio
    return 15.0;
  } else {
    // Adulto mayor: un poco más permisivos
    return 20.0;
  }
}