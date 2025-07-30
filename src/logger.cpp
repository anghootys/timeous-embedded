#include "logger.h"
#include <Arduino.h>

class Logger final : public ILogger {
    size_t baudRate = 115200;
public:
    Logger() = default;
    Logger(size_t baudRate): baudRate(baudRate) {}

    void initialize() override {
        Serial.begin(this->baudRate);
    }

    void success(LoggerConfig *conf, char *msg) override;
    void info(LoggerConfig *conf, char *msg) override;
    void warn(LoggerConfig *conf, char *msg) override;
    void error(LoggerConfig *conf, char *msg) override;
};
void
Logger::success(LoggerConfig *conf, char *msg) {}

void
Logger::info(LoggerConfig *conf, char *msg) {}

void
Logger::warn(LoggerConfig *conf, char *msg) {}

void
Logger::error(LoggerConfig *conf, char *msg) {}
