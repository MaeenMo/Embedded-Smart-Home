#include "DataLogging.h"

static uint32_t doorOpenLog = 0;
static uint32_t doorCloseLog = 0;

// Initialize data logging
void DoorLogging_init(void) {
    doorOpenLog = 0;
    doorCloseLog = 0;
}

// Log door open/close event
void setDataLog(bool isOpen) {
    if (isOpen) {
        doorOpenLog++;
    } else {
        doorCloseLog++;
    }
}

// Get number of times door was opened
uint32_t getDoorOpenLog(void) {
    return doorOpenLog;
}

// Get number of times door was closed
uint32_t getDoorCloseLog(void) {
    return doorCloseLog;
}
