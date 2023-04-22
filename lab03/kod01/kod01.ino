#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

int led[] = {LED_RED, LED_GREEN, LED_BLUE};

void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, HIGH);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, LOW);
}

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

bool isGreenButtonPressed()
{
    static int previous_reading = HIGH;
    int current_reading = digitalRead(GREEN_BUTTON);

    bool isPressed = false;
    if (previous_reading == HIGH && current_reading == LOW)
    {
        isPressed = true;
    }

    if (previous_reading != current_reading)
    {
        previous_reading = current_reading;
    }

    return isPressed;
}

void setup()
{
    initRGB();
    initButtons();
}

int led_index = 0;
void loop()
{
    if (isGreenButtonPressed())
    {
        digitalWrite(led[led_index], LOW);
        led_index = ++led_index % 3;
        digitalWrite(led[led_index], HIGH);
    }
}
