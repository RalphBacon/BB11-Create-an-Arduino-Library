#include <Arduino.h>
#include "DualDigit_74HC595.h"

// Pin definitions for the 7-seg display - any free ones can be used
#define dataPin 11
#define clkPin 12
#define latchPin 8

// Create display object with data, clock and latch pins
DualDigit_74HC595 dualDigit(DualDigit_74HC595::commonLedType::ANODE);

// Foward declaration of our demo functions (Arduino IDE does this for you)
void allDigits();
void allValues();

// Setup     Setup     Setup     Setup     Setup     Setup     Setup
void setup()
{
  Serial.begin(115200);
  dualDigit.begin(dataPin, clkPin, latchPin);
  Serial.println("Setup completed");
}

// Loop    Loop    Loop    Loop    Loop    Loop    Loop    Loop    Loop
void loop()
{
  // =========
  // EXAMPLE 1
  // =========
  // Output a simple counter updating each digit independently
  allDigits();

  // Display the answer to Life, the Universe and Everything
  dualDigit.updateChar(0, 4);
  dualDigit.updateChar(1, 2);
  dualDigit.sendOut();

  // Flash the output for a while
  dualDigit.flashDigits(5);

  // =========
  // EXAMPLE 2
  // =========
  // Different way to output a counter, updating both digits with "a value".
  allValues();

  // Display the answer to something else
  dualDigit.updateValue(87);
  dualDigit.sendOut();

  // Flash the output for a while
  dualDigit.flashDigits(5);
}

// Demo functions to display a looping counter 0 - FF - by individual digits
void allDigits()
{
  // Output a simple counter to digits 0 and 1
  for (int outer = 0; outer < 16; outer++)
  {
    dualDigit.updateChar(0, outer);
    dualDigit.updateChar(1, 0);

    for (int inner = 0; inner < 16; inner++)
    {
      dualDigit.updateChar(1, inner);
      dualDigit.sendOut();
      delay(175);
    }
  }
}

// Demo functions to display a looping counter 0 - FF - by value
void allValues()
{
  // Output a simple counter
  for (int count = 255; count > 0; count--)
  {
    // Output the count, with optional dp if remainder == 4, is it HEX (A-F) output
    dualDigit.updateValue(count, count % 10 == 4 ? true : false, true);
    dualDigit.sendOut();
    delay(200);
  }
}
