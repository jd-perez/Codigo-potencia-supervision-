
#ifndef STATE_SIMULATOR_HPP
#define STATE_SIMULATOR_HPP

#include <Arduino.h>

class StateSimulator {
private:
    uint32_t last_step_time;
    uint16_t current_step_index; 

public:
    StateSimulator();
    void begin();
    uint32_t get_expected_state();
};

#endif



#if 0 //code for arduino nano replacement
#ifndef STATE_SIMULATOR_HPP
#define STATE_SIMULATOR_HPP

#include <Arduino.h>
class StateSimulator {
private:
    uint32_t last_step_time;
    uint8_t current_phase; // Variable para saber qué color está encendido

public:
    StateSimulator();
    void begin();
    uint32_t get_expected_state();
};
#endif
#endif

#if 0
// Macros visuales para hacer el arreglo de tiempos fácil de leer
#define B 500   // Parpadeo Amarillo (Blink)
#define G 60000  // Verde Sólido (Green)
#define R 2000  // Rojo de transición (Red)

class StateSimulator {
private:
    uint32_t last_step_time;
    uint16_t current_step_index;

public:
    StateSimulator();
    void begin();
    uint32_t get_expected_state();
};
#endif

