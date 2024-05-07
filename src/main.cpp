#include <Arduino.h>
#include <LiquidCrystal.h>
#include "menc.hpp"
#include "lcd_bar.hpp"

LiquidCrystal lcd(9, 8, 5, 4, 3, 2);

void setup() {
    Serial.begin(115200);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print(" MOTOR ENCODER  ");
    lcd.setCursor(7, 1);
    lcd.print(0);
    lcdbar_init(lcd);
    menc_init(0, 1, 2, MENC_MODE_LINEAR);
}

int raw = 0;
int val = 0;

void loop() {
    if (int x = menc_get_direction()){
        Serial.print("> x = ");
        Serial.println(x);
        raw += x;
        val = raw / 2;
        lcdbar_draw(lcd, 0, val);
        lcd.setCursor(7, 1);
        lcd.print(val);
        lcd.print("        ");
    }
}
