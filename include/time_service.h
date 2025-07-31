#ifndef __TIME_SERVICE_H__
#define __TIME_SERVICE_H__

#include <RTClib.h>
#include "logger.h"

#define LOG_TIME_CTX "TIME"

typedef struct {
    int hour;
    int minute;
} SchedulerTimeMemScheme;

class Time {
    RTC_DS3231 rtc;

public:
    void initialize();

    char *get_full_time();

    void update_time(const String *iso8601);

    void setup_1H_timer();
    bool is_timer_fired(int timer_num);
    void clear_timer(int timer_num);

    bool is_scheduler_timer_period_came(SchedulerTimeMemScheme &s);

    static Time &
    instance() {
        static Time time;
        return time;
    }

    static char *convert_time_string_to_scheduler_mem_data(const char *time_str);

    static SchedulerTimeMemScheme convert_scheduler_mem_data_to_timer_scheme(const char *mem_data);

};


#endif // __TIME_SERVICE_H__
