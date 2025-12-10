
#include "MPU6050Sensor.h"
#include <Wire.h>
#include <math.h>

bool MPU6050Sensor::begin() {
  Wire.begin();
  Wire.beginTransmission(0x68);
  if (Wire.endTransmission() != 0) {
    _initialized = false;
    return false;
  }
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  _initialized = true;
  return true;
}

bool MPU6050Sensor::readAngles(float& angleX, float& angleY) {
  if (!_initialized) return false;

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) return false;

  if (Wire.requestFrom(0x68, 6) != 6) return false;

  int16_t ax = Wire.read() << 8 | Wire.read();
  int16_t ay = Wire.read() << 8 | Wire.read();
  int16_t az = Wire.read() << 8 | Wire.read();

  float gx = ax / 4096.0;
  float gy = ay / 4096.0;
  float gz = az / 4096.0;

  angleX = atan2(gy, gz) * 180 / PI;
  angleY = atan2(-gx, gz) * 180 / PI;

  return true;
}