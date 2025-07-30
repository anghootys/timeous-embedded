#ifndef UNTITLED_ACCESS_POINT_WIFI_H
#define UNTITLED_ACCESS_POINT_WIFI_H

#include <ESP8266WiFi.h>

#define LOG_AP_WIFI_CTX "WIFI"

#define AP_DEFAULT_LOCAL_IP IPAddress(192, 168, 10, 1)
#define AP_DEFAULT_GATEWAY IPAddress(192, 168, 10, 1)
#define AP_DEFAULT_SUBNET IPAddress(255, 255, 255, 0)

class AccessPoint {
    char *ssid = nullptr;
    char *password = nullptr;

    IPAddress local_ip;
    IPAddress gateway;
    IPAddress subnet;
public:
    void initialize(char *ssid = "Timeous", char *password = "",
                    IPAddress local_ip = AP_DEFAULT_LOCAL_IP, IPAddress gateway = AP_DEFAULT_GATEWAY,
                    IPAddress subnet = AP_DEFAULT_SUBNET);

    char *get_wifi_credentials_as_string();
};

#endif // UNTITLED_ACCESS_POINT_WIFI_H
