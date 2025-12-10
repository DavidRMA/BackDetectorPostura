#pragma once
#include <Arduino.h>

class MPU6050Sensor {
public:
  bool begin();
  bool readAngles(float& angleX, float& angleY);

private:
  bool _initialized = false;
};