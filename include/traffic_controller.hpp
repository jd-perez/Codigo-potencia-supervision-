#ifndef TRAFFIC_CONTROLLER_HPP
#define TRAFFIC_CONTROLLER_HPP

#include <Arduino.h>
#include "shiftreg_in.hpp"
#include "shiftreg_out.hpp"
#include "state_simulator.hpp"
#include "server_notifier.hpp"

#define TOTAL_PHASES  3 // 1 phase * 3 colors for the Proteus simulation

struct TrafficPhase {
    bool isCommandedOn;
    bool isSensorActive;
    PhaseStatus health;
    unsigned long lastFaultTime;
    bool isDebouncing;
    bool alertSent;
};

class TrafficController {
private:
    ShiftRegIn reader;
    ShiftRegOut writer;
    StateSimulator simulator;
    ServerNotifier notifier;

    TrafficPhase phases[TOTAL_PHASES];
    const unsigned long ZERO_CROSS_DEBOUNCE_MS = 30;

    void evaluate_phase(int software_bit_index);

public:
    TrafficController();
    void begin();
    void update();
};

#endif