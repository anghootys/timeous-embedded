#include "logger.h"
#include <Arduino.h>

#include <utility>

void
Logger::initialize(baud_rate_t _baud_rate) {
    this->baud_rate = _baud_rate;
    Serial.begin(this->baud_rate);
}

void
Logger::log(LoggerConfig *conf, char identifier, const char *msg) {
    String time;
    if (!this->time_provider) {
        time = "NO TIME";
    } else {
        time = String(this->time_provider());
    }

    Serial.printf("[%s] %c %s : %s\n", time.c_str(), identifier, conf->source, msg);
    Serial.flush();
}

void
Logger::success(LoggerConfig *conf, const char *msg) {
    this->log(conf, LOG_IDENTIFIER_SUCCESS, msg);
}

void
Logger::info(LoggerConfig *conf, const char *msg) {
    this->log(conf, LOG_IDENTIFIER_INFO, msg);
}

void
Logger::warn(LoggerConfig *conf, const char *msg) {
    this->log(conf, LOG_IDENTIFIER_WARN, msg);
}

void
Logger::error(LoggerConfig *conf, const char *msg) {
    this->log(conf, LOG_IDENTIFIER_ERROR, msg);
}
void
Logger::set_time_provider(std::function<const char *(void)> time_provider_cb) {
    this->time_provider = std::move(time_provider_cb);
}

void
LoggerFacade::success(const char *msg) {
    Logger::instance().success(this->logger_config, msg);
}

void
LoggerFacade::info(const char *msg) {
    Logger::instance().info(this->logger_config, msg);
}

void
LoggerFacade::warn(const char *msg) {
    Logger::instance().warn(this->logger_config, msg);
}

void
LoggerFacade::error(const char *msg) {
    Logger::instance().error(this->logger_config, msg);
}
