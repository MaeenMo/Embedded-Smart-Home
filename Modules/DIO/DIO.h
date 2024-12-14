#ifndef DIO_H
#define DIO_H

#include <stdint.h>
#include "../../tm4c123gh6pm.h"
#include "../../bitwise_operations.h"

// Define direction constants for Input and Output
#define Input  0
#define Output 1

// Pin mask values
#define Pin0   (1U << 0)
#define Pin1   (1U << 1)
#define Pin2   (1U << 2)
#define Pin3   (1U << 3)
#define Pin4   (1U << 4)
#define Pin5   (1U << 5)
#define Pin6   (1U << 6)
#define Pin7   (1U << 7)

// GPIO port base addresses
#define GPIO_PORTA_BASE  0x40004000
#define GPIO_PORTB_BASE  0x40005000
#define GPIO_PORTC_BASE  0x40006000
#define GPIO_PORTD_BASE  0x40007000
#define GPIO_PORTE_BASE  0x40024000
#define GPIO_PORTF_BASE  0x40025000

// Define the necessary registers for each port
#define GPIO_PORT_DIR_R(port_base)       (*(volatile uint32_t *)(port_base + 0x400))  // Direction register
#define GPIO_PORT_DEN_R(port_base)       (*(volatile uint32_t *)(port_base + 0x51C))  // Digital enable register
#define GPIO_PORT_PUR_R(port_base)       (*(volatile uint32_t *)(port_base + 0x510))  // Pull-up resistor register
#define GPIO_PORT_LOCK_R(port_base)      (*(volatile uint32_t *)(port_base + 0x520))  // Lock register
#define GPIO_PORT_CR_R(port_base)        (*(volatile uint32_t *)(port_base + 0x524))  // Commit register
#define GPIO_PORT_DATA_BITS_R(port_base) ((volatile uint32_t *)(port_base))           // Data bits register
#define GPIO_PORT_AFSEL_R(gpioBase)    (*((volatile uint32_t *)(gpioBase + 0x420)))  // GPIO Alternate Function Select
#define GPIO_PORT_PCTL_R(gpioBase)     (*((volatile uint32_t *)(gpioBase + 0x52C)))  // GPIO Port Control

// UART Registers
#define UART_CTL_R(uartBase)        (*((volatile uint32_t *)(uartBase + 0x030)))  // UART Control
#define UART_IBRD_R(uartBase)       (*((volatile uint32_t *)(uartBase + 0x024)))  // Integer Baud Rate Divisor
#define UART_FBRD_R(uartBase)       (*((volatile uint32_t *)(uartBase + 0x028)))  // Fractional Baud Rate Divisor
#define UART_CC_R(uartBase)         (*((volatile uint32_t *)(uartBase + 0xFC8)))  // Clock Control
#define UART_LCRH_R(uartBase)       (*((volatile uint32_t *)(uartBase + 0x02C)))  // Line Control
#define UART_DR_R(uartBase)         (*((volatile uint32_t *)(uartBase + 0x000)))  // Data Register
#define UART_FR_R(uartBase)         (*((volatile uint32_t *)(uartBase + 0x018)))  // Flag Register



// Function prototypes
void dio_init(char port, uint8_t pins, uint8_t direction);
uint8_t dio_readpin(char port, uint8_t pin);
uint8_t dio_readport(char port);
void dio_writepin(char port, uint8_t pin, uint8_t value);
void dio_writeport(char port, uint8_t value);
uint8_t ctz(uint32_t value);

#endif // DIO_H