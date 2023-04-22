#define LED_RED 6
#define LED_BLUE 3
#define POTENTIOMETER A0

void setup()
{
    pinMode(LED_RED, OUTPUT);
    analogWrite(LED_RED, 0);
    pinMode(LED_BLUE, OUTPUT);
    analogWrite(LED_BLUE, 0);
}

void loop()
{
    int value = analogRead(A0);
    value = min(max(0, value - 10), 1003); //1003 = 1023 - 10 - 10
    int lightRed = map(value, 0, 1003, 0, 255);
    int lightBlue = 255 - lightRed;
    analogWrite(LED_RED, lightRed);
    analogWrite(LED_BLUE, lightBlue);
}