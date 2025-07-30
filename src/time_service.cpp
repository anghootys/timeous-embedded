#include "time_service.h"
#include <Arduino.h>
#include <WString.h>
#include "RTClib.h"
#include "core.h"

#define LOG_TIME_CTX "TIME"

LoggerFacade time_logger = LoggerFacade(Logger::setup(LOG_TIME_CTX));

void
Time::update_time(const String *iso8601) {
    if (abs((long long)this->rtc.now().secondstime() - (long long)DateTime(iso8601->c_str()).secondstime()) > 5 * 60) {
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
}

char *
Time::get_full_time() {
    auto now = this->rtc.now();
    auto buf = alloc<char>(25);

    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

    return buf;
}
