#ifndef DOORSTATUS_H
#define DOORSTATUS_H

void DoorStatus_init(char port, uint8_t pins);
bool getDoorStatus(void);

#endif