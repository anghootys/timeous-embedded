#ifndef LOGGER_H
#define LOGGER_H

#define BAUD_RATE_115200 115200
#include <HardwareSerial.h>

typedef struct {
    char *source;
} LoggerConfig;

class ILogger {
public:
    ILogger() = default;
    virtual ~ILogger() = default;

    virtual void initialize();

    virtual void success(LoggerConfig *conf, char *msg) = 0;
    virtual void info(LoggerConfig *conf, char *msg) = 0;
    virtual void warn(LoggerConfig *conf, char *msg) = 0;
    virtual void error(LoggerConfig *conf, char *msg) = 0;

    static LoggerConfig*
    setup(char *source) {
        const auto config = new LoggerConfig();

        config->source = source;

        return config;
    }
};

#endif // LOGGER_H
