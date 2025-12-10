#pragma once
#include <Arduino.h>
#include "config/DeviceConfig.h"   // para DEFAULT_AGE y DEFAULT_THRESHOLD_DEG

struct PostureStatus
{
  float angleX;     // X relativa (ya con offset aplicado)
  float angleY;     // Y relativa
  float maxAngle;   // magnitud del vector (tilt)
  float threshold;  // umbral usado (deg)
  bool  isBadPosture;
};

class PostureEvaluator
{
public:
  PostureEvaluator();

  // ---- Calibración: offsets de postura correcta ----
  void setOffsets(float ox, float oy);

  // ---- Umbral directo (si viene del backend) ----
  void setThreshold(float deg);

  // ---- Edad: recalcula umbral automáticamente ----
  void setAge(int age);

  int   getAge() const;
  float getThreshold() const;

  // Recibe ángulos CRUDOS del MPU (en grados)
  PostureStatus evaluate(float rawX, float rawY);

private:
  float _offsetX;
  float _offsetY;
  float _thresholdDeg;
  int   _age;

  // regla por edades
  float computeThresholdForAge(int age);
};
