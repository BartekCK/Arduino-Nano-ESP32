#include "local_connection_manager.h"

LocalConnectionManager::LocalConnectionManager(const char *ssid, const char *pass)
    : ssid(ssid), pass(pass), wifiClient()
{
}

void LocalConnectionManager::connect()
{
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(this->ssid);

    WiFi.begin(this->ssid, this->pass) != WL_CONNECTED;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(F("."));
    }

    Serial.println("You're connected to the network");
    Serial.println();
}

WiFiClient& LocalConnectionManager::getWifiClient()
{
    return this->wifiClient;
}