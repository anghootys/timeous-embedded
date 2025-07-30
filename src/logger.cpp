#include "logger.h"
#include <Arduino.h>
#include "time.h"

class Logger final : public ILogger {
    size_t baudRate = 115200;

    ITimeProvider *timeProvider = nullptr;

    void log(LoggerConfig *conf, char *identifier, char *msg);

public:
    Logger() = default;
    Logger(size_t baudRate) : baudRate(baudRate) {}

    void initialize() override;

    void setTimeProvider(ITimeProvider *timeProvider);

    void success(LoggerConfig *conf, char *msg) override;
    void info(LoggerConfig *conf, char *msg) override;
    void warn(LoggerConfig *conf, char *msg) override;
    void error(LoggerConfig *conf, char *msg) override;
};

void
Logger::initialize() {
    Serial.begin(this->baudRate);
}

void
Logger::setTimeProvider(ITimeProvider *timeProvider) {
    this->timeProvider = timeProvider;
}

void
Logger::log(LoggerConfig *conf, char *identifier, char *msg) {
    Serial.printf("[%s] %s %s : %s", this->timeProvider->getFullTime(), identifier, conf->source, msg);
}

void
Logger::success(LoggerConfig *conf, char *msg) {}

void
Logger::info(LoggerConfig *conf, char *msg) {
    this->log(conf, "INF", msg);
}

void
Logger::warn(LoggerConfig *conf, char *msg) {}

void
Logger::error(LoggerConfig *conf, char *msg) {}
