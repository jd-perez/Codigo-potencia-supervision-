#include "state_simulator.hpp"

// Total number of steps in the array
const uint16_t SCENARIO_LENGTH = 91;

// The 32-bit state mapping for the 30 bulbs
const uint32_t SCENARIO[SCENARIO_LENGTH] = {
    0, 843654288, 306783376, 843654288, 306783376, 843654288, 306783376, 843654288,
    1380525200, 0, 2252940432, 2185831568, 2252940432, 2185831568, 2252940432, 
    2185831568, 2252940432, 2320049296, 2454267024, 2429101200, 2420712592, 
    2429101200, 2420712592, 2429101200, 2420712592, 2429101200, 2437489808, 
    2454267024, 2451121296, 2450072720, 2451121296, 2450072720, 2451121296, 
    2450072720, 2451121296, 2452169872, 2454267024, 2453873808, 2453742736, 
    2453873808, 2453742736, 2453873808, 2453742736, 2453873808, 2454004880, 
    2454267024, 2454217872, 2454201488, 2454217872, 2454201488, 2454217872, 
    2454201488, 2454217872, 2454234256, 2454267024, 2454260880, 2454258832, 
    2454260880, 2454258832, 2454260880, 2454258832, 2454260880, 2454262928, 
    2454267024, 2454266256, 2454266000, 2454266256, 2454266000, 2454266256, 
    2454266000, 2454266256, 2454266512, 2454267024, 2454266928, 2454266896, 
    2454266928, 2454266896, 2454266928, 2454266896, 2454266928, 2454266960, 
    2454267024, 2454267012, 2454267008, 2454267012, 2454267008, 2454267012, 
    2454267008, 2454267012, 2454267016, 2454267024
};
// 91-element timing array (Values in Milliseconds)
const uint32_t SCENARIO_TIMINGS[SCENARIO_LENGTH] = {
    // Phase 1 
    // State 0 (Skip), Solid Green, Blink Off, Blink On, Blink Off, Blink On, Blink Off, Blink On, Solid Yellow
    0, 4000, 500, 500, 500, 500, 500, 500, 1500, 
    
    // Phase 2
    0, 4000, 500, 500, 500, 500, 500, 500, 1500, 
    // Phase 3 
    0, 4000, 500, 500, 500, 500, 500, 500, 1500, 
    // Phase 4
    0, 4000, 500, 500, 500, 500, 500, 500, 1500,
    // Phase 5
    0, 4000, 500, 500, 500, 500, 500, 500, 1500,
    // Phase 6
    0, 4000, 500, 500, 500, 500, 500, 500, 1500,
    // Phase 7
    0, 4000, 500, 500, 500, 500, 500, 500, 1500,
    // Phase 8
    0, 4000, 500, 500, 500, 500, 500, 500, 1500,
    // Phase 9
    0, 4000, 500, 500, 500, 500, 500, 500, 1500,
    // Phase 10
    0, 4000, 500, 500, 500, 500, 500, 500, 1500,
    
    // End Padding (Index 90)
    0
};
// Class Constructor
StateSimulator::StateSimulator() {
    last_step_time = 0;
    current_step_index = 0; // Start at the beginning of the sequence
}

void StateSimulator::begin() {
    last_step_time = millis();
}

uint32_t StateSimulator::get_expected_state() {
    uint32_t current_time = millis();
    uint32_t current_interval = SCENARIO_TIMINGS[current_step_index];
    
    // Non-blocking timer: Advance to the next step if the interval has passed
    // If the interval is 0, it will instantly advance to the next index
    if (current_time - last_step_time >= current_interval) {
        current_step_index++;
        
        // Loop back to the start if we hit the end of the array
        if (current_step_index >= SCENARIO_LENGTH) {
            current_step_index = 0;
        }
        last_step_time = current_time;
    }
    
    // Return the raw 32-bit state to be passed to the shift registers and NeoPixels
    return SCENARIO[current_step_index];
}

#if 0  //code for arduino nano replacement
#include "state_simulator.hpp"

// Cambia de fase cada 3 segundos (3000 ms)
const uint32_t TOGGLE_INTERVAL_MS = 3000; 

StateSimulator::StateSimulator() {
    last_step_time = 0;
    current_phase = 0; // 0 = Verde, 1 = Amarillo, 2 = Rojo
}

void StateSimulator::begin() {
    last_step_time = millis();
}

