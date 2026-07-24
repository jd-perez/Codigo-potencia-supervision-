#include "state_simulator.hpp"

// CHANGED: Reduced from 4 states to 3.
const uint16_t SCENARIO_LENGTH = 3;

// 3-bit logic: Red (Bit 2), Yellow (Bit 1), Green (Bit 0)
const uint32_t SCENARIO[SCENARIO_LENGTH] = {
    0x01, // State 0: Green 
    0x02, // State 1: Yellow
    0x04  // State 2: Red
};

const uint32_t SCENARIO_TIMINGS[SCENARIO_LENGTH] = {
    3000, // Green for 3 seconds
    1000, // Yellow for 1 second
    3000  // Red for 3 seconds
};

StateSimulator::StateSimulator() {
    last_step_time = 0;
    current_step_index = 0; 
}

void StateSimulator::begin() {
    last_step_time = millis();
}

uint32_t StateSimulator::get_expected_state() {
    uint32_t current_time = millis();
    if (current_time - last_step_time >= SCENARIO_TIMINGS[current_step_index]) {
        current_step_index++;
        if (current_step_index >= SCENARIO_LENGTH) current_step_index = 0;
        last_step_time = current_time;
    }
    return SCENARIO[current_step_index];
}