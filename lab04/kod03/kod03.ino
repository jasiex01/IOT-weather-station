#define LED_RED 6

void setup()
{
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);
    Serial.begin(9600);
    while (!Serial)
    { /* just wait */
    }
}

void loop()
{
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');
        command.trim();
        command.toLowerCase();

        if (command == "on")
        {
            digitalWrite(LED_RED, HIGH);
            Serial.println("LED ON");
        }
        else if (command == "off")
        {
            digitalWrite(LED_RED, LOW);
            Serial.println("LED OFF");
        }
        else
        {
            Serial.println(String("Unknown command '") + command + "'");
        }
    }
}
