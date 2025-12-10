#include "posture/PostureEvaluator.h"
#include <math.h>

PostureEvaluator::PostureEvaluator()
  : _offsetX(0.0f),
    _offsetY(0.0f),
    _thresholdDeg(DEFAULT_THRESHOLD_DEG),
    _age(DEFAULT_AGE)
{
}

// ---- Calibración: offsets de postura correcta ----
void PostureEvaluator::setOffsets(float ox, float oy)
{
  _offsetX = ox;
  _offsetY = oy;
}

// ---- Umbral directo (si viene del backend) ----
void PostureEvaluator::setThreshold(float deg)
{
  _thresholdDeg = deg;
}

// ---- Edad: recalcula umbral automáticamente ----
void PostureEvaluator::setAge(int age)
{
  _age = age;
  _thresholdDeg = computeThresholdForAge(age);
}

int PostureEvaluator::getAge() const
{
  return _age;
}

float PostureEvaluator::getThreshold() const
{
  return _thresholdDeg;
}

// Recibe ángulos CRUDOS del MPU (en grados)
PostureStatus PostureEvaluator::evaluate(float rawX, float rawY)
{
  PostureStatus s;

  // 1) pasar a coordenadas relativas (respecto a la postura calibrada)
  float relX = rawX - _offsetX;
  float relY = rawY - _offsetY;

  // 2) magnitud del vector (tilt)
  float tilt = sqrt(relX * relX + relY * relY);

  // 3) comparar con umbral
  bool bad = tilt > _thresholdDeg;

  s.angleX    = relX;
  s.angleY    = relY;
  s.maxAngle  = tilt;
  s.threshold = _thresholdDeg;
  s.isBadPosture = bad;

  return s;
}

// 🔹 REGLA POR EDADES (ajústala si quieres)
float PostureEvaluator::computeThresholdForAge(int age)
{
  // Niños (<=12): más estrictos
  // Adolescentes (13-17)
  // Adultos (18-59)
  // Adulto mayor (60+)
  if (age <= 12) {
    return 10.0f;      // postura casi recta
  } else if (age <= 17) {
    return 12.0f;
  } else if (age <= 59) {
    return 15.0f;      // adulto estándar
  } else {
    return 18.0f;      // adulto mayor, más margen
  }
}
