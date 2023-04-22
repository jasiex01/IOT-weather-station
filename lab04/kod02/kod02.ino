void setup()
{
    Serial.begin(9600);
    while (!Serial)
    { /* just wait */
    }
}

int i = 0;
void loop()
{
    double x = TWO_PI * i / 20;
    String str = String(sin(x)) + " " + String(2 * cos(x));
    Serial.println(str);
    i++;
}
