Motor Encoder 
=============

Use BLDC motor (e.g. from HDD) as rotary encoder with Arduino.

> Library uses internal analog comparator. No external components needed. 

Hardware
--------

- Arduino Uno (Nano)
- BLDC Motor with 4 pins (Y configuration) 
- 16x2 LCD (Optional)

Connection
----------

    Motor  ---->  Arduino

        GND ---->  3V3 & D6
        L1  ---->  A0
        L2  ---->  A1
        L3  ---->  A2

    LCD  ------>  Arduino

        RS  ---->  D9  
        E   ---->  D8
        D4  ---->  D5  
        D5  ---->  D4
        D6  ---->  D3
        D7  ---->  D2

Dependencies
------------

- Arduino's LiquidCrystal Library (optional)