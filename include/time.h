#ifndef UNTITLED_TIME_H
#define UNTITLED_TIME_H

class ITimeProvider {
public:
    virtual char *getFullTime() = 0;
};

class ITime {
public:
    virtual void initialize() = 0;
};

#endif // UNTITLED_TIME_H
