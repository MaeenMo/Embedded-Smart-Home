#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "../../tm4c123gh6pm.h"
#include "../../bitwise_operations.h"
#include "../DIO/DIO.h"

// UART Base Addresses
#define UART1_BASE  0x4000D000
#define UART3_BASE  0x4000F000
#define UART5_BASE  0x40011000

// Function prototypes
void UART_Init(uint8_t uartNum, uint32_t baudRate);
void UART_Transmit(uint8_t uartNum, char data);
char UART_Receive(uint8_t uartNum);

#endif
