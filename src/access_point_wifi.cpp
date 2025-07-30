#include "access_point_wifi.h"
#include <ESP8266WiFi.h>
#include "WString.h"
#include "logger.h"


LoggerFacade wifi_logger = LoggerFacade(Logger::setup(LOG_AP_WIFI_CTX));

void
AccessPoint::initialize(char *ssid, char *password, IPAddress local_ip, IPAddress gateway, IPAddress subnet) {
    this->ssid = ssid;
    this->password = password;
    this->local_ip = local_ip;
    this->gateway = gateway;
    this->subnet = subnet;

    wifi_logger.info("configuring wifi access point.");
    if (!WiFi.softAPConfig(this->local_ip, this->gateway, this->subnet)) {
        wifi_logger.error("configuring wifi access point failed.");

        // halt processor
        EspClass::deepSleep(0);
    }
    wifi_logger.success("configuring wifi access point successfully.");

    wifi_logger.info("starting wifi access point.");
    if (!WiFi.softAP(this->ssid, this->password)) {
        wifi_logger.error("starting wifi access point failed.");

        // halt processor
        EspClass::deepSleep(0);
    }
    wifi_logger.success("starting wifi access point successfully.");

    // log wifi credentials
    char *buf = this->get_wifi_credentials_as_string();
    wifi_logger.info(buf);
    free(buf);
}

char *
AccessPoint::get_wifi_credentials_as_string() {
    size_t buf_len = (strlen(ssid) + strlen(password) + strlen(local_ip.toString().c_str())
                      + strlen(gateway.toString().c_str()) + strlen(subnet.toString().c_str()) + 47 + 20)
                     * sizeof(char);
    char *buf = (char *) malloc(buf_len);
    memset(buf, '\0', buf_len);
    snprintf(buf, buf_len, "ssid:\t%s\npassword:\t%s\nlocal ip:\t%s\ngateway:\t%s\nsubnet:\t\t%s", this->ssid, this->password,
             this->local_ip.toString().c_str(), this->gateway.toString().c_str(), this->subnet.toString().c_str());

    return buf;
}
