#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>
#include "../DIO/DIO.h"

void Alarm_init(char port, uint8_t pins);
void Toggle_alarm(char port, uint8_t pins);

#endif // ALARM_H
