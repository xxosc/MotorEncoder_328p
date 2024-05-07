#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>

uint8_t font[3][8] = {
    {
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
    },
    {
        0b10100,
        0b10100,
        0b10100,
        0b10100,
        0b10100,
        0b10100,
        0b10100,
        0b10100,
    },
    {
        0b10101,
        0b10101,
        0b10101,
        0b10101,
        0b10101,
        0b10101,
        0b10101,
        0b10101,
    },
};

void lcdbar_init(LiquidCrystal &lcd){
    lcd.createChar(1, font[0]);
    lcd.createChar(2, font[1]);
    lcd.createChar(3, font[2]);
}

/**
 * Max value = 16*3 = 48;
*/
void lcdbar_draw(LiquidCrystal &lcd, uint8_t col, uint8_t value){
    if (value > 48) return;
    lcd.setCursor(0, col);
    
    for (uint8_t i = 0; i < value / 3; i++) lcd.write(3);
    uint8_t m = value % 3;
    lcd.write(m ? m : 20);
    lcd.print("                ");
}