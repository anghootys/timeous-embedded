#include "core.h"

LoggerFacade core_logger(Logger::setup(LOG_CORE_CTX));

void
halt_processor() {
    EspClass::deepSleep(0);
}

void
reset_processor() {
    core_logger.warn("RESETTING IN 5 SECONDS.");

    // delay for the sake of latest logs to read
    delay(5000);
    EspClass::reset();
}

