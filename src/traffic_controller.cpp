#include "traffic_controller.hpp"

// Activación del puerto Serial, se 'encienden' despues las entradas y salidas.
void TrafficController::begin() {
    Serial.begin(115200);
    delay(500);
    
    inputs.begin();
    outputs.begin();
}

//Programación lógica
void TrafficController::update() {
    uint8_t current;

    // Se ejecuta solo si hubo un cambio de estado
    if (inputs.check_cambio(current)) {
        
        // Impresión de las filas visuales
        Serial.print("Estado de las entradas: [");
        for (int i = 0; i < 8; i++) {
            if (current & (1 << i)) { Serial.print(" 🟢 "); } 
            else { Serial.print(" 🔴 "); }
        }
        Serial.println("]");

        //Para mapear los circulos a los 8 LEDs del centro de la tarjeta (se ingnoran los de los extremos)
        uint32_t led_pattern = 0xFFFFFFFF;

        for (int i = 0; i < 8; i++) {
            if (current & (1 << i)) {
                int target_led_1 = 3 + (i * 3);
                int target_led_2 = 4 + (i * 3);
                int target_led_3 = 5 + (i * 3);
                
                led_pattern &= ~(1UL << target_led_1);
                led_pattern &= ~(1UL << target_led_2);
                led_pattern &= ~(1UL << target_led_3);
            }
        }

        // Se manda el patrón a los LEDs
        outputs.write_pattern(led_pattern);
    }

    delay(30); 
}