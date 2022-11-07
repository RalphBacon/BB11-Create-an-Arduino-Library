#include "Arduino.h"
#include "DualDigit_74HC595.h"
#include <string.h>
// Where you refer to the class you use the 'scope resolution operator' of a double colon

// Default ctor
DualDigit_74HC595::DualDigit_74HC595(commonLedType ledType)
{
    // Allocate a two-byte buffer to contain the digits that we send to the 74HC595
    memset(_buffer, '0', 2);

    // Make a note of the type of 7-seg LEDs we are dealing with
    _ledType = ledType;
}

// Initialise the object with user-supplied data/clock/latch pins
void DualDigit_74HC595::begin(int dataPin, int clockPin, int latchPin)
{
    // Store the pin number within the object for future use
    DualDigit_74HC595::_dataPin = dataPin;
    DualDigit_74HC595::_clkPin = clockPin;
    DualDigit_74HC595::_latchPin = latchPin;

    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);

    // If we have a COMMON ANODE LED display, invert the bit pattern for each number (eg 0b10001111 becomes 0b01110000)
    if (_ledType == commonLedType::ANODE)
    {
        for (auto cnt = 0; (unsigned int)cnt < sizeof(_digits); cnt++)
        {
            _digits[cnt] = ~_digits[cnt];
        }
    }

    // Clear the output
    DualDigit_74HC595::sendOut(true);
}

// Update the nth character in the buffer
int DualDigit_74HC595::updateChar(int charPos, uint8_t charValue, bool withDP)
{
    if (charPos > 1)
    {
        Serial.print("[E] Char pos > 1 :");
        Serial.println(charPos);
        return 1;
    }

    if (charValue > sizeof(_digits) / sizeof(uint8_t))
    {
        Serial.print("[E] Exceeds _digits bounds:");
        Serial.println(charValue);
        return 2;
    }

    _buffer[charPos] = withDP ? _digits[charValue] | B00000001 : _digits[charValue];
    return 0;
}

// Update the total value (both chars)
int DualDigit_74HC595::updateValue(uint8_t value, bool withDP, bool isHex)
{
    if (value > (static_cast<bool>(isHex) ? 256 : 99))
    {
        Serial.print("Value > max value :");
        Serial.println(value);
        return 1;
    }

    // Split value into 'tens' and 'units'
    uint8_t tens = isHex ? value / 16 : value / 10;
    uint8_t units = isHex ? value - (tens * 16) : value - (tens * 10);

    // Update each individual digit with the corresponding value
    updateChar(0, tens);
    updateChar(1, units, withDP);
    return 0;
}

// Update the data and optionally latch (ie show) the output
void DualDigit_74HC595::sendOut(bool showData)
{
    shiftOut(_dataPin, _clkPin, LSBFIRST, _buffer[1]);
    shiftOut(_dataPin, _clkPin, LSBFIRST, _buffer[0]);

    // Optionally toggle the latch pin to show the data on the display
    if (showData)
    {
        digitalWrite(_latchPin, HIGH);
        digitalWrite(_latchPin, LOW);
    }
}

// A mechanism to flash the current value for secs seconds
void DualDigit_74HC595::flashDigits(uint8_t secs)
{
    // We need to remember what's in the buffer to flash it
    char _oldBuffer[2] = {0};
    memcpy(_oldBuffer, _buffer, 2);

    unsigned long currMillis = millis();
    unsigned long totalDelay = secs * 1000;

    while (millis() < currMillis + totalDelay)
    {
        // Blank the display for a while
        blankDigits();
        delay(250);

        // Copy original value back and display it
        memcpy(_buffer, _oldBuffer, 2);
        sendOut();
        delay(250);
    }
}

void DualDigit_74HC595::blankDigits()
{
    // Blank the display for a while
    updateChar(0, 16);
    updateChar(1, 16);
    sendOut();
}