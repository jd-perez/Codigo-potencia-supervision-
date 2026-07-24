#ifndef SHIFTREG_IN_HPP
#define SHIFTREG_IN_HPP

#include <Arduino.h>

// Updated to match Proteus image_2f593a.png
#define PIN_IN_PL     8   // Latch / Parallel Load (PL)
#define PIN_IN_CLK    13  // Shared Clock (CLK)
#define PIN_IN_DATA   12  // Data from 74HC165 (SDO)

class ShiftRegIn {
public:
    void begin();
    uint32_t read165();
};

#endif