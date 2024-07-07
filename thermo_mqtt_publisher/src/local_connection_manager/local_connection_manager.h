#ifndef LOCAL_CONNECTION_MANAGER_H
#define LOCAL_CONNECTION_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

class LocalConnectionManager
{
public:
    LocalConnectionManager(const char *ssid, const char *pass);
    void connect();
    WiFiClient& getWifiClient();

private:
    const char *ssid;
    const char *pass;
    WiFiClient wifiClient;
};

#endif