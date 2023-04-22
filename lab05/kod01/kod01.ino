#include <LiquidCrystal_I2C.h>

#define RED_BUTTON 2
#define DEBOUNCING_PERIOD 100

LiquidCrystal_I2C lcd(0x27, 16, 2);

int pressCounter = 0;
volatile unsigned long buttonTimestamp = 0UL;
unsigned long previousButtonTimestamp = 0UL;
unsigned long elapsedStartTimestamp = 0UL;

void interruptAction()
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
    pinMode(RED_BUTTON, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    printResults(0, 0UL);

    attachInterrupt(digitalPinToInterrupt(RED_BUTTON), interruptAction, FALLING);
}

void loop()
{
    noInterrupts();
    unsigned long localButtonTimestamp = buttonTimestamp;
    interrupts();

    if (localButtonTimestamp != previousButtonTimestamp && millis() > localButtonTimestamp + DEBOUNCING_PERIOD)
    {
        if (digitalRead(RED_BUTTON) == LOW)
        {
            pressCounter++;

            unsigned long elapsedTime = localButtonTimestamp - elapsedStartTimestamp;
            elapsedStartTimestamp = localButtonTimestamp;

            printResults(pressCounter, elapsedTime);
        }
        previousButtonTimestamp = localButtonTimestamp;
    }
}