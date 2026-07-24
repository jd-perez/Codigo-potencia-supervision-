#include <Arduino.h>
#include "traffic_controller.hpp"

TrafficController system_supervisor;

void setup() {
    Serial.begin(115200);
    delay(500); 
    
    system_supervisor.begin();
    Serial.println("System Initialized. Starting hardware supervision...");
}

void loop() {
    system_supervisor.update();
    // 20ms delay is shorter than the 30ms debounce window, ensuring high resolution polling
    delay(20); 
}