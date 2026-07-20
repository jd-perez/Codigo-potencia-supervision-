#ifndef SERVER_NOTIFIER_HPP
#define SERVER_NOTIFIER_HPP

#include <Arduino.h>

// System constants 
#define TOTAL_PHASES   10
#define PINS_PER_PHASE 3  // Green, Yellow, Red
#define TOTAL_BULBS    (TOTAL_PHASES * PINS_PER_PHASE) // 30 Bulbs

class ServerNotifier {
public:
    void begin();
    void send_alert(uint32_t physical_state, uint32_t expected_state);
};

#endif


#if 0 //code for arduino nano replacement
#ifndef SERVER_NOTIFIER_HPP
#define SERVER_NOTIFIER_HPP

#include <Arduino.h>

class ServerNotifier {
public:
    void begin();
    void send_alert(uint8_t physical_state, uint8_t expected_state);
};

#endif
#endif