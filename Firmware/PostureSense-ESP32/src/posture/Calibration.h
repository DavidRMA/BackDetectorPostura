#pragma once
#include <Arduino.h>
#include "sensors/MPU6050Sensor.h"
#include "posture/PostureEvaluator.h"

class Calibration
{
public:
    Calibration(MPU6050Sensor &mpu, PostureEvaluator &eval);

    // Calibrar leyendo la postura correcta (pidiendo al usuario que se quede quieto)
    void runCalibration();

    // Cargar offsets desde EEPROM (true = había datos válidos)
    bool loadOffsetsFromEEPROM();

    // Guardar offsets actuales en EEPROM
    void saveOffsetsToEEPROM();

private:
    MPU6050Sensor &_mpu;
    PostureEvaluator &_eval;

    float _offsetX = 0.0f;
    float _offsetY = 0.0f;
};
