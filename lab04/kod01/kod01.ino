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
    Serial.print("Hello World! ");
    Serial.println(i++, HEX);
}
