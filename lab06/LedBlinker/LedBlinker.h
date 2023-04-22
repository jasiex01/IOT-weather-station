#ifndef LEDBLINKER_H_
#define LEDBLINKER_H_

#include "Arduino.h"

typedef unsigned long Time;

class LedBlinker
{
public:
    void init(byte led, Time onPeriod, Time offPeriod);
    void runMeInLoop();

private:
    byte led_;
    Time onPeriod_;
    Time offPeriod_;
    Time lastChangeTime_;
    byte ledState_;
};

#endif