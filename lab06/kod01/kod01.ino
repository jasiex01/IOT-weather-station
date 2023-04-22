#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

OneWire oneWire(A1);
DallasTemperature tempSensors(&oneWire);

void setup()
{
    tempSensors.begin();

    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void loop()
{
    tempSensors.requestTemperatures();
    float tempIn = tempSensors.getTempCByIndex(1);
    float tempOut = tempSensors.getTempCByIndex(0);

    char buffer[40];
    sprintf(buffer, "Temp  IN%8s", String(tempIn, 4).c_str());
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "Temp OUT%8s", String(tempOut, 4).c_str());
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}