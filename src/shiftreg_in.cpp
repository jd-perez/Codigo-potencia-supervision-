#include "shiftreg_in.hpp"

// constructor
ShiftRegIn::ShiftRegIn() {
    last_state = 0xFF;
    first_run = true;
}

//Función para leer
uint8_t ShiftRegIn::read_165() {
    uint8_t value = 0;
    
    digitalWrite(PIN_PLIN, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_PLIN, HIGH);

    for (int i = 0; i < 8; i++) {
        value <<= 1; 
        if (digitalRead(PIN_DATAIN)) { value |= 1; }
        
        digitalWrite(PIN_CLKIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(PIN_CLKIN, LOW);
        delayMicroseconds(1);
    }
    return value;
}

//Función begin
void ShiftRegIn::begin() {
    
    pinMode(PIN_PLIN, OUTPUT);
    pinMode(PIN_CLKIN, OUTPUT);
    pinMode(PIN_DATAIN, INPUT);
    digitalWrite(PIN_PLIN, HIGH);
    digitalWrite(PIN_CLKIN, LOW);

}

bool ShiftRegIn::check_cambio(uint8_t &current_state) {
    current_state = read_165();
    
    if (first_run || (current_state != last_state)) {
        last_state = current_state;
        first_run = false;
        return true; 
    }
    return false;
}
