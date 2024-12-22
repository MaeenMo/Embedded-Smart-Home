#ifndef RELAY_H
#define RELAY_H

#include <stdint.h>
#include "../DIO/DIO.h"

// Relay states
#define RELAY_ON  0
#define RELAY_OFF 1

void Relay_Init(char port, uint8_t pin1, uint8_t pin2);
void Relay_Control(char port, uint8_t pin1, uint8_t pin2, char c);

#endif // RELAY_H
