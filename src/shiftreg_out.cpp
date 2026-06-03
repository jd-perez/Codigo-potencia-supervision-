#include "shiftreg_out.hpp"

void ShiftRegOut::led_write(uint8_t reg4, uint8_t reg3, uint8_t reg2, uint8_t reg1) {
    digitalWrite(PIN_LATCHOUT, LOW);
    shiftOut(PIN_DATAOUT, PIN_CLKOUT, MSBFIRST, reg4);
    shiftOut(PIN_DATAOUT, PIN_CLKOUT, MSBFIRST, reg3);
    shiftOut(PIN_DATAOUT, PIN_CLKOUT, MSBFIRST, reg2);
    shiftOut(PIN_DATAOUT, PIN_CLKOUT, MSBFIRST, reg1);
    digitalWrite(PIN_LATCHOUT, HIGH);
}

void ShiftRegOut::begin() {
    pinMode(PIN_DATAOUT, OUTPUT);
    pinMode(PIN_LATCHOUT, OUTPUT);
    pinMode(PIN_CLKOUT, OUTPUT);
    pinMode(PIN_OEOUT, OUTPUT);

    digitalWrite(PIN_OEOUT, HIGH);   
    digitalWrite(PIN_CLKOUT, LOW);    
    digitalWrite(PIN_LATCHOUT, LOW);
    
    led_write(0xFF, 0xFF, 0xFF, 0xFF); 
    digitalWrite(PIN_OEOUT, LOW);   
}

// Parte el patron de 32 bits (los 4 registros de desplazamiento) en 4 bytes y los envia en al hardware
void ShiftRegOut::write_pattern(uint32_t pattern) {
    uint8_t reg1 = pattern & 0xFF;           
    uint8_t reg2 = (pattern >> 8) & 0xFF;    
    uint8_t reg3 = (pattern >> 16) & 0xFF;   
    uint8_t reg4 = (pattern >> 24) & 0xFF;   

    led_write(reg4, reg3, reg2, reg1);
}

