#include "time.h"

class Time : public ITime, public ITimeProvider {
public:
    void initialize() override;

    char *getFullTime() override;
};

void
Time::initialize() {}

char *
Time::getFullTime() {
    return "";
}
