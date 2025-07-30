#ifndef LOGGER_H
#define LOGGER_H

#include <stddef.h>
#include "time_service.h"

#define DEFAULT_SERIAL_BAUD_RATE 115200

typedef unsigned int baud_rate_t;

#define LOG_IDENTIFIER_INFO 'i'
#define LOG_IDENTIFIER_SUCCESS '+'
#define LOG_IDENTIFIER_WARN '~'
#define LOG_IDENTIFIER_ERROR '!'


typedef struct {
    const char *source;
} LoggerConfig;

class ILogger {
public:
    ILogger() = default;
    virtual ~ILogger() = default;

    virtual void success(LoggerConfig *conf, const char *msg) = 0;
    virtual void info(LoggerConfig *conf, const char *msg) = 0;
    virtual void warn(LoggerConfig *conf, const char *msg) = 0;
    virtual void error(LoggerConfig *conf, const char *msg) = 0;

    static LoggerConfig *
    setup(const char *source) {
        const auto config = new LoggerConfig();

        config->source = source;

        return config;
    }
};

class Logger : public ILogger {
    baud_rate_t baud_rate = DEFAULT_SERIAL_BAUD_RATE;

    void log(LoggerConfig *conf, char identifier, const char *msg);

    std::function<const char *(void)> time_provider = nullptr;

public:
    Logger() = default;

    void initialize(baud_rate_t baud_rate = DEFAULT_SERIAL_BAUD_RATE);

    void set_time_provider(std::function<const char *(void)> time_provider);

    void success(LoggerConfig *conf, const char *msg) override;
    void info(LoggerConfig *conf, const char *msg) override;
    void warn(LoggerConfig *conf, const char *msg) override;
    void error(LoggerConfig *conf, const char *msg) override;

    static Logger &
    instance() {
        static Logger logger;
        return logger;
    }
};

class LoggerFacade {
    LoggerConfig *logger_config;

public:
    LoggerFacade(LoggerConfig *logger_config) : logger_config(logger_config) {}

    void success(const char *msg);
    void info(const char *msg);
    void warn(const char *msg);
    void error(const char *msg);
};

#endif // LOGGER_H
