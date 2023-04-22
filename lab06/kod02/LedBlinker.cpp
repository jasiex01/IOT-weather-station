#include "LedBlinker.h"

void LedBlinker::init(byte led, Time onPeriod, Time offPeriod)
{
    led_ = led;
    onPeriod_ = onPeriod;
    offPeriod_ = offPeriod;
    lastChangeTime_ = 0;
    ledState_ = LOW;
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
}

void LedBlinker::runMeInLoop()
{
    Time time = millis();
    if (ledState_ == LOW)
    {
        if (time >= lastChangeTime_ + offPeriod_)
        {
            ledState_ = HIGH;
            digitalWrite(led_, HIGH);
            lastChangeTime_ = time;
        }
    }
    else
    {
        if (time >= lastChangeTime_ + onPeriod_)
        {
            ledState_ = LOW;
            digitalWrite(led_, LOW);
            lastChangeTime_ = time;
        }
    }
}
