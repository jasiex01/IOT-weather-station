#include <util/atomic.h>
#include <LiquidCrystal_I2C.h>

#define GREEN_BUTTON 4

#define DEBOUNCING_PERIOD 100

LiquidCrystal_I2C lcd(0x27, 16, 2);

int pressCounter = 0;
volatile unsigned long buttonTimestamp = 0UL;
unsigned long previousButtonTimestamp = 0UL;
unsigned long elapsedStartTimestamp = 0UL;

ISR(PCINT2_vect)
{
    buttonTimestamp = millis();
}

void printResults(int count, unsigned long time)
{
    char buffer[40];
    sprintf(buffer, "Press count%5d", count);
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "Time [ms]%7lu", time);
    //sprintf(buffer, "Time [s]%4lu.%03d", time / 1000, time % 1000);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void setup()
{
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    printResults(0, 0UL);

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT20);
}

void loop()
{
    unsigned long localbuttonTimestamp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        localbuttonTimestamp = buttonTimestamp;
    }

    if (localbuttonTimestamp != previousButtonTimestamp && millis() > localbuttonTimestamp + DEBOUNCING_PERIOD)
    {
        if (digitalRead(GREEN_BUTTON) == LOW)
        {
            pressCounter++;

            unsigned long elapsedTime = localbuttonTimestamp - elapsedStartTimestamp;
            elapsedStartTimestamp = localbuttonTimestamp;

            printResults(pressCounter, elapsedTime);
        }
        previousButtonTimestamp = localbuttonTimestamp;
    }
}