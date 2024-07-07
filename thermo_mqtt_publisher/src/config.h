#ifndef CONFIG_H
#define CONFIG_H

#include "secrets.h"

// Pin definitions
#define THERMISTOR_PIN A0

// Thermistor constants
#define SERIES_RESISTOR 10000    // 10kΩ resistor
#define NOMINAL_RESISTANCE 10000 // 10kΩ nominal resistance of the thermistor
#define NOMINAL_TEMPERATURE 25   // nominal temperature (25°C)
#define B_COEFFICIENT 3950       // B coefficient of the thermistor

// MQTT configuration
const char broker[] = "192.168.55.102";
const int port = 1883;
const char topic[] = "temperature-topic";

// WiFi configuration
char ssid[] = SECRET_SSID; // network SSID
char pass[] = SECRET_PASS; // network password

#endif