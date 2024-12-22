#include "UART.h"

volatile char received_char = 0;

// Initialize UART
void UART_Init(uint8_t uartNum, uint32_t baudRate) {
    uint32_t uartBase, gpioBase, rxPin, txPin;

    switch (uartNum) {
        case 1:
            uartBase = UART1_BASE;
            gpioBase = GPIO_PORTB_BASE;
            rxPin = Pin0;
            txPin = Pin1;
            SYSCTL_RCGCUART_R |= (1 << 1);              // Enable UART1 clock
            SYSCTL_RCGCGPIO_R |= (1 << 1);              // Enable GPIOB clock
            while ((SYSCTL_PRUART_R & (1 << 1)) == 0);
            while ((SYSCTL_PRGPIO_R & (1 << 1)) == 0);
            UART1_IM_R |= (1 << 4);                     // Enable RX interrupt
            break;
        case 3:
            uartBase = UART3_BASE;
            gpioBase = GPIO_PORTC_BASE;
            rxPin = Pin6;
            txPin = Pin7;
            SYSCTL_RCGCUART_R |= (1 << 3);  // Enable UART3 clock
            SYSCTL_RCGCGPIO_R |= (1 << 2);  // Enable GPIOC clock
            while ((SYSCTL_PRUART_R & (1 << 3)) == 0);
            while ((SYSCTL_PRGPIO_R & (1 << 2)) == 0);
            break;
        case 5:
            uartBase = UART5_BASE;
            gpioBase = GPIO_PORTE_BASE;
            rxPin = Pin4;
            txPin = Pin5;
            SYSCTL_RCGCUART_R |= (1 << 5);  // Enable UART5 clock
            SYSCTL_RCGCGPIO_R |= (1 << 4);  // Enable GPIOE clock
            while ((SYSCTL_PRUART_R & (1 << 5)) == 0);
            while ((SYSCTL_PRGPIO_R & (1 << 4)) == 0);
            UART5_IM_R |= (1 << 4);     // Enable RX interrupt
            NVIC_EN1_R |= (1 << 29);    // Enable IRQ29 for UART5 in NVIC
            break;
        default:
            return; // Invalid UART number
    }

    // Configure RX/TX pins
    GPIO_PORT_AFSEL_R(gpioBase) |= (rxPin | txPin);
    GPIO_PORT_PCTL_R(gpioBase) |= (1 << (ctz(rxPin) * 4)) | (1 << (ctz(txPin) * 4));
    GPIO_PORT_DEN_R(gpioBase) |= (rxPin | txPin);

    // Configure UART
    UART_CTL_R(uartBase) = 0;            // Disable UART
    UART_IBRD_R(uartBase) = 16000000 / (16 * baudRate); // Integer part of baud rate
    UART_FBRD_R(uartBase) = (16000000 % (16 * baudRate)) * 64 / (16 * baudRate); // Fractional part
    UART_CC_R(uartBase) = 0;            // Use system clock
    UART_LCRH_R(uartBase) = 0x60;        // 8 data bits, 1 stop bit, no parity
    UART_CTL_R(uartBase) = 0x301;        // Enable UART, TX, RX
}

// Transmit a character
void UART_Transmit(uint8_t uartNum, char data) {
    uint32_t uartBase;
    switch (uartNum) {
        case 1: uartBase = UART1_BASE; break;
        case 3: uartBase = UART3_BASE; break;
        case 5: uartBase = UART5_BASE; break;
        default: return;
    }
    while ((UART_FR_R(uartBase) & (1 << 5)) != 0); // Wait for TX buffer to be empty
    UART_DR_R(uartBase) = data;
}

// Transmit a string
void UART_Transmit_String(uint8_t uartNum, const char *str) {
    while (*str != '\0') {              // Loop until the end of the string (null terminator)
        UART_Transmit(uartNum, *str++); // Transmit each character
    }
}

// Transmit a character
void UART_Transmit_Temperature(uint8_t uartNum, uint8_t data) {
    uint8_t num_size = snprintf(0, 0, "%d", data) + 1;
    char* str = (char *)malloc(num_size);
    // Convert the number to a string
    sprintf(str, "%d", data);
    for (uint8_t i = 0; str[i] != '\0'; i++) {
        UART_Transmit(uartNum, str[i]);
    }
    free(str);
}

// Receive a character
char UART_Receive(uint8_t uartNum) {
    uint32_t uartBase;
    switch (uartNum) {
        case 1: uartBase = UART1_BASE; break;
        case 3: uartBase = UART3_BASE; break;
        case 5: uartBase = UART5_BASE; break;
        default: return 0;
    }
    while ((UART_FR_R(uartBase) & (1 << 4)) != 0); // Wait for RX buffer to have data
    return (char)(UART_DR_R(uartBase) & 0xFF);
}

// Interrupt Service Routine for UART5
void UART5_Handler(void) {
    while (!(UART5_FR_R & (1 << 4))) {  // While RX buffer is not empty
        received_char = (char)(UART5_DR_R & 0xFF); // Read the character
        if (received_char == '0') {
            break;
        }
        Relay_Control('E', Pin1, Pin2, received_char);
        tempEN(received_char);
        setCurrentDoorStatus(received_char);
    }
    UART5_ICR_R = (1 << 4); // Clear RX interrupt flag
}