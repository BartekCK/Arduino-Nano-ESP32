#include "thermistor.h"

Thermistor::Thermistor(int seriesResistor, int nominalResistance, int nominalTemperature, int bCoefficient)
{
    this->seriesResistor = seriesResistor;
    this->nominalResistance = nominalResistance;
    this->nominalTemperature = nominalTemperature;
    this->bCoefficient = bCoefficient;
}

float Thermistor::calculateCelciousTemperature(int rawValue)
{
    float resistance = this->seriesResistor / ((4095.0 / rawValue) - 1);
    float steinhart;

    steinhart = resistance / this->nominalResistance;       // (R/Ro)
    steinhart = log(steinhart);                             // ln(R/Ro)
    steinhart /= this->bCoefficient;                        // 1/B * ln(R/Ro)
    steinhart += 1.0 / (this->nominalTemperature + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                            // Invert
    steinhart -= 273.15;                                    // convert to C

    return steinhart;
}