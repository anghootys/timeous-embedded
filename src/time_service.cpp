#include "time_service.h"
#include <Arduino.h>
#include <WString.h>
#include "RTClib.h"
#include "core.h"

#define LOG_TIME_CTX "TIME"

LoggerFacade time_logger = LoggerFacade(Logger::setup(LOG_TIME_CTX));

void
Time::update_time(const String *iso8601) {
    if (abs((long long) this->rtc.now().secondstime() - (long long) DateTime(iso8601->c_str()).secondstime())
        > 5 * 60) {
        time_logger.info("updating time to:");
        time_logger.info(iso8601->c_str());
        this->rtc.adjust(DateTime(iso8601->c_str()));
    }
}

void
Time::initialize() {
    if (!this->rtc.begin()) {
        time_logger.error("could not begin RTC.");

        // halt processor
        EspClass::deepSleep(0);
    }

    if (this->rtc.lostPower()) {
        time_logger.warn("RTC lost power, let's set the time!");
    }

    this->rtc.clearAlarm(1);
    this->rtc.clearAlarm(2);
    this->rtc.disableAlarm(1);
    this->rtc.disableAlarm(2);
    this->rtc.writeSqwPinMode(DS3231_OFF);
}

char *
Time::get_full_time() {
    auto now = this->rtc.now();
    auto buf = alloc<char>(25);

    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(),
            now.second());

    return buf;
}

char *
Time::convert_time_string_to_scheduler_mem_data(const char *time_str) {
    auto *s = new SchedulerTimeMemScheme{ 0 };

    sscanf(time_str, "%d:%d", &s->hour, &s->minute);

    if (s->hour < 0 || s->hour > 23 || s->minute < 0 || s->minute > 59) {
        time_logger.error("hour or minute is in a wrong format.");
        time_logger.error("input time_str:");
        time_logger.error(time_str);
        time_logger.error("hour:");
        time_logger.error(String(s->hour).c_str());
        time_logger.error("minute:");
        time_logger.error(String(s->minute).c_str());

        delete s;
        return nullptr;
    }

    return (char *) s;
}

bool
Time::is_scheduler_timer_period_came(SchedulerTimeMemScheme &s) {
    auto now = this->rtc.now();
    return s.hour == now.hour() && s.minute - now.minute() <= 0 && s.minute - now.minute() >= -5;
}

void
Time::setup_1H_timer() {
    time_logger.info("setting up timer 1 for 1hour.");
    auto alarm_timer = this->rtc.now() + TimeSpan(5 * 60);
    this->rtc.setAlarm2(alarm_timer, DS3231_A2_Minute);
    time_logger.info("timer 2 setup for 1hour successfully.");
}

bool
Time::is_timer_fired(int timer_num) {
    return this->rtc.alarmFired(timer_num);
}

void
Time::clear_timer(int timer_num) {
    this->rtc.clearAlarm(timer_num);
}

SchedulerTimeMemScheme
Time::convert_scheduler_mem_data_to_timer_scheme(const char *mem_data) {
    SchedulerTimeMemScheme m = { 0 };
    m.hour = (unsigned char) mem_data[0];
    m.minute = (unsigned char) mem_data[sizeof(int)];

    return m;
}
