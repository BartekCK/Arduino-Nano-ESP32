#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <Arduino.h>

class Thermistor
{
public:
    Thermistor(int seriesResistor, int nominalResistance, int nominalTemperature, int bCoefficient);
    float calculateCelciousTemperature(int rawValue);

private:
    int seriesResistor;
    int nominalResistance;
    int nominalTemperature;
    int bCoefficient;
};

#endif