#include "traffic_controller.hpp"

TrafficController::TrafficController() {
    for(int i = 0; i < TOTAL_PHASES; i++) {
        phases[i].health = STATUS_OK;
        phases[i].isDebouncing = false;
        phases[i].alertSent = false;
    }
}

void TrafficController::begin() {
    reader.begin();
    writer.begin();
    simulator.begin();
    notifier.begin();
}

void TrafficController::evaluate_phase(int bit_index) {
    TrafficPhase &phase = phases[bit_index];
    PhaseStatus instantaneousState = STATUS_OK;

    if (phase.isCommandedOn) {
        instantaneousState = (!phase.isSensorActive) ? STATUS_OK : FAULT_OPEN_TRIAC;
    } else {
        instantaneousState = (phase.isSensorActive) ? STATUS_OK : FAULT_BURNT_OR_SHORTED;
    }

    if (instantaneousState != STATUS_OK) {
        if (!phase.isDebouncing) {
            phase.isDebouncing = true;
            phase.lastFaultTime = millis();
        } else if ((millis() - phase.lastFaultTime) > ZERO_CROSS_DEBOUNCE_MS) {
            phase.health = instantaneousState;
            if (!phase.alertSent) {
                // Offset by 2 to align with the ServerNotifier's expected 32-bit layout tracking (for physical system use bit_index+2)
                notifier.send_alert(bit_index, phase.health);
                phase.alertSent = true; 
            }
        }
    } else {
        if (phase.health != STATUS_OK) {
            Serial.print("Phase bit ");
            Serial.print(bit_index);
            Serial.println(" RECOVERED.");
        }
        phase.isDebouncing = false;
        phase.health = STATUS_OK;
        phase.alertSent = false;
    }
}


void TrafficController::update() {
    static uint32_t previous_expected_state = 0;
    static unsigned long state_change_time = 0;
    
    // The amount of time to ignore the shift registers after a color change
    const unsigned long TRANSITION_BLIND_SPOT = 600; 

    uint32_t expected_state = simulator.get_expected_state();

    // Trigger only on the exact millisecond the state changes
    if (expected_state != previous_expected_state) {
        writer.write_pattern(expected_state);
        previous_expected_state = expected_state;
        state_change_time = millis();
        
        // Reset all debounce flags so old data doesn't trigger instant faults
        for(int i = 0; i < TOTAL_PHASES; i++) {
             phases[i].isDebouncing = false;
        }
    }

    // NON-BLOCKING MASK: If 600ms hasn't passed since the last change, exit the function.
    // This allows the main loop to run at full speed for I2C and RTOS tasks.
    if (millis() - state_change_time < TRANSITION_BLIND_SPOT) {
        return; 
    }

    // --- HARDWARE EVALUATION ---
    // This code only runs after the capacitors have stabilized
    uint32_t raw_physical_read = reader.read165();
    
    for (int bit = 0; bit < 3; bit++) {
        phases[bit].isCommandedOn = (expected_state >> bit) & 0x01;
        phases[bit].isSensorActive = (raw_physical_read >> bit) & 0x01;
        
        evaluate_phase(bit);
    }
}