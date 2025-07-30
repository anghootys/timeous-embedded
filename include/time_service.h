#ifndef UNTITLED_TIME_SERVICE_H
#define UNTITLED_TIME_SERVICE_H

#include <RTClib.h>
#include "logger.h"
#include "time_service.h"

#define LOG_TIME_CTX "TIME"

class ITime {
public:
    virtual const char *getFullTime() = 0;
};

class TimeService : public ITime {
    RTC_DS3231 rtc;

public:
    void initialize();

    const char *getFullTime() override;

    static TimeService &
    instance() {
        static TimeService time;
        return time;
    }
};


#endif // UNTITLED_TIME_SERVICE_H
