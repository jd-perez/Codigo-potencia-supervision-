#include <Arduino.h>
#include "shiftreg_in.hpp"
#include "shiftreg_out.hpp"
#include "traffic_controller.hpp"


// Global supervisor instance
TrafficController system_supervisor;

void setup() {
    // 115200 baud rate
    Serial.begin(115200);
    
    // Brief delay to ensure serial monitor connection stabilizes 
    delay(500);
    
    system_supervisor.begin();
    Serial.println("System Initialized. Starting hardware supervision...");
}

void loop() {
    system_supervisor.update();
    delay(20); 
}

#if 0 //code for arduino nano replacement
#include <Arduino.h>
#include "traffic_controller.hpp"

// Instancia global del supervisor¿
TrafficController system_supervisor;

void setup() {
    Serial.begin(115200);
    delay(500);
    
    system_supervisor.begin();
}

void loop() {
    system_supervisor.update();
    delay(20); 
}
#endif





















#if 0
// Definiciones de pines shift register de ENTRADA
#define PIN_PL       14  // Carga paralela / Latch
#define PIN_CLK      32  // Shift Clock de entrada
#define PIN_DATA     13  // Serial data de entrada 

// Definiciones de pines shift register de SALIDA
#define POUT_INDATA  12  // Serial data de salida
#define POUT_INLATCH 33  // Storage latch de salida
#define POUT_INCLK   15  // Shift clock de salida
#define POUT_INOE    27  // Output Enable
// Variables de guía
uint8_t lastState = 0xFF; 
bool firstRun = true;    

uint8_t read165() {
    uint8_t value = 0;

    digitalWrite(PIN_PL, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_PL, HIGH);

    for (int i = 0; i < 8; i++) {
        value <<= 1; 

        
        if (digitalRead(PIN_DATA)) {
            value |= 1; 
        }

        digitalWrite(PIN_CLK, HIGH);
        delayMicroseconds(1);
        digitalWrite(PIN_CLK, LOW);
        delayMicroseconds(1);
    }

    return value;
}


void ledwrite(uint8_t reg4, uint8_t reg3, uint8_t reg2, uint8_t reg1) {
    digitalWrite(POUT_INLATCH, LOW);

    shiftOut(POUT_INDATA, POUT_INCLK, MSBFIRST, reg4);
    shiftOut(POUT_INDATA, POUT_INCLK, MSBFIRST, reg3);
    shiftOut(POUT_INDATA, POUT_INCLK, MSBFIRST, reg2);
    shiftOut(POUT_INDATA, POUT_INCLK, MSBFIRST, reg1);

    digitalWrite(POUT_INLATCH, HIGH);
}


void setup() {
    Serial.begin(115200);
    delay(500);

    pinMode(PIN_PL, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    pinMode(PIN_DATA, INPUT);
    digitalWrite(PIN_PL, HIGH);
    digitalWrite(PIN_CLK, LOW);

    // Configuración de pines de salida
    pinMode(POUT_INDATA, OUTPUT);
    pinMode(POUT_INLATCH, OUTPUT);
    pinMode(POUT_INCLK, OUTPUT);
    pinMode(POUT_INOE, OUTPUT);

    digitalWrite(POUT_INOE, HIGH);   
    digitalWrite(POUT_INCLK, LOW);    
    digitalWrite(POUT_INLATCH, LOW);
    
    ledwrite(0xFF, 0xFF, 0xFF, 0xFF); 
    
    digitalWrite(POUT_INOE, LOW);   

}

void loop() {
    
    uint8_t current = read165();

    if (firstRun || (current != lastState)) {
        
        Serial.print("Estado de las entradas: [");

        
        for (int i = 0; i < 8; i++) {
            if (current & (1 << i)) {
                Serial.print(" 🟢 "); //(3.3V)
            } else {
                Serial.print(" 🔴 "); // (0V / GND)
            }
        }
        
        Serial.println("]");

        // Mapeo de LEDs de 3 en 3 (como son filas de 3 LEDs no puede ser secuencial)
        
        uint32_t ledPattern = 0xFFFFFFFF;

        //Se revisan los 8 bits de entrada para calcular sus posiciones
        for (int i = 0; i < 8; i++) {
            
            // Revisar si el bit de entrada en cuestión está encendido o apagado
            if (current & (1 << i)) {
                
                // Para mapear de 3 en 3 (Empezamos en el 4 y de ahí nos vamos porque son los blancos)
                int targetLED = 3 + (i * 3);
                int targetLED2 = 4 +(i * 3);
                int targetLED3 = 5 + (i * 3);
                // '1UL' se asegura de que el compilador lo tome como un numero de 32 bits
                ledPattern &= ~(1UL << targetLED);
                ledPattern &= ~(1UL << targetLED2);
                ledPattern &= ~(1UL << targetLED3);
            }
        }
        // Parte la secuencia de 32 bits en 4 bytes de 8 bits para los shift registers
        uint8_t reg1 = ledPattern & 0xFF;           // Targets LEDs 0 to 7
        uint8_t reg2 = (ledPattern >> 8) & 0xFF;    // Targets LEDs 8 to 15
        uint8_t reg3 = (ledPattern >> 16) & 0xFF;   // Targets LEDs 16 to 23
        uint8_t reg4 = (ledPattern >> 24) & 0xFF;   // Targets LEDs 24 to 31

        ledwrite(reg4, reg3, reg2, reg1);



        lastState = current;
        firstRun = false; 
    }

    delay(30); 
}
#endif

#if 0
// Pines
#define PIN_PL   14
#define PIN_CLK  32
#define PIN_DATA 13

#define POUT_INDATA 12
#define POUT_INLATCH 33
#define POUT_INCLK 15
#define POUT_INOE 27



uint8_t lastState = 0;


uint8_t read165() {
    uint8_t value = 0;

    digitalWrite(PIN_PL, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_PL, HIGH);

    for (int i = 0; i < 8; i++) {
        value <<= 1;

        if (digitalRead(PIN_DATA)) {
            value |= 1;
        }

        digitalWrite(PIN_CLK, HIGH);
        delayMicroseconds(1);
        digitalWrite(PIN_CLK, LOW);
        delayMicroseconds(1);
    }

    return value;
}

void setup() {
    pinMode(PIN_PL, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    pinMode(PIN_DATA, INPUT);

    digitalWrite(PIN_PL, HIGH);
    digitalWrite(PIN_CLK, LOW);

    Serial.begin(115200);
}

void loop() {
    uint8_t current = read165();

    uint8_t changes = current ^ lastState;

    if (changes != 0) {
        for (int i = 0; i < 8; i++) {
            if (changes & (1 << i)) {
                Serial.print("Cambio en bit ");
                Serial.print(i);
                Serial.print(" -> ");

                if (current & (1 << i)) {
                    Serial.println("HIGH");
                } else {
                    Serial.println("LOW");
                }
            }
        }
    }

    lastState = current;

    delay(100);
}
#endif
 