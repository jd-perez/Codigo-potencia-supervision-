#include "traffic_controller.hpp"

const uint32_t DEBOUNCE_TIME_MS = 1000; 
const uint32_t ALERT_COOLDOWN_MS = 3000; 

// Translates physical pin layout (Bits 0-29, R-Y-G) to Software array (Bits 2-31, G-Y-R)
uint32_t hardware_to_software(uint32_t physical_state) {
    uint32_t aligned_state = 0;
    
    for (int phase = 0; phase < 10; phase++) { 
        // Physical Layout: Phase 10 is at bits 0-2, Phase 1 is at 27-29.
        int phys_base = (9 - phase) * 3;
        uint32_t phys_red    = (physical_state >> (phys_base + 0)) & 0x01;
        uint32_t phys_yellow = (physical_state >> (phys_base + 1)) & 0x01;
        uint32_t phys_green  = (physical_state >> (phys_base + 2)) & 0x01;

        // Software Layout: Phase 10 is at 2-4, Phase 1 is at 29-31.
        int sw_base = (9 - phase) * 3 + 2;
        aligned_state |= (phys_green << (sw_base + 0));
        aligned_state |= (phys_yellow << (sw_base + 1));
        aligned_state |= (phys_red << (sw_base + 2));
    }
    
    return aligned_state;
}

// Translates Software array (Bits 2-31, G-Y-R) to physical pin layout (Bits 0-29, R-Y-G)
uint32_t software_to_hardware(uint32_t expected_state) {
    uint32_t hardware_state = 0;
    
    for (int phase = 0; phase < 10; phase++) {
        // Extract from Software Layout
        int sw_base = (9 - phase) * 3 + 2;
        uint32_t sw_green  = (expected_state >> (sw_base + 0)) & 0x01;
        uint32_t sw_yellow = (expected_state >> (sw_base + 1)) & 0x01;
        uint32_t sw_red    = (expected_state >> (sw_base + 2)) & 0x01;

        // Inject into Physical Layout
        int phys_base = (9 - phase) * 3;
        hardware_state |= (sw_red << (phys_base + 0));
        hardware_state |= (sw_yellow << (phys_base + 1));
        hardware_state |= (sw_green << (phys_base + 2));
    }
    
    return hardware_state;
}

TrafficController::TrafficController() 
    : strip(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800) {
    last_alert_time = 0;
    mismatch_start_time = 0;
    is_mismatch_active = false;
}

void TrafficController::begin() {
    reader.begin();
    writer.begin();
    simulator.begin();
    notifier.begin();
    
    strip.begin();
    strip.setBrightness(10);
    strip.show(); 
}

void TrafficController::update_neopixels(uint32_t ideal_state) {
    for (int phase = 0; phase < NUM_PIXELS; phase++) {
        int inverted_phase = (NUM_PIXELS - 1) - phase; 
        int base_bit = (inverted_phase * 3) + 2; 

        bool is_green  = (ideal_state >> base_bit) & 0x01;
        bool is_yellow = (ideal_state >> (base_bit + 1)) & 0x01;
        bool is_red    = (ideal_state >> (base_bit + 2)) & 0x01;

        if (is_green) {
            strip.setPixelColor(phase, strip.Color(0, 255, 0));
        } else if (is_yellow) {
            strip.setPixelColor(phase, strip.Color(255, 255, 0));
        } else if (is_red) {
            strip.setPixelColor(phase, strip.Color(255, 0, 0));
        } else {
            strip.setPixelColor(phase, strip.Color(0, 0, 0)); 
        }
    }
    strip.show();
}

void TrafficController::update() {
    uint32_t expected_state = simulator.get_expected_state();
    update_neopixels(expected_state);

    // 1. Send the translated physical state to the 74AHC595 outputs
    uint32_t hardware_output = software_to_hardware(expected_state);
    writer.write_pattern(hardware_output);

    // 2. Read the raw physical state from the 74HC165 inputs
    uint32_t raw_physical_read = reader.read165();
    
    // 3. Translate the physical read back to the software array logic
    uint32_t sw_aligned_physical = hardware_to_software(raw_physical_read);
    
    // 4. Mask the lowest 2 bits (Bits 0 and 1 are empty in the software layout)
    uint32_t mask = 0xFFFFFFFC;
    uint32_t masked_expected = expected_state & mask; 
    uint32_t masked_physical = sw_aligned_physical & mask;

    // 5. Evaluate System State
    if (masked_expected != masked_physical) {
        if (!is_mismatch_active) {
            is_mismatch_active = true;
            mismatch_start_time = millis();
        }

        if (millis() - mismatch_start_time > DEBOUNCE_TIME_MS) {
            if (millis() - last_alert_time > ALERT_COOLDOWN_MS) {
                notifier.send_alert(masked_physical, masked_expected);
                last_alert_time = millis();
            }
        }
    } else {
        is_mismatch_active = false;
    }
}

