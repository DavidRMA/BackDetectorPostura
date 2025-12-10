#include "Calibration.h"
#include <EEPROM.h>

/// Tamaño de la EEPROM emulada (ESP32)
#define EEPROM_SIZE 64

/// Dirección base donde guardaremos la calibración
#define CALIB_EEPROM_ADDR 0

/// Magic number para saber si hay datos válidos
#define CALIB_MAGIC 0xDEADBEEF

struct CalibData {
    uint32_t magic;
    float offsetX;
    float offsetY;
};

Calibration::Calibration(MPU6050Sensor &mpu, PostureEvaluator &eval)
    : _mpu(mpu), _eval(eval)
{
}

bool Calibration::loadOffsetsFromEEPROM()
{
    if (!EEPROM.begin(EEPROM_SIZE)) {
        Serial.println("[CALIB] ❌ Error inicializando EEPROM");
        return false;
    }

    CalibData data;
    EEPROM.get(CALIB_EEPROM_ADDR, data);

    if (data.magic != CALIB_MAGIC) {
        Serial.println("[CALIB] ⚠ No hay calibración válida en EEPROM");
        return false;
    }

    _offsetX = data.offsetX;
    _offsetY = data.offsetY;
    _eval.setOffsets(_offsetX, _offsetY);

    Serial.println("[CALIB] ✅ Calibración cargada desde EEPROM");
    Serial.print("       offsetX = ");
    Serial.print(_offsetX);
    Serial.print("°, offsetY = ");
    Serial.print(_offsetY);
    Serial.println("°");

    return true;
}

void Calibration::saveOffsetsToEEPROM()
{
    CalibData data;
    data.magic   = CALIB_MAGIC;
    data.offsetX = _offsetX;
    data.offsetY = _offsetY;

    EEPROM.put(CALIB_EEPROM_ADDR, data);
    EEPROM.commit();

    Serial.println("[CALIB] 💾 Offsets guardados en EEPROM");
    Serial.print("       offsetX = ");
    Serial.print(_offsetX);
    Serial.print("°, offsetY = ");
    Serial.print(_offsetY);
    Serial.println("°");
}

void Calibration::runCalibration()
{
    Serial.println("\n===== 🔧 INICIANDO CALIBRACIÓN DE POSTURA =====");
    Serial.println("Por favor mantén una postura CORRECTA durante 3 segundos...");
    delay(3000);

    float x = 0, y = 0;
    float sumX = 0, sumY = 0;
    const int samples = 50;

    Serial.println("📡 Midiendo posición de referencia...");

    for (int i = 0; i < samples; i++)
    {
        if (_mpu.readAngles(x, y))
        {
            sumX += x;
            sumY += y;
        }
        delay(50);
    }

    _offsetX = sumX / samples;
    _offsetY = sumY / samples;

    // Pasar offsets al PostureEvaluator
    _eval.setOffsets(_offsetX, _offsetY);

    Serial.println("\n🎯 CALIBRACIÓN COMPLETADA!");
    Serial.print("   Offsets aplicados: X=");
    Serial.print(_offsetX);
    Serial.print("°, Y=");
    Serial.print(_offsetY);
    Serial.println("°");
    Serial.println("   ✅ Ahora 0° = posición actual del sensor");
    Serial.println("============================================\n");
}
