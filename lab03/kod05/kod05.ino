#define LED_RED 6
#define RED_BUTTON 2

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);

    pinMode(RED_BUTTON, INPUT_PULLUP);
}

void blinkBuiltinLed()
{
    const unsigned long BlinkChangePeriod = 1000UL;
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;

    if (millis() - lastBlinkChange >= BlinkChangePeriod)
    {
        if (ledState == HIGH)
        {
            ledState = LOW;
        }
        else
        {
            ledState = HIGH;
        }

        digitalWrite(LED_BUILTIN, ledState);
        lastBlinkChange += BlinkChangePeriod;
    }
}

void readButtonSetLed()
{
    if (digitalRead(RED_BUTTON) == LOW)
        digitalWrite(LED_RED, HIGH);
    else
        digitalWrite(LED_RED, LOW);
}

void loop()
{
    blinkBuiltinLed();
    readButtonSetLed();
}
