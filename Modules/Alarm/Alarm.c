#include "Alarm.h"

// Initialize the alarm (buzzer) pins
void Alarm_init(char port, uint8_t pins) {
    dio_init(port, pins, Output);
}

// Toggle the alarm (buzzer) state
void Toggle_alarm(char port, uint8_t pins) {
    dio_writeport(port, dio_readport(port) ^ pins);
}
