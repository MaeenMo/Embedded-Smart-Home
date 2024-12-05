#include <stdint.h>

#ifndef DOORLOGGING_H
#define DOORLOGGING_H

// Add 2 static var for open / close counter in .c
void DoorLogging_init(void);
void setDataLog(void);
uint32_t getDoorOpenLog(void);
uint32_t getDoorCloseLog(void);

#endif