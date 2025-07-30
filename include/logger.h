#ifndef LOGGER_H
#define LOGGER_H

typedef struct {
    char *source;
    TimeService *timeService;
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
    setup(char *source, TimeService *timeService) {
        const auto config = new LoggerConfig();

        config->source = source;
        config->timeService = timeService;

        return config;
    }
};

#endif // LOGGER_H
