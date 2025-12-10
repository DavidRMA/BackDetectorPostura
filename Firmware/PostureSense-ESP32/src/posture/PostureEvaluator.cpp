#pragma once
#include <Arduino.h>
#include <math.h>
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
  PostureEvaluator()
    : _offsetX(0.0f),
      _offsetY(0.0f),
      _age(DEFAULT_AGE),
      _thresholdDeg(DEFAULT_THRESHOLD_DEG) {}

  // ---- Calibración: offsets de postura correcta ----
  void setOffsets(float ox, float oy)
  {
    _offsetX = ox;
    _offsetY = oy;
  }

  // ---- Umbral directo (si viene del backend) ----
  void setThreshold(float deg)
  {
    _thresholdDeg = deg;
  }

  // ---- Edad: recalcula umbral automáticamente ----
  void setAge(int age)
  {
    _age = age;
    _thresholdDeg = computeThresholdForAge(age);
  }

  int getAge() const { return _age; }
  float getThreshold() const { return _thresholdDeg; }

  // Recibe ángulos CRUDOS del MPU (en grados)
  PostureStatus evaluate(float rawX, float rawY)
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

private:
  float _offsetX;
  float _offsetY;
  float _thresholdDeg;
  int   _age;

  // 🔹 REGLA POR EDADES (puedes ajustarla a lo que hablaron con el profe)
  float computeThresholdForAge(int age)
  {
    // Ejemplo de política:
    // Niños (6-12): más estrictos
    // Adolescentes (13-17): un poco más flexibles
    // Adultos (18-59): estándar
    // Adulto mayor (60+): más tolerancia
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
};
