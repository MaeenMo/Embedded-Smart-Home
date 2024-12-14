#include "LM35.h"
#include "../UART/UART.h"

static uint32_t temperature = 0;

// Initialize LM35 module
void LM35_init(void) {
    UART_Init(3, 9600); // UART3 for LM35 temperature sensor
}

// Get current temperature
uint32_t GetTemp(void) {
    char tempData = UART_Receive(3); // Read temperature data from UART3
    temperature = (uint32_t)tempData; // Convert to integer
    return temperature;
}
