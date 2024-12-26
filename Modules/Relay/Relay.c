#include "Relay.h"

// Initialize the relay pins
void Relay_Init(char port, uint8_t pin1, uint8_t pin2) {
    dio_init(port, (pin1 | pin2), Output);
    dio_writepin(port, pin1, 1); // Deactivate lamp relay
    dio_writepin(port, pin2, 1); // Deactivate plug relay
}

// Control the relay state (ON/OFF)
void Relay_Control(char port, uint8_t pin1, uint8_t pin2, char c) {
    static uint8_t lastStatePE1 = 0;  // Track last known state of PE1
    static uint8_t lastStatePE2 = 0;  // Track last known state of PE2
    if (c == 'L') {
        GPIO_PORTE_DEN_R |= (1 << 1); // Enable digital functionality for PE1
        dio_writepin(port, pin1, RELAY_ON);
        if ((GPIO_PORTE_DATA_R & (1 << 2)) != 0) {
            GPIO_PORTE_DEN_R &= ~(1 << 2); // Disable digital functionality for PE2
        }
    } else if (c == 'l') {
        dio_writepin(port, pin1, RELAY_OFF);
    } else if (c == 'P') {
        GPIO_PORTE_DEN_R |= (1 << 2); // Enable digital functionality for PE2
        dio_writepin(port, pin2, RELAY_ON);
        if ((GPIO_PORTE_DATA_R & (1 << 1)) != 0) {
            GPIO_PORTE_DEN_R &= ~(1 << 1);// Disable digital functionality for PE1
        }
    } else if (c == 'p') {
        dio_writepin(port, pin2, RELAY_OFF);
    }
}