uint32_t StateSimulator::get_expected_state() {
    uint32_t current_time = millis();
    
    if (current_time - last_step_time >= TOGGLE_INTERVAL_MS) {
        current_phase++;
        if (current_phase > 2) current_phase = 0; 
        last_step_time = current_time;
    }
    
    // Lógica Active-Low: Un '0' enciende el foco.
    if (current_phase == 0) return ~0b00000001; // Verde
    if (current_phase == 1) return ~0b00000010; // Amarillo
    if (current_phase == 2) return ~0b00000100; // Rojo
    
    return 0xFFFFFFFF; // Todo apagado
}
#endif

#if 0
// Constantes
const uint32_t BLINK_INTERVAL_MS = 500;
const uint32_t GREEN_INTERVAL_MS = 5000;
const uint16_t SCENARIO_LENGTH = 91;

// Arreglo dado por Jordy para los diferentes estados de las fases
const uint32_t SCENARIO[SCENARIO_LENGTH] = {
    0, 838860800, 301989888, 838860800, 301989888, 838860800, 301989888, 838860800,
    1375731712, 0, 2248146944, 2181038080, 2248146944, 2181038080, 2248146944, 2181038080,
    2248146944, 2315255808, 2449473536, 2424307712, 2415919104, 2424307712, 2415919104,
    2424307712, 2415919104, 2424307712, 2432696320, 2449473536
};

const uint32_t SCENARIO_TIMINGS[SCENARIO_LENGTH] = {
    // Bloque 1 (Tiempos: Transición, OFF, ON, OFF, ON, OFF, ON, Verde)
    0, 0, 500, 3000, 500, 3000, 500, 3000, 18000, 
    // Bloque 2
    0, 0, 500, 3000, 500, 3000, 500, 3000, 18000, 
    // Bloque 3 (Nota el 0 extra al final por la longitud de 28)
    0, 0, 500, 3000, 500, 3000, 500, 3000, 18000, 0
};

#if 0
const uint32_t SCENARIO_TIMINGS[SCENARIO_LENGTH] = {
    0, 0, 500, 500, 500, 500, 500, 500, 3000, 0, 0, 500, 500, 500, 500, 500, 
    500, 3000, 0, 0, 500, 500, 500, 500, 500, 500, 3000, 0
};
#endif

const int BIT_MAP[8] = {
    // Se genera un arreglo del siguiente tipo:
    // Bits: 31 30 29  28 27 26  25 24 23
    // Son:  Bloque 1  Bloque 2  Bloque 3


    // Semáforo 1 (Bloque 3 del arreglo)
    25, // Físico 1: Rojo 1
    23, // Físico 2: Amarillo 1 (Parpadeo)
    24, // Físico 3: Verde 1 

    // Semáforo 2 (Bloque 2 del arreglo)
    28, // Físico 4: Rojo 2
    26, // Físico 5: Amarillo 2
    27, // Físico 6: Verde 2

    // Semáforo 3 (Bloque 1 del arreglo)
    31, // Físico 7: Rojo 3
    29  // Físico 8: Amarillo 3
    
};


StateSimulator::StateSimulator() {
    last_step_time = 0;
    current_step_index = SCENARIO_LENGTH - 1; 
}

void StateSimulator::begin() {
    last_step_time = millis();
}

uint32_t StateSimulator::get_expected_state() {
    uint32_t current_time = millis();
    uint32_t current_interval = SCENARIO_TIMINGS[current_step_index];
    
// Si el tiempo es 0ms, este bloque avanza instantáneamente al siguiente paso
    if (current_time - last_step_time >= current_interval) {
        if (current_step_index == 0) {
            current_step_index = SCENARIO_LENGTH - 1;
        } else {
            current_step_index--;
        }
        last_step_time = current_time;
    }
    
    uint32_t raw_array_value = SCENARIO[current_step_index];
    uint32_t physical_pattern = 0xFFFFFFFF; // Empieza todo apagado (Active-Low)

// Solo revisamos los 8 primeros LEDs
    for (int physical_pin = 0; physical_pin < 8; physical_pin++) {
        int target_array_bit = BIT_MAP[physical_pin];
        bool is_led_on = (raw_array_value & (1UL << target_array_bit)) != 0;

        if (is_led_on) {
            physical_pattern &= ~(1UL << physical_pin); 
        }
    }

    return physical_pattern;
}
#endif