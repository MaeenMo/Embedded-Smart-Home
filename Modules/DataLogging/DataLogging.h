#ifndef DATALOGGING_H
#define DATALOGGING_H

#include <stdint.h>
#include <stdbool.h>

void DoorLogging_init(void);
void setDataLog(bool isOpen);
uint32_t getDoorOpenLog(void);
uint32_t getDoorCloseLog(void);

#endif // DATALOGGING_H
