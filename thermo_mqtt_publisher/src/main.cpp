#include <Arduino.h>

#include "secrets.h"

#include "./thermistor/thermistor.h"
#include "./local_connection_manager/local_connection_manager.h"
#include "./mqtt_client/mqtt_client.h"

#define THERMISTOR_PIN A0
#define SERIES_RESISTOR 10000    // 10kΩ resistor
#define NOMINAL_RESISTANCE 10000 // 10kΩ nominal resistance of the thermistor
#define NOMINAL_TEMPERATURE 25   // nominal temperature (25°C)
#define B_COEFFICIENT 3950       // B coefficient of the thermistor

char ssid[] = SECRET_SSID; // network SSID
char pass[] = SECRET_PASS; // network password

const char broker[] = "192.168.55.102";
int port = 1883;
const char topic[] = "temperature-topic";

LocalConnectionManager localConnectionManager(ssid, pass);
MqttClient mqttClient(broker, port, localConnectionManager.getWifiClient());

Thermistor thermistor(SERIES_RESISTOR, NOMINAL_RESISTANCE, NOMINAL_TEMPERATURE, B_COEFFICIENT);

// set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

void setup()
{
  Serial.begin(9600);
  // while (!Serial) {
  //; // wait for serial port to connect. Needed for native USB port only
  // }

  localConnectionManager.connect();
  mqttClient.connect();
}

void loop()
{
  mqttClient.loop(); // Maintain MQTT connection

  int rawValue = analogRead(THERMISTOR_PIN);

  float celciousTemperature = thermistor.calculateCelciousTemperature(rawValue);
  Serial.print("Celcious temperature: ");
  Serial.print(celciousTemperature);
  Serial.println(" *C");

  char tempStr[16];
  dtostrf(celciousTemperature, 6, 2, tempStr);

  mqttClient.publish(topic, tempStr);

  delay(1000);
}
