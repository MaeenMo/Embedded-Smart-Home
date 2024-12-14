#include "Relay.h"

// Initialize the relay pins
void Relay_Init(char port, uint8_t pins) {
    dio_init(port, pins, Output);
    dio_writepin('E', Pin1, 1); // Deactivate lamp relay
    dio_writepin('E', Pin2, 1); // Deactivate plug relay
}

// Control the relay state (ON/OFF)
void Relay_Control(char port, uint8_t pin, uint8_t state) {
    if (state == RELAY_ON) {
        dio_writepin(port, pin, 0); // Activate relay (active low)
    } else if (state == RELAY_OFF) {
        dio_writepin(port, pin, 1); // Deactivate relay
    }
}
