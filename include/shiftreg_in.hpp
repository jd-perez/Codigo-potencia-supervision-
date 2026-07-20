//code for the trial PCB
#ifndef SHIFTREG_IN_HPP
#define SHIFTREG_IN_HPP

#include <Arduino.h>

// ESP32 Pins for 74HC165 (Inputs)
#define PIN_IN_PL     14  // Latch / Parallel Load
#define PIN_IN_CLK    32  // Clock (CP)
#define PIN_IN_DATA   13  // Data (Q7 / MISO)

class ShiftRegIn {
public:
    void begin();
    uint32_t read165();
};

#endif


#if 0 // code for the arduino nano replacement
#ifndef SHIFTREG_IN_HPP
#define SHIFTREG_IN_HPP

#include <Arduino.h>

// Definiciones de pines shift register (registro de desplazamiento) de ENTRADA
#define PIN_PLIN       8  //Pin de carga paralela (Parallel Load)
#define PIN_CLKIN      13  //Pin de reloj de entrada (ClockIn)
#define PIN_DATAIN    12  //Pin de datos de entrada (DataIn)


// Clase enfocada puramente en los registros de desplazamiento (shift registers) de entrada
class ShiftRegIn{

  public:
    void begin();
    uint32_t read165();
};

#endif
#endif