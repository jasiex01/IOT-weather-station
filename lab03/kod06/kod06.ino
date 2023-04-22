#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte smile[8] = {
    B10101,
    B00000,
    B00000,
    B10001,
    B00000,
    B00000,
    B10001,
    B01110,
};

void setup()
{
    lcd.init();
    lcd.backlight();

    lcd.createChar(0, smile);

    lcd.setCursor(0, 0);
    lcd.print("Hello");
    lcd.setCursor(3, 1);
    lcd.print("World!");

    lcd.setCursor(15, 0);
    lcd.write(byte(0));
}

void loop()
{
}