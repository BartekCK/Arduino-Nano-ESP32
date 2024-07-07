#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

#define THERMISTOR_PIN A0
#define SERIES_RESISTOR 10000    // 10kΩ resistor
#define NOMINAL_RESISTANCE 10000 // 10kΩ nominal resistance of the thermistor
#define NOMINAL_TEMPERATURE 25   // nominal temperature (25°C)
#define B_COEFFICIENT 3950       // B coefficient of the thermistor

char ssid[] = SECRET_SSID; // network SSID
char pass[] = SECRET_PASS; // network password

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char broker[] = "192.168.55.104";
int port = 1883;
const char topic[] = "temperature-topic";

// set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

void publishMessage(const char *message);

void setup()
{
  Serial.begin(9600);

  // while (!Serial) {
  //; // wait for serial port to connect. Needed for native USB port only
  // }

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass) != WL_CONNECTED;

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println("You're connected to the network");
  Serial.println();

  mqttClient.setServer(broker, port);

  if (!mqttClient.connect("ESP32Client"))
  {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.state());
    while (1)
      ;
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop()
{
  if (!mqttClient.connected())
  {
    Serial.println("Disconnected from MQTT broker. Reconnecting...");
    if (mqttClient.connect("ESP32Client"))
    {
      Serial.println("Reconnected to MQTT broker");
    }
    else
    {
      Serial.print("Failed to reconnect. Error code = ");
      Serial.println(mqttClient.state());
      delay(5000); // Wait 5 seconds before retrying
    }
  }

  mqttClient.loop(); // Maintain MQTT connection

  int rawValue = analogRead(THERMISTOR_PIN);
  float voltage = rawValue * (3.3 / 4095.0); // convert value into voltage
  float resistance = SERIES_RESISTOR * (3.3 / voltage - 1);

  // Steinharta-Hart
  float steinhart;
  steinhart = resistance / NOMINAL_RESISTANCE;       // (R/Ro)
  steinhart = log(steinhart);                        // ln(R/Ro)
  steinhart /= B_COEFFICIENT;                        // 1/B * ln(R/Ro)
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                       // Inwersja
  steinhart -= 273.15;                               // To celcious

  Serial.print("Raw Value: ");
  Serial.print(rawValue);
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  Serial.print("V Resistance: ");
  Serial.print(resistance);
  Serial.print(" ohms Temperature: ");
  Serial.print(steinhart);
  Serial.println(" *C");

  char tempStr[16];
  dtostrf(steinhart, 6, 2, tempStr);
  publishMessage(tempStr);

  delay(1000);
}

void publishMessage(const char *message)
{
  if (mqttClient.publish(topic, message, true))
  {
    Serial.print("Message published: ");
    Serial.println(message);
  }
  else
  {
    Serial.println("Message publish failed");
  }
}