#ifndef DOORSTATUS_H
#define DOORSTATUS_H

#include <stdbool.h>
#include "../UART/UART.h"

void DoorStatus_init(char port, uint8_t pins);
void Door_Handler(void);
void setCurrentDoorStatus(char c);

#endif // DOORSTATUS_H
