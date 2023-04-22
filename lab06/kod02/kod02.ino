#include "LedBlinker.h"

#define LED_RED 6
#define LED_BLUE 3

LedBlinker redBlinker;
LedBlinker blueBlinker;

void setup()
{
    redBlinker.init(LED_RED, 1000, 2000);
    blueBlinker.init(LED_BLUE, 1000, 3000);
}

void loop()
{
    redBlinker.runMeInLoop();
    blueBlinker.runMeInLoop();

    /*YOUR CODE*/
}