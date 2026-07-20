
#include "shiftreg_out.hpp"

ShiftRegOut::ShiftRegOut() {
    last_written_state = 0; // Empty initial state
}

void ShiftRegOut::led_write(uint8_t reg4, uint8_t reg3, uint8_t reg2, uint8_t reg1) {
    digitalWrite(PIN_OUT_LATCH, LOW);
    
    // Shift out the 32 bits (4 cascaded 8-bit registers)
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

    // Disable outputs (Active Low OE) before setting initial state
    digitalWrite(PIN_OUT_OE, HIGH);   
    digitalWrite(PIN_OUT_CLK, LOW);    
    digitalWrite(PIN_OUT_LATCH, LOW);
    
    // Sending 0x00 physically outputs 0V, keeping Triacs OFF during boot.
    led_write(0x00, 0x00, 0x00, 0x00); 
    
    // Enable outputs
    digitalWrite(PIN_OUT_OE, LOW);   
}

void ShiftRegOut::write_pattern(uint32_t pattern) {
    // INVERSION LOGIC: The software sends a '0', but the hardware needs a '1' 
    // to turn on the Triac. Invert here before writing to the physical pins.
    uint32_t physical_pattern = pattern;

    if (physical_pattern != last_written_state) {
        uint8_t reg1 = physical_pattern & 0xFF;           
        uint8_t reg2 = (physical_pattern >> 8) & 0xFF;    
        uint8_t reg3 = (physical_pattern >> 16) & 0xFF;   
        uint8_t reg4 = (physical_pattern >> 24) & 0xFF;   

        led_write(reg4, reg3, reg2, reg1);
        last_written_state = physical_pattern;
    }
}

#if 0 //code for arduino nano replacement
#include "shiftreg_out.hpp"

// 1. El Constructor (Faltaba este)
ShiftRegOut::ShiftRegOut() {
    last_written_state = 0; // Estado inicial vacío
}

// 2. La función de escritura de hardware (Faltaba esta)
void ShiftRegOut::led_write(uint8_t reg4, uint8_t reg3, uint8_t reg2, uint8_t reg1) {
    digitalWrite(PIN_LATCHOUT, LOW);
    shiftOut(PIN_DATAOUT, PIN_CLKOUT, MSBFIRST, reg4);
    shiftOut(PIN_DATAOUT, PIN_CLKOUT, MSBFIRST, reg3);
    shiftOut(PIN_DATAOUT, PIN_CLKOUT, MSBFIRST, reg2);
    shiftOut(PIN_DATAOUT, PIN_CLKOUT, MSBFIRST, reg1);
    digitalWrite(PIN_LATCHOUT, HIGH);
}

// 3. El Setup de los pines (Actualizado para el arranque seguro de 0V)
void ShiftRegOut::begin() {
    pinMode(PIN_DATAOUT, OUTPUT);
    pinMode(PIN_LATCHOUT, OUTPUT);
    pinMode(PIN_CLKOUT, OUTPUT);
    pinMode(PIN_OEOUT, OUTPUT);

    digitalWrite(PIN_OEOUT, HIGH);   
    digitalWrite(PIN_CLKOUT, LOW);    
    digitalWrite(PIN_LATCHOUT, LOW);
    
    // Mandar 0x00 físicamente envía 0V, manteniendo los Triacs APAGADOS.
    led_write(0x00, 0x00, 0x00, 0x00); 
    digitalWrite(PIN_OEOUT, LOW);   
}

// 4. La escritura del patrón (Actualizado con la inversión lógica)
void ShiftRegOut::write_pattern(uint32_t pattern) {
    // EL TRUCO: El software manda un '0', pero el hardware necesita un '1' para 
    // encender el Triac. Invertimos aquí y solo aquí.
    uint32_t physical_pattern = ~pattern;

    if (physical_pattern != last_written_state) {
        uint8_t reg1 = physical_pattern & 0xFF;           
        uint8_t reg2 = (physical_pattern >> 8) & 0xFF;    
        uint8_t reg3 = (physical_pattern >> 16) & 0xFF;   
        uint8_t reg4 = (physical_pattern >> 24) & 0xFF;   

        led_write(reg4, reg3, reg2, reg1);
        last_written_state = physical_pattern;
    }
}
#endif