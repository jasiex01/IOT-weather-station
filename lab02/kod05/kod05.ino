#define LED_RED 6

void setup()
{
    pinMode(LED_RED, OUTPUT);
}

void loop()
{
    analogWrite(LED_RED, 64);
    delay(1000);
    analogWrite(LED_RED, 255);
    delay(1000);
}
