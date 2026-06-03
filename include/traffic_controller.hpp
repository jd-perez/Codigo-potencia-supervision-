#ifndef CONTROLADOR_TRAFICO_HPP
#define CONTROLADOR_TRAFICO_HPP

#include <Arduino.h>
#include "shiftreg_in.hpp"
#include "shiftreg_out.hpp"

class TrafficController {
  private:
    // Aquí se manejan ambas las entradas y las salidas para el controlador general, 
    // pero su lógica individual está en otros archivos
    ShiftRegIn inputs;
    ShiftRegOut outputs;

  public:
    //Las únicas dos funciones que se pueden (y que son necesarias) en archivos externos
    void begin();
    void update();
};

#endif