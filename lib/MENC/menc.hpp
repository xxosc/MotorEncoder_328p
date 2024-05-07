/**
 * MENC (Motor ENCoder) LIBRARY
 * 
 * Made by xxosc
*/

#pragma once

#include <Arduino.h>

/*****************************************************************************/

typedef enum {
    MENC_MODE_LINEAR,
    MENC_MODE_LOGARITHMIC,
} menc_mode_t;

uint8_t menc_pin_L1 = 0;
uint8_t menc_pin_L2 = 1;
uint8_t menc_pin_L3 = 2;

menc_mode_t menc_mode = MENC_MODE_LINEAR;

void menc_init(uint8_t L1, uint8_t L2, uint8_t L3, menc_mode_t mode = MENC_MODE_LINEAR) {
    menc_mode = mode;

    menc_pin_L1 = L1 & 0x7;
    menc_pin_L2 = L2 & 0x7;
    menc_pin_L3 = L3 & 0x7;

    // Set AIN0 (D6) pin as INPUT (positive input of comparator)
    DDRD &= ~(1 << DDD6);

    // Set L1, L2, L3 pins as inputs
    DDRC &= ~(1 << menc_pin_L1);
    DDRC &= ~(1 << menc_pin_L2);
    DDRC &= ~(1 << menc_pin_L3);

    // Switch off ADC
    ADCSRA &= ~(1 << ADEN);

    // Connect negative input of comparator to multiplexer
    ADCSRB |= (1 << ACME);
}

// Switch analog multiplexer
inline void menc_set_mux(uint8_t x) { ADMUX = (ADMUX & 0xF0) | (x & 0x0F); }

uint8_t menc_compare() {
    uint8_t result = 0;
    menc_set_mux(menc_pin_L2);
    delayMicroseconds(30);
    if (bit_is_set(ACSR, ACO)) result |= 2;
    menc_set_mux(menc_pin_L3);
    delayMicroseconds(30);
    if (bit_is_set(ACSR, ACO)) result |= 4;
    menc_set_mux(menc_pin_L1);
    delayMicroseconds(30);
    if (bit_is_set(ACSR, ACO)) result |= 1;
    return result;
}

uint32_t timestamp = 0;

int menc_get_direction() {
    static uint16_t last4 = 0;
    uint8_t q = menc_compare();
    if (!q || q == (last4 & 0xF)) return 0;
    last4 = (last4 << 4) | (q & 0xF);

    uint32_t delta_time = millis() - timestamp;
    timestamp = millis();

    static int k = 1;

    if (menc_mode == MENC_MODE_LOGARITHMIC) {
        if (delta_time <= 2) k = 1; else // Discard noice when rotating slowly 
        k = max(1, (1600 / (delta_time * delta_time)));
    }

    // check last 3 values
    switch (last4 & 0xFFF) {
        // CCW: 1 -> 3 -> 2 -> 6 -> 4 -> 5
        case 0x124:

#if MENC_EN_MIDVAL
        case 0x136:
        case 0x364:
        case 0x265:
        case 0x651:
        case 0x453:
        case 0x532:

        case 0x126:
        case 0x324:
        case 0x245:
        case 0x641:
        case 0x413:
        case 0x512:
#endif

        case 0x132:
        case 0x326:
        case 0x264:
        case 0x645:
        case 0x451:
            return -1 * k;

        //  CW: 1 -> 5 -> 4 -> 6 -> 2 -> 3
        case 0x142:

#if MENC_EN_MIDVAL
        case 0x156:
        case 0x562:
        case 0x463:
        case 0x631:
        case 0x235:
        case 0x354:

        case 0x146:
        case 0x542:
        case 0x423:
        case 0x621:
        case 0x215:
        case 0x314:
#endif

        case 0x154:
        case 0x546:
        case 0x462:
        case 0x623:
        case 0x231:
            return 1 * k;

        default:
            return 0;
    }
}

/*****************************************************************************/
// xxosc 2024