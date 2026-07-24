// shiftreg_out.cpp
#include "shiftreg_out.hpp"

ShiftRegOut::ShiftRegOut() {
    last_written_state = 0; 
}

void ShiftRegOut::led_write(uint8_t reg4, uint8_t reg3, uint8_t reg2, uint8_t reg1) {
    digitalWrite(PIN_OUT_LATCH, LOW);
    shiftOut(PIN_OUT_DATA, PIN_OUT_CLK, MSBFIRST, reg4);
    shiftOut(PIN_OUT_DATA, PIN_OUT_CLK, MSBFIRST, reg3);
    shiftOut(PIN_OUT_DATA, PIN_OUT_CLK, MSBFIRST, reg2);
    shiftOut(PIN_OUT_DATA, PIN_OUT_CLK, MSBFIRST, reg1);
    digitalWrite(PIN_OUT_LATCH, HIGH);
}

void ShiftRegOut::begin() {
    pinMode(PIN_OUT_DATA, OUTPUT);
    pinMode(PIN_OUT_LATCH, OUTPUT);
    pinMode(PIN_OUT_CLK, OUTPUT);
    pinMode(PIN_OUT_OE, OUTPUT);

    digitalWrite(PIN_OUT_OE, HIGH);   
    digitalWrite(PIN_OUT_CLK, LOW);    
    digitalWrite(PIN_OUT_LATCH, LOW);
    
    led_write(0x00, 0x00, 0x00, 0x00); 
    digitalWrite(PIN_OUT_OE, LOW);   
}

void ShiftRegOut::write_pattern(uint32_t pattern) {
    if (pattern != last_written_state) {
        uint8_t reg1 = pattern & 0xFF;           
        uint8_t reg2 = (pattern >> 8) & 0xFF;    
        uint8_t reg3 = (pattern >> 16) & 0xFF;   
        uint8_t reg4 = (pattern >> 24) & 0xFF;   

        led_write(reg4, reg3, reg2, reg1);
        last_written_state = pattern;
    }
}