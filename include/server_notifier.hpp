// server_notifier.hpp
#ifndef SERVER_NOTIFIER_HPP
#define SERVER_NOTIFIER_HPP

#include <Arduino.h>

// Fault definitions
enum PhaseStatus {
  STATUS_OK,
  FAULT_BURNT_OR_SHORTED, 
  FAULT_OPEN_TRIAC        
};

class ServerNotifier {
public:
    void begin();
    void send_alert(uint8_t software_bit_index, PhaseStatus fault_type);
};

#endif