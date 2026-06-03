#ifndef REGISTRODESIN_HPP
#define REGISTRODESIN_HPP

#include <Arduino.h>

// Definiciones de pines shift register (registro de desplazamiento) de ENTRADA
#define PIN_PLIN       14  //Pin de carga paralela (Parallel Load)
#define PIN_CLKIN      32  //Pin de reloj de entrada (ClockIn)
#define PIN_DATAIN    13  //Pin de datos de entrada (DataIn)


// Clase enfocada puramente en los registros de desplazamiento (shift registers) de entrada
class ShiftRegIn{
  
  private:
    uint8_t last_state; 
    bool first_run;
    uint8_t read_165();

  public:
    // nombre del constructor
    ShiftRegIn();

    void begin();
    //& es para el 'pass by reference' es decir, que la función accesa la variable original directamente
    bool check_cambio(uint8_t &current_state);
};

#endif