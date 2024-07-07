#include "mqtt_client.h"

MqttClient::MqttClient(const char *broker, int mqttPort, WiFiClient &wifiClient)
{
    this->broker = broker;
    this->mqttPort = mqttPort;
    this->pubsubClient = PubSubClient(wifiClient);
}

void MqttClient::connect()
{
    Serial.print("Attempting to connect to MQTT broker: ");
    Serial.println(this->broker);

    this->pubsubClient.setServer(this->broker, this->mqttPort);

    while (!this->pubsubClient.connected())
    {
        Serial.print(F("Attempting MQTT connection..."));
        if (this->pubsubClient.connect("ESP32Client"))
        {
            Serial.println("You're connected to the MQTT broker!");
            Serial.println();
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(this->pubsubClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MqttClient::publish(const char *topic, const char *payload)
{
    if (this->pubsubClient.publish(topic, payload))
    {
        Serial.print("Message published: ");
        Serial.println(payload);
    }
    else
    {
        Serial.println("Message publish failed");
    }
}

void MqttClient::loop()
{
    if (!this->pubsubClient.connected())
    {
        Serial.println("Disconnected from MQTT broker. Reconnecting...");
        if (this->pubsubClient.connect("ESP32Client"))
        {
            Serial.println("Reconnected to MQTT broker");
        }
        else
        {
            Serial.print("Failed to reconnect. Error code = ");
            Serial.print(this->pubsubClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000); // Wait 5 seconds before retrying
        }
    }

    this->pubsubClient.loop(); // Maintain MQTT connection
}