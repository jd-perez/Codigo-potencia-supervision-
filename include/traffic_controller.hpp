#ifndef TRAFFIC_CONTROLLER_HPP
#define TRAFFIC_CONTROLLER_HPP

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "shiftreg_in.hpp"
#include "shiftreg_out.hpp"
#include "state_simulator.hpp"
#include "server_notifier.hpp"

// NeoPixel Configuration
#define PIN_NEOPIXEL  21  
#define NUM_PIXELS    10  

class TrafficController {
private:
    ShiftRegIn reader;
    ShiftRegOut writer;
    StateSimulator simulator;
    ServerNotifier notifier;
    Adafruit_NeoPixel strip;

    uint32_t last_alert_time;
    uint32_t mismatch_start_time; 
    bool is_mismatch_active;      

    void update_neopixels(uint32_t ideal_state);

public:
    TrafficController();
    void begin();
    void update();
};

#endif


#if 0 //code for arduino nano replacement
#ifndef TRAFFIC_CONTROLLER_HPP
#define TRAFFIC_CONTROLLER_HPP

#include <Arduino.h>
#include "shiftreg_in.hpp"
#include "shiftreg_out.hpp"
#include "state_simulator.hpp"
#include "server_notifier.hpp"

class TrafficController {
private:
    ShiftRegIn reader;
    ShiftRegOut writer;
    StateSimulator simulator;
    ServerNotifier notifier;

    uint32_t last_alert_time;
    
    // VARIABLES RESTAURADAS PARA EL DEBOUNCE
    uint32_t mismatch_start_time; 
    bool is_mismatch_active;      

public:
    TrafficController();
    void begin();
    void update();
};

#endif
#endif