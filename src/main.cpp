#include <Arduino.h>

// Pines
#define PIN_PL   14
#define PIN_CLK  32
#define PIN_DATA 13

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