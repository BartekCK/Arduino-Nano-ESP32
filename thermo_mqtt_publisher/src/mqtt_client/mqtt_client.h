#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

class MqttClient
{

public:
    MqttClient(const char *broker, int port, WiFiClient &wifiClient);
    void connect();
    void publish(const char *topic, const char *payload);
    void loop();

private:
    PubSubClient pubsubClient;
    const char *broker;
    int mqttPort;
};

#endif