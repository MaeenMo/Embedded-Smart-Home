#include "Relay.h"

// Initialize the relay pins
void Relay_Init(char port, uint8_t pin1, uint8_t pin2) {
    dio_init(port, (pin1 | pin2), Output);
    dio_writepin(port, pin1, 1); // Deactivate lamp relay
    dio_writepin(port, pin2, 1); // Deactivate plug relay
}

// Control the relay state (ON/OFF)
void Relay_Control(char port, uint8_t pin1, uint8_t pin2, char c) {
    if (c == 'L') {
        dio_writepin(port, pin1, RELAY_ON);
    } else if (c == 'l') {
        dio_writepin(port, pin1, RELAY_OFF);
    } else if (c == 'P') {
        dio_writepin(port, pin2, RELAY_ON);
    } else if (c == 'p') {
        dio_writepin(port, pin2, RELAY_OFF);
    }
}
