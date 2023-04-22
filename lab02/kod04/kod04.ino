#define LED_RED 6

void setup()
{
    pinMode(LED_RED, OUTPUT);
    analogWrite(LED_RED, 64);
}

void loop()
{
}
