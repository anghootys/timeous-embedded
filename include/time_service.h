#ifndef __TIME_SERVICE_H__
#define __TIME_SERVICE_H__

#include <RTClib.h>
#include "logger.h"

#define LOG_TIME_CTX "TIME"

class Time {
    RTC_DS3231 rtc;

public:
    void initialize();

    char *get_full_time();

    void update_time(const String *iso8601);

    static Time &
    instance() {
        static Time time;
        return time;
    }
};


#endif // __TIME_SERVICE_H__
