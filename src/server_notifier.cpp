
#include "server_notifier.hpp"

void ServerNotifier::begin() {}

void ServerNotifier::send_alert(uint32_t physical_state, uint32_t expected_state) {
    Serial.println("=================================================");
    Serial.println("HARDWARE FAULT DETECTED");

    for (int phase = 0; phase < TOTAL_PHASES; phase++) { 
        // THE CORRECT MATHEMATICAL OFFSET
        int base_bit = 29 - (phase * 3); 

        bool exp_green  = (expected_state >> base_bit) & 0x01;
        bool exp_yellow = (expected_state >> (base_bit + 1)) & 0x01;
        bool exp_red    = (expected_state >> (base_bit + 2)) & 0x01;

        bool phys_green  = (physical_state >> base_bit) & 0x01;
        bool phys_yellow = (physical_state >> (base_bit + 1)) & 0x01;
        bool phys_red    = (physical_state >> (base_bit + 2)) & 0x01;

        int phase_display = phase + 1; 

        // 1. Compare Green
        if (exp_green == 1 && phys_green == 0) Serial.printf("BURNT BULB: Phase %d [Green]\n", phase_display);
        if (exp_green == 0 && phys_green == 1) Serial.printf("SHORTED TRIAC: Phase %d [Green]\n", phase_display);

        // 2. Compare Yellow
        if (exp_yellow == 1 && phys_yellow == 0) Serial.printf("BURNT BULB: Phase %d [Yellow]\n", phase_display);
        if (exp_yellow == 0 && phys_yellow == 1) Serial.printf("SHORTED TRIAC: Phase %d [Yellow]\n", phase_display);

        // 3. Compare Red
        if (exp_red == 1 && phys_red == 0) Serial.printf("BURNT BULB: Phase %d [Red]\n", phase_display);
        if (exp_red == 0 && phys_red == 1) Serial.printf("SHORTED TRIAC: Phase %d [Red]\n", phase_display);
    }
}

#if 0
#include "server_notifier.hpp"

void ServerNotifier::begin() {}

void ServerNotifier::send_alert(uint8_t physical_state, uint8_t expected_state) {
    // physical_state: 1 = Current Flowing, 0 = No Current
    // expected_state: 0 = Lamp Commanded ON, 1 = Lamp Commanded OFF

    Serial.println("-------------------------------------------------");
    Serial.println("ALERTA DE HARDWARE DETECTADA!");

    for (int pin = 0; pin < 3; pin++) { 
        uint8_t expected_bit = (expected_state >> pin) & 0x01;
        uint8_t physical_bit = (physical_state >> pin) & 0x01;

        // Commanded ON (0), but no current flowing (0)
        if (expected_bit == 0 && physical_bit == 0) {
            Serial.print("FOCO QUEMADO detectado en fase: ");
            Serial.println(pin + 1);
        }
        
        // Commanded OFF (1), but current is flowing (1)
        if (expected_bit == 1 && physical_bit == 1) {
            Serial.print("TRIAC EN CORTO detectado en fase: ");
            Serial.println(pin + 1);
        }
    }

    Serial.println("-------------------------------------------------");
}

#endif

#if 0
#include "server_notifier.hpp"

void ServerNotifier::begin() {
    // Inicialización si es necesaria en el futuro
}

void ServerNotifier::send_alert(uint8_t physical_state, uint8_t expected_state) {
    
    // El XOR aísla exactamente qué sensores no coinciden con su comando ideal
    uint8_t differences = physical_state ^ expected_state;

    // Como demostró la física, tanto un Foco Quemado como un Triac en Corto 
    // provocan que el sensor lea '0' cuando el Arduino esperaba un '1' (Luz APAGADA).
    // Con esta línea filtramos exactamente ese escenario:
    uint8_t hardware_errors = differences & expected_state;
    
    Serial.println("-------------------------------------------------");
    Serial.println("ALERTA DE HARDWARE DETECTADA!");

    // Imprimir el error combinado
    if (hardware_errors > 0) {
        Serial.print("FALLA EN FASE (Corto o Foco Quemado): [ ");
        
        // Evaluamos los pines 0, 1 y 2 (Verde, Amarillo, Rojo)
        for (int pin = 0; pin < 3; pin++) { 
            if (hardware_errors & (1 << pin)) {
                Serial.print(pin + 1); 
                Serial.print(" ");
            }
        }
        Serial.println("]");
    }

    Serial.print("Sensores Esperados: "); Serial.println(expected_state, BIN);
    Serial.print("Sensores Actuales:  "); Serial.println(physical_state, BIN);
    Serial.println("-------------------------------------------------");
}
#endif