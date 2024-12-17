#include "DoorStatus.h"
#include "../UART/UART.h"
#include "DoorStatus.h"

static bool doorStatus = false;

// Initialize door status module
void DoorStatus_init(void) {
    UART_Init(1, 9600); // UART1 for door sensor
}

// Get door status (open/closed)
bool getDoorStatus(void) {
    char status = UART_Receive(1); // Receive status from door sensor
    doorStatus = (status == '0');  // '0' means door open, '1' means door closed
    return doorStatus;
}
