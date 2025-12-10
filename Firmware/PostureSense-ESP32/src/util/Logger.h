#pragma once
#include <Arduino.h>

struct PostureStatus
{
    float angleX;
    float angleY;
    float maxAngle;
    float threshold;
    bool isBadPosture;
};

class PostureEvaluator
{
public:
    void setAge(int age);
    void setThreshold(float thresholdDeg); // para cuando venga del backend
    void setOffsets(float offX, float offY);
    PostureStatus evaluate(float rawX, float rawY);

private:
    int _age = 25;
    float _thresholdDeg = 15.0;
    float _offsetX = 0.0;
    float _offsetY = 0.0;

    float computeThresholdForAge(int age);
};
