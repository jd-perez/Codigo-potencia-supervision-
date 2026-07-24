// shiftreg_in.cpp
#include "shiftreg_in.hpp"

void ShiftRegIn::begin() {
    pinMode(PIN_IN_PL, OUTPUT);
    pinMode(PIN_IN_CLK, OUTPUT);
    pinMode(PIN_IN_DATA, INPUT);
    
    digitalWrite(PIN_IN_PL, HIGH);
    digitalWrite(PIN_IN_CLK, LOW);
}

uint32_t ShiftRegIn::read165() {
    uint32_t physical_state = 0;
    
    digitalWrite(PIN_IN_PL, LOW);
    delayMicroseconds(5);
    digitalWrite(PIN_IN_PL, HIGH);
    delayMicroseconds(5);

    //for the 30-phase system, change i<8 to i<32
    for (int i = 0; i < 8; i++) {
        physical_state <<= 1; 
        if (digitalRead(PIN_IN_DATA)) { 
            physical_state |= 1; 
        }
        digitalWrite(PIN_IN_CLK, HIGH);
        delayMicroseconds(2);
        digitalWrite(PIN_IN_CLK, LOW);
        delayMicroseconds(2);
    }
    
    return physical_state;
}