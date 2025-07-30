#include <WString.h>
#include <stdio.h>
#include "RTClib.h"
#include "time_service.h"

#define LOG_TIME_CTX "TIME"

LoggerFacade time_logger = LoggerFacade(Logger::setup(LOG_TIME_CTX));

void
TimeService::initialize() {
    if (!this->rtc.begin()) {
        time_logger.error("could not begin RTC.");

        // halt processor
        EspClass::deepSleep(0);
    }

    if (this->rtc.lostPower()) {
        time_logger.warn("RTC lost power, let's set the time!");
    }
}

const char *
TimeService::getFullTime() {
    DateTime now = rtc.now();
    char *time = (char *) malloc(25 * sizeof(char));

    if (!time) {
        time_logger.error("Could not allocate memory for time.");

        // halt processor
        EspClass::deepSleep(0);
    }

    sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(),
            now.second());

    return time;
}
