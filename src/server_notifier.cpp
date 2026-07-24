// server_notifier.cpp
#include "server_notifier.hpp"

void ServerNotifier::begin() {}

void ServerNotifier::send_alert(uint8_t software_bit_index, PhaseStatus fault_type) {
    Serial.println("=================================================");
    Serial.println("HARDWARE FAULT DETECTED");

    int phase_display = 1; // Hardcoded for 1-phase simulation
    String color_str;

    if (software_bit_index == 0) color_str = "Green";
    else if (software_bit_index == 1) color_str = "Yellow";
    else if (software_bit_index == 2) color_str = "Red";
    else color_str = "Unknown";

    char printBuffer[64]; 
    if (fault_type == FAULT_BURNT_OR_SHORTED) {
        sprintf(printBuffer, "BURNT BULB OR SHORTED TRIAC: Phase %d [%s]", phase_display, color_str.c_str());
        Serial.println(printBuffer);
    } else if (fault_type == FAULT_OPEN_TRIAC) {
        sprintf(printBuffer, "OPEN TRIAC: Phase %d [%s]", phase_display, color_str.c_str());
        Serial.println(printBuffer);
    }
}


// send_alert code for 30 phases (physically)
#if 0
void ServerNotifier::send_alert(uint8_t software_bit_index, PhaseStatus fault_type) {
    Serial.println("=================================================");
    Serial.println("HARDWARE FAULT DETECTED");

    // Reverse engineer the phase and color from the software bit index
    // Software layout: bits 2-31. Phase 10 is at 2-4, Phase 1 is at 29-31.
    int phase_group = 9 - ((software_bit_index - 2) / 3);
    int color_index = (software_bit_index - 2) % 3; 
    
    int phase_display = phase_group + 1;
    String color_str;

    if (color_index == 0) color_str = "Green";
    else if (color_index == 1) color_str = "Yellow";
    else if (color_index == 2) color_str = "Red";

// Allocate a buffer large enough to hold the final text
    char printBuffer[64]; 

    if (fault_type == FAULT_BURNT_OR_SHORTED) {
        sprintf(printBuffer, "BURNT BULB OR SHORTED TRIAC: Phase %d [%s]", phase_display, color_str.c_str());
        Serial.println(printBuffer);
    } else if (fault_type == FAULT_OPEN_TRIAC) {
        sprintf(printBuffer, "OPEN TRIAC: Phase %d [%s]", phase_display, color_str.c_str());
        Serial.println(printBuffer);
    }
}
#endif