#ifndef SHIFTREG_OUT_HPP
#define SHIFTREG_OUT_HPP

#include <Arduino.h>

// ESP32 Pins for 74HC595 (Outputs)
#define PIN_OUT_LATCH 33   // Latch (STCP)
#define PIN_OUT_CLK   15  // Clock (SHCP)
#define PIN_OUT_DATA  12  // Data (DS / MOSI)
#define PIN_OUT_OE    27  // Output Enable (Active Low)

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

#if 0 //code for arduino nano replacement
#ifndef SHIFTREG_OUT_HPP
#define SHIFTREG_OUT_HPP

#include <Arduino.h>

// Definiciones de pines shift register (registor de desplazamiento) de SALIDA
#define PIN_DATAOUT  11  // Pin de datos de salida (DataOut)
#define PIN_LATCHOUT 10  // Pin de almacenamiento temporal de salida (Latch)
#define PIN_CLKOUT   13  // Pin de reloj de salida (ClockOut)
#define PIN_OEOUT    9  // Pin de activación de salidas (Output enable)

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
#endif