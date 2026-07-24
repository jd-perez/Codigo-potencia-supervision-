
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




