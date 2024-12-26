#include "DoorStatus.h"

volatile bool doorStatus = false;  // Tracks door status: true = open, false = closed

// Initialize door status module
void DoorStatus_init(char port, uint8_t pins) {
    dio_init(port, pins, Input);
    GPIO_PORTB_PUR_R &= ~pins;
    GPIO_PORTB_PDR_R |= pins;  // Enable pull-down resistor

    // 3. Configure for edge-triggered interrupts
    GPIO_PORTB_IS_R &= ~pins;   // make pin edge-sensitive
    GPIO_PORTB_IBE_R |= pins;   // Interrupt on both rising and falling edges

    // 4. Clear any prior interrupts and enable interrupt
    GPIO_PORTB_ICR_R |= pins;   // Clear any prior interrupt
    GPIO_PORTB_IM_R |= pins;    // Enable interrupt

    // 5. Configure NVIC to handle Port interrupts
    NVIC_EN0_R |= (1 << 1);
    NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFFFF1F) | (2 << 5); // Set priority to 2 (bits 5-7)
}

// ISR => For Door Sensor at Port B, Pin 0
void Door_Handler(void) {
    if (GPIO_PORTB_RIS_R & (1 << 0)) {  // Check if PB0 caused the interrupt
        SysTick_Init(3 * (16000 - 1));
        SysTick_Wait();  // Wait for 3 ms for debounce
        // Check the current state of PB0
        if (((GPIO_PORTB_DATA_R & (1 << 0)) == 0) && !doorStatus) {  // Door open (PB0 LOW)
            UART_Transmit(5,'O');  // Send 'O' for Door Open
        }
        else if (((GPIO_PORTB_DATA_R & (1 << 0)) == 1) && doorStatus) {  // Door closed (PB0 HIGH)
            UART_Transmit(5,'C');  // Send 'C' for Door Closed
        }
    }
    SysTick_Init(3 * (16000 - 1));
    SysTick_Wait();  // Wait for 3 ms for debounce
    GPIO_PORTB_ICR_R |= (1 << 0);  // Clear interrupt flag for PB0
}

void setCurrentDoorStatus(char c){
    if (c == 'D') {
        if ((GPIO_PORTB_DATA_R & (1 << 0)) == 0)
            UART_Transmit(5,'D');
        else if ((GPIO_PORTB_DATA_R & (1 << 0)) == 1)
            UART_Transmit(5,'d');
    } else if (c == 'O'){
        doorStatus = true;
    } else if (c == 'C'){
        doorStatus = false;
    }
}
