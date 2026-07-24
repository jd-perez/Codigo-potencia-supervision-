#ifndef SHIFTREG_OUT_HPP
#define SHIFTREG_OUT_HPP

#include <Arduino.h>

// Updated to match Proteus image_2f593a.png
#define PIN_OUT_LATCH 10  // Latch (STCP)
#define PIN_OUT_CLK   13  // Shared Clock (SHCP)
#define PIN_OUT_DATA  11  // Data to 74HC595 (SDI)
#define PIN_OUT_OE    9   // Output Enable (Active Low)

class ShiftRegOut {
private:
    uint32_t last_written_state;
    void led_write(uint8_t reg4, uint8_t reg3, uint8_t reg2, uint8_t reg1); 

public:
    ShiftRegOut();
    void begin();
    void write_pattern(uint32_t pattern);
};

#endif