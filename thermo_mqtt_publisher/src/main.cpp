#include <Arduino.h>

#include "config.h"

#include "./thermistor/thermistor.h"
#include "./local_connection_manager/local_connection_manager.h"
#include "./mqtt_client/mqtt_client.h"

LocalConnectionManager localConnectionManager(ssid, pass);
MqttClient mqttClient(broker, port, localConnectionManager.getWifiClient());

Thermistor thermistor(SERIES_RESISTOR, NOMINAL_RESISTANCE, NOMINAL_TEMPERATURE, B_COEFFICIENT);

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
  mqttClient.loop();

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