#if 0
void TrafficController::update() {
    // 1. Let the simulator and Neopixels run normally so you can see the sequence
    uint32_t expected_state = simulator.get_expected_state();
    update_neopixels(expected_state);

    // 2. Read the RAW, unmasked physical feedback from the shift registers
    uint32_t physical_inputs = reader.read165();

    // 3. Print the raw 32-bit binary string to the terminal
    Serial.println("====================================");
    Serial.print("RAW HARDWARE BITS: ");
    
    // Print all 32 bits, forcing leading zeros so it always lines up perfectly
    for (int i = 31; i >= 0; i--) {
        Serial.print((physical_inputs >> i) & 1);
        if (i % 4 == 0) Serial.print(" "); // Add a space every 4 bits for readability
    }
    Serial.println();
    
    // Slow it down so we can read it
    delay(500); 
}
#endif

#if 0  //code for arduino nano replacement
#include "traffic_controller.hpp"

// CONSTANTE DE DEBOUNCE RESTAURADA (500ms)
const uint32_t DEBOUNCE_TIME_MS = 1000; 
const uint32_t ALERT_COOLDOWN_MS = 3000; 

TrafficController::TrafficController() {
    last_alert_time = 0;
    mismatch_start_time = 0;
    is_mismatch_active = false;
}

void TrafficController::begin() {
    reader.begin();
    writer.begin();
    simulator.begin();
    notifier.begin();
}

void TrafficController::update() {
    if (millis() < 3000) return; 

    uint32_t expected_state = simulator.get_expected_state();
    writer.write_pattern(expected_state);

    // INVERT the physical inputs so a '1' (Current Flowing) becomes a '0' (Active-Low ON)
    uint8_t raw_inputs = reader.read165();
    uint8_t fault_inputs = ~raw_inputs; 
    
    uint8_t ideal_sensors = (expected_state & 0xFF);
    uint8_t active_mask = 0b00000111; 
    
    ideal_sensors &= active_mask;
    fault_inputs &= active_mask;

    if (ideal_sensors != fault_inputs) {
        if (!is_mismatch_active) {
            is_mismatch_active = true;
            mismatch_start_time = millis();
        }

        if (millis() - mismatch_start_time > DEBOUNCE_TIME_MS) {
            if (millis() - last_alert_time > ALERT_COOLDOWN_MS) {
                notifier.send_alert(raw_inputs, ideal_sensors); // Pass raw_inputs for accurate serial printing
                last_alert_time = millis();
            }
        }
    } else {
        is_mismatch_active = false;
    }
}
#endif

#if 0 
void TrafficController::update() {
    if (millis() < 500) return; // Periodo de gracia inicial AC

    uint32_t expected_state = simulator.get_expected_state();
    writer.write_pattern(expected_state);

    uint8_t fault_inputs = reader.read165();
    uint8_t ideal_sensors = (expected_state & 0xFF);

    uint8_t active_mask = 0b00000111; 
    ideal_sensors &= active_mask;
    fault_inputs &= active_mask;

    // --- LA LÓGICA DE DEBOUNCE RESTAURADA ---
    if (ideal_sensors != fault_inputs) {
        
        // 1. Si es la primera vez que vemos el error, iniciar el cronómetro
        if (!is_mismatch_active) {
            is_mismatch_active = true;
            mismatch_start_time = millis();
        }

        // 2. Solo lanzar la alerta si el error sobrevive más de 500ms
        if (millis() - mismatch_start_time > DEBOUNCE_TIME_MS) {
            if (millis() - last_alert_time > ALERT_COOLDOWN_MS) {
                notifier.send_alert(fault_inputs, ideal_sensors);
                last_alert_time = millis();
            }
        }
    } else {
        // 3. Si todo está bien, reiniciar el estado de error
        is_mismatch_active = false;
    }
}

#endif

#if 0
void TrafficController::update() {
    // Obtener el estado ideal (32-bit Active-Low)
    uint32_t expected_state = simulator.get_expected_state();
    
    // Escribir el estado 'ideal' a los LEDs 
    writer.write_pattern(expected_state);

    // Leer los sensores físicos (8-bit Active-High)
    uint8_t fault_inputs = reader.read165();
    
    // se extraen solo los primeros 8 bits del expected_state (& 0xFF)
    // se invierten (~) para que el 0 (ON en para el LED) sea 1 (ON para el sensor)
    uint8_t ideal_sensors = ~(expected_state & 0xFF);

    //Comparación
    if (ideal_sensors != fault_inputs) {
        
        if (!is_mismatch_active) {
            is_mismatch_active = true;
            mismatch_start_time = millis();
        }

        if (millis() - mismatch_start_time > DEBOUNCE_TIME_MS) {
            if (millis() - last_alert_time > ALERT_COOLDOWN_MS) {
                
                notifier.send_alert(fault_inputs, ideal_sensors);
                last_alert_time = millis();
            }
        }
    } else {
        is_mismatch_active = false;
    }
}
#endif