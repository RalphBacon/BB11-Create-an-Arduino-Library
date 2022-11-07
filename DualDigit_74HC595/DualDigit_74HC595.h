#pragma once
#include "Arduino.h"

// Simple library class to control a two digit, 74HC595 controlled 7-segment display
class DualDigit_74HC595
{

public: // These are the "functions" exposed to the consumer of this library
    enum class commonLedType : uint8_t
    {
        ANODE = 0,
        CATHODE = 1
    };

    // Constructor (we could allocate the pins here)
    DualDigit_74HC595(commonLedType ledType);

    // Set some values (yes, could be done in the constructor)
    void begin(int dataPin, int clockPin, int latchPin);

    // We can either update a specific digit (1 or 2)...
    int updateChar(int charPos, uint8_t value, bool withDP = false);

    // ... or output a value that (potentially) occupies both digits
    int updateValue(uint8_t value, bool withDP = false, bool isHex = false);

    // And send that data to the 74HC595 and latch (show) it (optionally)
    void sendOut(bool showValue = true);

    // Flash current value to draw attention
    void flashDigits(uint8_t secs = 3);

    // Blank the output
    void blankDigits();

private: // Only visible from within this class

    // Two byte buffer for our incoming data
    uint8_t _buffer[2];

    // Keep a record of which pins we're using
    int _dataPin;
    int _clkPin;
    int _latchPin;

    // Track the type of display the engineer has
    DualDigit_74HC595::commonLedType _ledType;

    // Define which segments the 7-seg display needs to display each number 0 - 9 and A - F
    // These are defined for a COMMON CATHODE type display (1=ON, 0=OFF).
    // The library will auto-invert these for COMMON ANODE types (1=OFF, 0=ON).
    // COMMON CATHODE
    uint8_t _digits[17] = {
        // abcdefgx
        0b11111100, // 0 ie segments abcdef but not g nor the dp (x)
        0b01100000, // 1
        0b11011010, // 2
        0b11110010, // 3
        0b01100110, // 4
        0b10110110, // 5
        0b10111110, // 6
        0b11100000, // 7
        0b11111110, // 8
        0b11110110, // 9
        0b11101110, // A 10
        0b00111110, // b 11
        0b10011100, // C 12
        0b01111010, // d 13
        0b10011110, // E 14
        0b10001110, // F 15
        0b00000000, // Blank 16
    };

protected: // Not exposed publicly. Can be used within this class AND used in inherited classes
           // Hmm Nothing springs to mind that should go here instead of one of the above sections
};