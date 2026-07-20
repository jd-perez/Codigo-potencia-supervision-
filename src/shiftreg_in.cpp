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
    
    // Pulse PL to load physical pin states into the shift register
    digitalWrite(PIN_IN_PL, LOW);
    delayMicroseconds(5);
    digitalWrite(PIN_IN_PL, HIGH);
    delayMicroseconds(5);

    // Read 32 bits (4 cascaded 8-bit registers)
    for (int i = 0; i < 32; i++) {
        physical_state <<= 1; 
        if (digitalRead(PIN_IN_DATA)) { 
            physical_state |= 1; 
        }
        // Pulse clock to shift to the next bit
        digitalWrite(PIN_IN_CLK, HIGH);
        delayMicroseconds(2);
        digitalWrite(PIN_IN_CLK, LOW);
        delayMicroseconds(2);
    }
    
    return physical_state;
}


#if 0//code for arduino nano replacement
#include "shiftreg_in.hpp"

void ShiftRegIn::begin() {
    // Configuración de pines para el shift register
    pinMode(PIN_PLIN, OUTPUT);
    pinMode(PIN_CLKIN, OUTPUT);
    pinMode(PIN_DATAIN, INPUT);
    digitalWrite(PIN_PLIN, HIGH);
    digitalWrite(PIN_CLKIN, LOW);
}

uint32_t ShiftRegIn::read165() {
    uint32_t physical_state = 0;
    
    // Ciclo for para leer los shift registers
    digitalWrite(PIN_PLIN, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_PLIN, HIGH);

    for (int i = 0; i < 8; i++) {
        physical_state <<= 1; 
        if (digitalRead(PIN_DATAIN)) { 
            physical_state |= 1; 
        }
        digitalWrite(PIN_CLKIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(PIN_CLKIN, LOW);
        delayMicroseconds(1);
    }
    
    return physical_state;
}
#endif